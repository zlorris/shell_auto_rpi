#include <airsim_ros_wrapper.h>
#include <boost/make_shared.hpp>
// #include <pluginlib/class_list_macros.h>
// PLUGINLIB_EXPORT_CLASS(AirsimROSWrapper, nodelet::Nodelet)
#include "common/AirSimSettings.hpp"
#include <swri_profiler/profiler.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

constexpr char AirsimROSWrapper::CAM_YML_NAME[];
constexpr char AirsimROSWrapper::WIDTH_YML_NAME[];
constexpr char AirsimROSWrapper::HEIGHT_YML_NAME[];
constexpr char AirsimROSWrapper::K_YML_NAME[];
constexpr char AirsimROSWrapper::D_YML_NAME[];
constexpr char AirsimROSWrapper::R_YML_NAME[];
constexpr char AirsimROSWrapper::P_YML_NAME[];
constexpr char AirsimROSWrapper::DMODEL_YML_NAME[];

const std::unordered_map<int, std::string> AirsimROSWrapper::image_type_int_to_string_map_ = {
  {  0, "Scene"}
  , {1, "DepthPlanner"}
  , {2, "DepthPerspective"}
  , {3, "DepthVis"}
  , {4, "DisparityNormalized"}
  , {5, "Segmentation"}
  , {6, "SurfaceNormals"}
  , {7, "Infrared"}
};

AirsimROSWrapper::AirsimROSWrapper(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private)
  : img_async_spinner_(1, &img_timer_cb_queue_) // a thread for image callbacks to be 'spun' by img_async_spinner_
  , lidar_async_spinner_(1, &lidar_timer_cb_queue_) // same as above, but for lidar
  , is_used_lidar_timer_cb_queue_(false)
  , is_used_img_timer_cb_queue_(false)
  , is_vulkan_(false)
  , nh_(nh)
  , nh_private_(nh_private)
  , has_gimbal_cmd_(false)
  , vel_cmd_duration_(0.0)
  , publish_vehicle_tf_(false)
  , publish_enu_(true)
  , imu_calibration_(1.0)
  , speed_calibration_(1.0)
{
  if (publish_enu_)
  {
    world_frame_id_ = "world_enu"; // todo rosparam?
  }
  else
  {
    world_frame_id_ = "world_ned"; // todo rosparam?
  }
  

  initialize_ros();

  ROS_INFO("AirsimROSWrapper Initialized!");
  // intitialize placeholder control commands
  // vel_cmd_ = VelCmd();
  // gimbal_cmd_ = GimbalCmd();
}

void AirsimROSWrapper::initialize_airsim()
{
  // todo do not reset if already in air?
  try {
    airsim_client_.confirmConnection();
    airsim_client_images_.confirmConnection();
    airsim_client_lidar_.confirmConnection();

    for (const auto& vehicle_name : vehicle_names_) {
      airsim_client_.enableApiControl(true, vehicle_name); // todo expose as rosservice?
      airsim_client_.armDisarm(true, vehicle_name); // todo exposes as rosservice?
    }

    origin_geo_point_ = airsim_client_.getHomeGeoPoint("");
    // todo there's only one global origin geopoint for environment. but airsim API accept a parameter vehicle_name? inside carsimpawnapi.cpp, there's a geopoint being assigned in the constructor. by? 
    origin_geo_point_msg_ = get_gps_msg_from_airsim_geo_point(origin_geo_point_);
  }
  catch (rpc::rpc_error& e) {
    std::string msg = e.get_error().as<std::string>();
    ROS_ERROR("Exception raised by the RPC API: %s", msg.c_str());
  }
}

void AirsimROSWrapper::initialize_ros()
{

  params_.initialize(nh_private_);

  params_.get("imu_calibration", imu_calibration_,
              1.0 /*default*/, "Imu calibration factor",
              0.0 /*min */, 2.0 /*max*/);
  params_.get("speed_calibration", speed_calibration_,
              1.0 /*default*/, "Wheel speed calibration factor",
              0.0 /*min */, 2.0 /*max*/);
  params_.finalize();
  params_.setCallback(boost::bind(&AirsimROSWrapper::callback, this, _1));

  // ros params
  double update_airsim_control_every_n_sec;
  nh_private_.getParam("is_vulkan", is_vulkan_);
  nh_private_.getParam("update_airsim_control_every_n_sec", update_airsim_control_every_n_sec);
  nh_private_.param("publish_vehicle_tf", publish_vehicle_tf_, false);
  nh_private_.param("publish_enu", publish_enu_, true);
  vel_cmd_duration_ = 0.05; // todo rosparam
  // todo enforce dynamics constraints in this node as well?
  // nh_.getParam("max_vert_vel_", max_vert_vel_);
  // nh_.getParam("max_horz_vel", max_horz_vel_)

  create_ros_pubs_from_settings_json();
  airsim_control_update_timer_ = nh_private_.createTimer(ros::Duration(update_airsim_control_every_n_sec),
                                                         &AirsimROSWrapper::drone_state_timer_cb, this);
}

// XmlRpc::XmlRpcValue can't be const in this case
void AirsimROSWrapper::create_ros_pubs_from_settings_json()
{
  // subscribe to control commands on global nodehandle
  //    gimbal_angle_quat_cmd_sub_ = nh_private_.subscribe("gimbal_angle_quat_cmd", 50, &AirsimROSWrapper::gimbal_angle_quat_cmd_cb, this);
  //    gimbal_angle_euler_cmd_sub_ = nh_private_.subscribe("gimbal_angle_euler_cmd", 50, &AirsimROSWrapper::gimbal_angle_euler_cmd_cb, this);
  origin_geo_point_pub_ = nh_private_.advertise<airsim_car_ros_pkgs::GPSYaw>("origin_geo_point", 10);

  airsim_img_request_vehicle_name_pair_vec_.clear();
  image_pub_vec_.clear();
  cam_info_pub_vec_.clear();
  camera_info_msg_vec_.clear();
  static_tf_msg_vec_.clear();
  imu_pub_vec_.clear();
  gps_pub_vec_.clear();
  lidar_pub_vec_.clear();
  vehicle_names_.clear(); // todo should eventually support different types of vehicles in a single instance
  // vehicle_setting_vec_.clear();
  // vehicle_imu_map_;
  car_ros_vec_.clear();
  // callback_queues_.clear();

  image_transport::ImageTransport image_transporter(nh_private_);

  int idx = 0;
  // iterate over std::map<std::string, std::unique_ptr<VehicleSetting>> vehicles;
  for (const auto& curr_vehicle_elem : AirSimSettings::singleton().vehicles) {
    auto& vehicle_setting = curr_vehicle_elem.second;
    auto curr_vehicle_name = curr_vehicle_elem.first;
    ROS_INFO("Found car: %s", curr_vehicle_name.c_str());
    vehicle_names_.push_back(curr_vehicle_name);
    set_nans_to_zeros_in_pose(*vehicle_setting);
    // auto vehicle_setting_local = vehicle_setting.get();

    append_static_vehicle_tf(curr_vehicle_name, *vehicle_setting);
    vehicle_name_idx_map_[curr_vehicle_name] = idx; // allows fast lookup in command callbacks in case of a lot of drones  

    CarROS car_ros;
    if (publish_enu_)
    {
      car_ros.odom_frame_id = curr_vehicle_name + "/odom_local_enu";
    }
    else
    {
      car_ros.odom_frame_id = curr_vehicle_name + "/odom_local_ned";
    }
    
    car_ros.vehicle_name = curr_vehicle_name;
    if (publish_enu_)
    {
      car_ros.odom_local_ned_pub = nh_private_.advertise<nav_msgs::Odometry>(curr_vehicle_name + "/odom_local_enu", 10);
    }
    else
    {
      car_ros.odom_local_ned_pub = nh_private_.advertise<nav_msgs::Odometry>(curr_vehicle_name + "/odom_local_ned", 10);
    }
    
    //    car_ros.global_gps_pub = nh_private_.advertise<sensor_msgs::NavSatFix>(curr_vehicle_name + "/global_gps", 10);
    car_ros.collision_pub = nh_private_.advertise<std_msgs::Int32>(curr_vehicle_name + "/collision_count", 10);
    //
    // bind to a single callback. todo optimal subs queue length
    // bind multiple topics to a single callback, but keep track of which vehicle name it was by passing curr_vehicle_name as the 2nd argument
    car_ros.car_cmd_body_frame_sub = nh_private_.subscribe<airsim_car_ros_pkgs::CarCmd>(
      curr_vehicle_name + "/car_cmd_body_frame",
      1,
      boost::bind(&AirsimROSWrapper::car_cmd_frame_cb,
                  this, _1, car_ros.vehicle_name));
    //	          car_ros.vel_cmd_body_frame_sub = nh_private_.subscribe<airsim_car_ros_pkgs::VelCmd>(curr_vehicle_name + "/vel_cmd_body_frame", 1, 
    //  boost::bind(&AirsimROSWrapper::vel_cmd_body_frame_cb, this, _1, car_ros.vehicle_name)); // todo ros::TransportHints().tcpNoDelay();
    //car_ros.vel_cmd_world_frame_sub = nh_private_.subscribe<airsim_car_ros_pkgs::VelCmd>(curr_vehicle_name + "/vel_cmd_world_frame", 1, 
    //  boost::bind(&AirsimROSWrapper::vel_cmd_world_frame_cb, this, _1, car_ros.vehicle_name));


    car_ros_vec_.push_back(car_ros);
    idx++;

    // iterate over camera map std::map<std::string, CameraSetting> cameras;
    for (auto& curr_camera_elem : vehicle_setting->cameras) {
      auto& camera_setting = curr_camera_elem.second;
      auto& curr_camera_name = curr_camera_elem.first;
      // vehicle_setting_vec_.push_back(*vehicle_setting.get());
      set_nans_to_zeros_in_pose(*vehicle_setting, camera_setting);
      append_static_camera_tf(curr_vehicle_name, curr_camera_name, camera_setting);
      // camera_setting.gimbal
      std::vector<ImageRequest> current_image_request_vec;
      current_image_request_vec.clear();

      // iterate over capture_setting std::map<int, CaptureSetting> capture_settings
      for (const auto& curr_capture_elem : camera_setting.capture_settings) {
        auto& capture_setting = curr_capture_elem.second;

        // todo why does AirSimSettings::loadCaptureSettings calls AirSimSettings::initializeCaptureSettings()
        // which initializes default capture settings for _all_ NINE msr::airlib::ImageCaptureBase::ImageType
        if (!(std::isnan(capture_setting.fov_degrees))) {
          ImageType curr_image_type = msr::airlib::Utils::toEnum<ImageType>(capture_setting.image_type);
          // if scene / segmentation / surface normals / infrared, get uncompressed image with pixels_as_floats = false
          if (capture_setting.image_type == 0 || capture_setting.image_type == 5 || capture_setting.image_type == 6 ||
              capture_setting.image_type == 7) {
            current_image_request_vec.emplace_back(curr_camera_name, curr_image_type, false, false);
          }
            // if {DepthPlanner, DepthPerspective,DepthVis, DisparityNormalized}, get float image
          else {
            current_image_request_vec.emplace_back(curr_camera_name, curr_image_type, true);
          }

          image_pub_vec_.push_back(image_transporter.advertise(curr_vehicle_name + "/" + curr_camera_name + "/" +
                                                               image_type_int_to_string_map_.at(
                                                                 capture_setting.image_type), 1));
          cam_info_pub_vec_.push_back(nh_private_.advertise<sensor_msgs::CameraInfo>(
            curr_vehicle_name + "/" + curr_camera_name + "/" +
            image_type_int_to_string_map_.at(capture_setting.image_type) + "/camera_info", 10));
          camera_info_msg_vec_.push_back(generate_cam_info(curr_camera_name, camera_setting, capture_setting));
        }
      }
      // push back pair (vector of image captures, current vehicle name) 
      airsim_img_request_vehicle_name_pair_vec_.emplace_back(current_image_request_vec, curr_vehicle_name);

    }

    ROS_INFO("Sensor Init");
    // iterate over sensors std::map<std::string, std::unique_ptr<SensorSetting>> sensors;
    for (auto& curr_sensor_map : vehicle_setting->sensors) {
      auto& sensor_name = curr_sensor_map.first;
      auto& sensor_setting = curr_sensor_map.second;

      ROS_INFO("Sensor: Name( %s )", curr_sensor_map.first.c_str());

      switch (sensor_setting->sensor_type) {
        case SensorBase::SensorType::Barometer: {
          ROS_INFO("Sensor: Type ( Barometer )");
          break;
        }
        case SensorBase::SensorType::Imu: {
          vehicle_imu_map_[curr_vehicle_name] = sensor_name;
          // todo this is pretty non scalable, refactor airsim and ros api and maintain a vehicle <-> sensor (setting) map
          ROS_INFO("Sensor: Type ( IMU )");
          imu_pub_vec_.push_back(
            nh_private_.advertise<sensor_msgs::Imu>(curr_vehicle_name + "/imu/" + sensor_name, 10));
          break;
        }
        case SensorBase::SensorType::Gps: {
          vehicle_gps_map_[curr_vehicle_name] = sensor_name;
          // todo this is pretty non scalable, refactor airsim and ros api and maintain a vehicle <-> sensor (setting) map
          ROS_INFO("Sensor: Type ( GPS )");
          gps_pub_vec_.push_back(
            nh_private_.advertise<sensor_msgs::NavSatFix>(curr_vehicle_name + "/global_gps/" + sensor_name, 10));
          break;
        }
        case SensorBase::SensorType::Magnetometer: {
          ROS_INFO("Sensor: Type ( Magnetometer )");
          break;
        }
        case SensorBase::SensorType::Distance: {
          ROS_INFO("Sensor: Type ( Distance )");
          break;
        }
        case SensorBase::SensorType::Lidar: {
          ROS_INFO("Sensor: Type ( Lidar )");
          auto lidar_setting = *dynamic_cast<LidarSetting*>(sensor_setting.get());
          set_nans_to_zeros_in_pose(*vehicle_setting, lidar_setting);
          append_static_lidar_tf(curr_vehicle_name, sensor_name,
                                 lidar_setting); // todo is there a more readable way to down-cast?
          vehicle_lidar_map_[sensor_name] = curr_vehicle_name; // non scalable
          lidar_pub_vec_.push_back(
            nh_private_.advertise<sensor_msgs::PointCloud2>(curr_vehicle_name + "/lidar/" + sensor_name, 10));
          break;
        }
        default: {
          throw std::invalid_argument("Unexpected sensor type");
        }
      }
    }
  }

  // add takeoff and land all services if more than 2 drones
  if (car_ros_vec_.size() > 1) {
    ROS_WARN("There shouldn't be multiple vehicles; check the settings.json file");
  }

  // todo add per vehicle reset in AirLib API
  reset_srvr_ = nh_private_.advertiseService("reset", &AirsimROSWrapper::reset_srv_cb, this);

  // todo mimic gazebo's /use_sim_time feature which publishes airsim's clock time..via an rpc call?!
  // clock_pub_ = nh_private_.advertise<rosgraph_msgs::Clock>("clock", 10); 

  // if >0 cameras, add one more thread for img_request_timer_cb
  if (!airsim_img_request_vehicle_name_pair_vec_.empty()) {
    double update_airsim_img_response_every_n_sec;
    nh_private_.getParam("update_airsim_img_response_every_n_sec", update_airsim_img_response_every_n_sec);
    bool separate_spinner = true; // todo debugging race condition
    if (separate_spinner) {
      ros::TimerOptions timer_options(ros::Duration(update_airsim_img_response_every_n_sec),
                                      boost::bind(&AirsimROSWrapper::img_response_timer_cb, this, _1),
                                      &img_timer_cb_queue_);
      airsim_img_response_timer_ = nh_private_.createTimer(timer_options);
      is_used_img_timer_cb_queue_ = true;
    }
    else {
      airsim_img_response_timer_ = nh_private_.createTimer(ros::Duration(update_airsim_img_response_every_n_sec),
                                                           &AirsimROSWrapper::img_response_timer_cb, this);
    }
  }

  if (!lidar_pub_vec_.empty()) {
    double update_lidar_every_n_sec;
    nh_private_.getParam("update_lidar_every_n_sec", update_lidar_every_n_sec);
    nh_private_.setCallbackQueue(&lidar_timer_cb_queue_);
    bool separate_spinner = true; // todo debugging race condition
    if (separate_spinner) {
      ros::TimerOptions timer_options(ros::Duration(update_lidar_every_n_sec),
                                      boost::bind(&AirsimROSWrapper::lidar_timer_cb, this, _1), &lidar_timer_cb_queue_);
      airsim_lidar_update_timer_ = nh_private_.createTimer(timer_options);
      is_used_lidar_timer_cb_queue_ = true;
    }
    else {
      airsim_lidar_update_timer_ = nh_private_.createTimer(ros::Duration(update_lidar_every_n_sec),
                                                           &AirsimROSWrapper::lidar_timer_cb, this);
    }
  }

  initialize_airsim();
}


// todo add reset by vehicle_name API to airlib
// todo not async remove waitonlasttask
bool AirsimROSWrapper::reset_srv_cb(airsim_car_ros_pkgs::Reset::Request& request,
                                    airsim_car_ros_pkgs::Reset::Response& response)
{
  std::lock_guard<std::recursive_mutex> guard(drone_control_mutex_);

  airsim_client_.reset();
  return true; //todo
}

tf2::Quaternion AirsimROSWrapper::get_tf2_quat(const msr::airlib::Quaternionr& airlib_quat) const
{
  return tf2::Quaternion(airlib_quat.x(), airlib_quat.y(), airlib_quat.z(), airlib_quat.w());
}

msr::airlib::Quaternionr AirsimROSWrapper::get_airlib_quat(const geometry_msgs::Quaternion& geometry_msgs_quat) const
{
  return msr::airlib::Quaternionr(geometry_msgs_quat.w, geometry_msgs_quat.x, geometry_msgs_quat.y,
                                  geometry_msgs_quat.z);
}

msr::airlib::Quaternionr AirsimROSWrapper::get_airlib_quat(const tf2::Quaternion& tf2_quat) const
{
  return msr::airlib::Quaternionr(tf2_quat.w(), tf2_quat.x(), tf2_quat.y(), tf2_quat.z());
}

// void AirsimROSWrapper::vel_cmd_body_frame_cb(const airsim_car_ros_pkgs::VelCmd& msg, const std::string& vehicle_name)
void
AirsimROSWrapper::car_cmd_frame_cb(const airsim_car_ros_pkgs::CarCmd::ConstPtr& msg, const std::string& vehicle_name)
{
  SWRI_PROFILE("car_cmd_frame_cb")
  std::lock_guard<std::recursive_mutex> guard(drone_control_mutex_);

  int vehicle_idx = vehicle_name_idx_map_[vehicle_name];
  ROS_DEBUG("Vehicle Name: %s", vehicle_name.c_str());
  car_ros_vec_[vehicle_idx].car_cmd.throttle = msg->throttle;
  car_ros_vec_[vehicle_idx].car_cmd.steering = msg->steering;
  car_ros_vec_[vehicle_idx].car_cmd.brake = msg->brake;
  car_ros_vec_[vehicle_idx].car_cmd.handbrake = msg->handbrake;
  car_ros_vec_[vehicle_idx].car_cmd.is_manual_gear = msg->is_manual_gear;
  car_ros_vec_[vehicle_idx].car_cmd.manual_gear = msg->manual_gear;
  car_ros_vec_[vehicle_idx].car_cmd.gear_immediate = msg->gear_immediate;
  car_ros_vec_[vehicle_idx].has_car_cmd = true;
}

//Code was here to command multiple MultiRotor platorms

nav_msgs::Odometry
AirsimROSWrapper::get_odom_msg_from_airsim_state(const msr::airlib::CarApiBase::CarState& drone_state) const
{
  nav_msgs::Odometry odom_ned_msg;
  odom_ned_msg.header.frame_id = world_frame_id_;

  odom_ned_msg.pose.pose.position.x = drone_state.getPosition().x();
  odom_ned_msg.pose.pose.position.y = drone_state.getPosition().y();
  odom_ned_msg.pose.pose.position.z = drone_state.getPosition().z();
  odom_ned_msg.pose.pose.orientation.x = drone_state.getOrientation().x();
  odom_ned_msg.pose.pose.orientation.y = drone_state.getOrientation().y();
  odom_ned_msg.pose.pose.orientation.z = drone_state.getOrientation().z();
  odom_ned_msg.pose.pose.orientation.w = drone_state.getOrientation().w();

  odom_ned_msg.twist.twist.linear.x = drone_state.kinematics_estimated.twist.linear.x();
  odom_ned_msg.twist.twist.linear.y = drone_state.kinematics_estimated.twist.linear.y();
  odom_ned_msg.twist.twist.linear.z = drone_state.kinematics_estimated.twist.linear.z();
  odom_ned_msg.twist.twist.angular.x = drone_state.kinematics_estimated.twist.angular.x();
  odom_ned_msg.twist.twist.angular.y = drone_state.kinematics_estimated.twist.angular.y();
  odom_ned_msg.twist.twist.angular.z = drone_state.kinematics_estimated.twist.angular.z();

  return odom_ned_msg;
}

// https://docs.ros.org/jade/api/sensor_msgs/html/point__cloud__conversion_8h_source.html#l00066
// look at UnrealLidarSensor.cpp UnrealLidarSensor::getPointCloud() for math
// read this carefully https://docs.ros.org/kinetic/api/sensor_msgs/html/msg/PointCloud2.html
sensor_msgs::PointCloud2 AirsimROSWrapper::get_lidar_msg_from_airsim(const msr::airlib::LidarData& lidar_data) const
{
  sensor_msgs::PointCloud2 lidar_msg;
  lidar_msg.header.frame_id = world_frame_id_; // todo

  if (lidar_data.point_cloud.size() > 3) {
    lidar_msg.height = 1;
    lidar_msg.width = lidar_data.point_cloud.size() / 3;

    lidar_msg.fields.resize(3);
    lidar_msg.fields[0].name = "x";
    lidar_msg.fields[1].name = "y";
    lidar_msg.fields[2].name = "z";
    int offset = 0;

    for (size_t d = 0; d < lidar_msg.fields.size(); ++d, offset += 4) {
      lidar_msg.fields[d].offset = offset;
      lidar_msg.fields[d].datatype = sensor_msgs::PointField::FLOAT32;
      lidar_msg.fields[d].count = 1;
    }

    lidar_msg.is_bigendian = false;
    lidar_msg.point_step = offset; // 4 * num fields
    lidar_msg.row_step = lidar_msg.point_step * lidar_msg.width;

    lidar_msg.is_dense = true; // todo
    std::vector<float> data_std = lidar_data.point_cloud;

    for (int i = 2; i < data_std.size(); i += 3) {
      data_std[i] *= -1.0;
    }

    for (int i = 1; i < data_std.size(); i += 3) {
      data_std[i] *= -1.0;
    }

    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&data_std[0]);
    vector<unsigned char> lidar_msg_data(bytes, bytes + sizeof(float) * data_std.size());
    lidar_msg.data = std::move(lidar_msg_data);
  }
  else {
    // msg = []
  }
  return lidar_msg;
}

// todo covariances
sensor_msgs::Imu AirsimROSWrapper::get_imu_msg_from_airsim(const msr::airlib::ImuBase::Output& imu_data) const
{
  SWRI_PROFILE("get_imu_msg_from_airsim")
  sensor_msgs::Imu imu_msg;

  if (publish_enu_)
  {
    imu_msg.header.frame_id = "/airsim/odom_local_enu";// todo multiple drones

    // Now assign the converted coordinate to the message
    imu_msg.orientation.x = imu_data.orientation.y();
    imu_msg.orientation.y = imu_data.orientation.x();
    imu_msg.orientation.z = -imu_data.orientation.z();
    imu_msg.orientation.w = imu_data.orientation.w();

    // todo radians per second
    imu_msg.angular_velocity.x = math_common::deg2rad(imu_data.angular_velocity.y());
    imu_msg.angular_velocity.y = math_common::deg2rad(imu_data.angular_velocity.x());
    imu_msg.angular_velocity.z = -math_common::deg2rad(imu_data.angular_velocity.z());

    imu_msg.angular_velocity.x = (imu_data.angular_velocity.y()) * imu_calibration_;
    imu_msg.angular_velocity.y = (imu_data.angular_velocity.x()) * imu_calibration_;
    imu_msg.angular_velocity.z = (imu_data.angular_velocity.z()) * imu_calibration_;

    // meters/s2^m 
    imu_msg.linear_acceleration.x = imu_data.linear_acceleration.y();
    imu_msg.linear_acceleration.y = imu_data.linear_acceleration.x();
    imu_msg.linear_acceleration.z = -imu_data.linear_acceleration.z();

    // imu_msg.orientation_covariance = ;
    // imu_msg.angular_velocity_covariance = ;
    // imu_msg.linear_acceleration_covariance = ;
  }
  else
  {
    imu_msg.header.frame_id = "/airsim/odom_local_ned";// todo multiple drones
    imu_msg.orientation.x = imu_data.orientation.x();
    imu_msg.orientation.y = imu_data.orientation.y();
    imu_msg.orientation.z = imu_data.orientation.z();
    imu_msg.orientation.w = imu_data.orientation.w();

    // todo radians per second
    imu_msg.angular_velocity.x = math_common::deg2rad(imu_data.angular_velocity.x());
    imu_msg.angular_velocity.y = math_common::deg2rad(imu_data.angular_velocity.y());
    imu_msg.angular_velocity.z = math_common::deg2rad(imu_data.angular_velocity.z());

    // meters/s2^m 
    imu_msg.linear_acceleration.x = imu_data.linear_acceleration.x();
    imu_msg.linear_acceleration.y = imu_data.linear_acceleration.y();
    imu_msg.linear_acceleration.z = imu_data.linear_acceleration.z();

    // imu_msg.orientation_covariance = ;
    // imu_msg.angular_velocity_covariance = ;
    // imu_msg.linear_acceleration_covariance = ;
  }
  return imu_msg;
}

void AirsimROSWrapper::publish_odom_tf(const nav_msgs::Odometry& odom_ned_msg)
{
  SWRI_PROFILE("publish_odom_tf")
  geometry_msgs::TransformStamped odom_tf;
  odom_tf.header = odom_ned_msg.header;
  odom_tf.child_frame_id = odom_ned_msg.child_frame_id;
  odom_tf.transform.translation.x = odom_ned_msg.pose.pose.position.x;
  odom_tf.transform.translation.y = odom_ned_msg.pose.pose.position.y;
  odom_tf.transform.translation.z = odom_ned_msg.pose.pose.position.z;
  odom_tf.transform.rotation.x = odom_ned_msg.pose.pose.orientation.x;
  odom_tf.transform.rotation.y = odom_ned_msg.pose.pose.orientation.y;
  odom_tf.transform.rotation.z = odom_ned_msg.pose.pose.orientation.z;
  odom_tf.transform.rotation.w = odom_ned_msg.pose.pose.orientation.w;
  tf_broadcaster_.sendTransform(odom_tf);
}

airsim_car_ros_pkgs::GPSYaw
AirsimROSWrapper::get_gps_msg_from_airsim_geo_point(const msr::airlib::GeoPoint& geo_point) const
{
  airsim_car_ros_pkgs::GPSYaw gps_msg;
  gps_msg.latitude = geo_point.latitude;
  gps_msg.longitude = geo_point.longitude;
  gps_msg.altitude = geo_point.altitude;
  return gps_msg;
}

sensor_msgs::NavSatFix
AirsimROSWrapper::get_gps_sensor_msg_from_airsim_geo_point(const msr::airlib::GpsBase::Output& geo_base) const
{
  sensor_msgs::NavSatFix gps_msg;
  gps_msg.latitude = geo_base.gnss.geo_point.latitude;
  gps_msg.longitude = geo_base.gnss.geo_point.longitude;
  gps_msg.altitude = geo_base.gnss.geo_point.altitude;
  gps_msg.status.status = geo_base.gnss.fix_type;
  return gps_msg;
}

void AirsimROSWrapper::drone_state_timer_cb(const ros::TimerEvent& event)
{
  SWRI_PROFILE("drone_state_timer_cb")
  try {
    std::lock_guard<std::recursive_mutex> guard(drone_control_mutex_);

    // todo this is global origin
    origin_geo_point_pub_.publish(origin_geo_point_msg_);

    if (car_ros_vec_.empty()) {
      ROS_ERROR("No vehicles found; check your Airsim settings.json.");
    }

    // iterate over drones
    for (auto& car_ros: car_ros_vec_) {
      // get drone state from airsim
      std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
      ROS_DEBUG("Getting state for %s", car_ros.vehicle_name.c_str());
      car_ros.curr_drone_state = airsim_client_.getCarState(car_ros.vehicle_name);
      msr::airlib::CollisionInfo collision = airsim_client_.simGetCollisionInfo(car_ros.vehicle_name);
      lck.unlock();


      if (car_ros.vehicle_name == "PhysXCar") {

        ros::Time curr_ros_time = ros::Time::now();

        // convert airsim drone state to ROS msgs
        car_ros.curr_odom_ned = get_odom_msg_from_airsim_state(car_ros.curr_drone_state);
        car_ros.curr_odom_ned.header.frame_id = car_ros.vehicle_name;
        car_ros.curr_odom_ned.child_frame_id = car_ros.odom_frame_id;
        car_ros.curr_odom_ned.header.stamp = curr_ros_time;

        if (publish_enu_) {
          std::swap(car_ros.curr_odom_ned.pose.pose.position.x,
                    car_ros.curr_odom_ned.pose.pose.position.y);
          car_ros.curr_odom_ned.pose.pose.position.z *= -1.0;

          std::swap(car_ros.curr_odom_ned.twist.twist.linear.x,
                    car_ros.curr_odom_ned.twist.twist.linear.y);
          car_ros.curr_odom_ned.twist.twist.linear.z *= -1.0;
        }

        // publish to ROS!  
        car_ros.odom_local_ned_pub.publish(car_ros.curr_odom_ned);

        if (publish_vehicle_tf_) {
          publish_odom_tf(car_ros.curr_odom_ned);
        }

        if (collision.has_collided == true) {
          //ROS_ERROR("Collision Detected %d, %d ", collision.has_collided, car_ros.collision_count); 
          car_ros.collision_count++;
          //car_ros.collision_count = collision.collision_count++;
          car_ros.has_collided = true;
          std_msgs::Int32 count;
          count.data = car_ros.collision_count;
          car_ros.collision_pub.publish(count);
        }

        // send control commands from the last callback to airsim
        if (car_ros.has_car_cmd) {
          std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);

          //MJB change this function
          // airsim_client_.moveByVelocityAsync(multirotor_ros.vel_cmd.x, multirotor_ros.vel_cmd.y, multirotor_ros.vel_cmd.z, vel_cmd_duration_, 
          //     msr::airlib::DrivetrainType::MaxDegreeOfFreedom, multirotor_ros.vel_cmd.yaw_mode, multirotor_ros.vehicle_name);

          msr::airlib::CarApiBase::CarControls car_controls;

          ROS_WARN("%s Command Received!!! Throttle cmd = %f", car_ros.vehicle_name.c_str(), car_ros.car_cmd.throttle);
          car_controls.throttle = car_ros.car_cmd.throttle;
          car_controls.steering = car_ros.car_cmd.steering;
          car_controls.brake = car_ros.car_cmd.brake;
          car_controls.handbrake = car_ros.car_cmd.handbrake;
          car_controls.is_manual_gear = car_ros.car_cmd.is_manual_gear;
          car_controls.manual_gear = car_ros.car_cmd.manual_gear;
          car_controls.gear_immediate = car_ros.car_cmd.gear_immediate;
          airsim_client_.setCarControls(car_controls, car_ros.vehicle_name);
          lck.unlock();
        }

        // "clear" control cmds
      }
      car_ros.has_car_cmd = false;
    }

    // IMUS
    if (imu_pub_vec_.size() > 0) {
      int ctr = 0;
      for (const auto& vehicle_imu_pair: vehicle_imu_map_) {
        std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
        auto imu_data = airsim_client_.getImuData(vehicle_imu_pair.second, vehicle_imu_pair.first);
        lck.unlock();
        sensor_msgs::Imu imu_msg = get_imu_msg_from_airsim(imu_data);
        imu_msg.header.frame_id = vehicle_imu_pair.first;
        imu_msg.header.stamp = ros::Time::now();

        if (std::abs(imu_msg.angular_velocity.z) > 2.5) {
          ROS_WARN("Throwing away bad IMU message.");
          ctr++;
          continue;
        }
        imu_pub_vec_[ctr].publish(imu_msg);
        ctr++;
      }
    }

    // GPS
    if (gps_pub_vec_.size() > 0) {
      int ctr = 0;
      for (const auto& vehicle_gps_pair: vehicle_gps_map_) {
        std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
        auto gps_data = airsim_client_.getGpsData(vehicle_gps_pair.second, vehicle_gps_pair.first);
        lck.unlock();
        sensor_msgs::NavSatFix gps_msg = get_gps_sensor_msg_from_airsim_geo_point(gps_data);
        gps_msg.header.frame_id = vehicle_gps_pair.first;
        gps_msg.header.stamp = ros::Time::now();
        gps_pub_vec_[ctr].publish(gps_msg);
        ctr++;
      }
    }

    if (static_tf_msg_vec_.size() > 0) {
      for (auto& static_tf_msg : static_tf_msg_vec_) {
        static_tf_msg.header.stamp = ros::Time::now();
        static_tf_pub_.sendTransform(static_tf_msg);
      }
      static_tf_msg_vec_.clear();
    }

    // todo add and expose a gimbal angular velocity to airlib
    if (has_gimbal_cmd_) {
      std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
      airsim_client_.simSetCameraOrientation(gimbal_cmd_.camera_name, gimbal_cmd_.target_quat,
                                             gimbal_cmd_.vehicle_name);
      lck.unlock();
    }

    has_gimbal_cmd_ = false;
  }
 catch (rpc::rpc_error& e) {
    std::string msg = e.get_error().as<std::string>();
    ROS_ERROR("Exception raised by the API: %s", msg.c_str());
  }
  catch (rpc::timeout& e) {
    ROS_ERROR("RPC timeout: %s", e.what());
  }
}

// airsim uses nans for zeros in settings.json. we set them to zeros here for handling tfs in ROS 
void AirsimROSWrapper::set_nans_to_zeros_in_pose(VehicleSetting& vehicle_setting) const
{
  if (std::isnan(vehicle_setting.position.x())) {
    vehicle_setting.position.x() = 0.0;
  }

  if (std::isnan(vehicle_setting.position.y())) {
    vehicle_setting.position.y() = 0.0;
  }

  if (std::isnan(vehicle_setting.position.z())) {
    vehicle_setting.position.z() = 0.0;
  }

  if (std::isnan(vehicle_setting.rotation.yaw)) {
    vehicle_setting.rotation.yaw = 0.0;
  }

  if (std::isnan(vehicle_setting.rotation.pitch)) {
    vehicle_setting.rotation.pitch = 0.0;
  }

  if (std::isnan(vehicle_setting.rotation.roll)) {
    vehicle_setting.rotation.roll = 0.0;
  }
}

// if any nan's in camera pose, set them to match vehicle pose (which has already converted any potential nans to zeros)
void
AirsimROSWrapper::set_nans_to_zeros_in_pose(const VehicleSetting& vehicle_setting, CameraSetting& camera_setting) const
{
  if (std::isnan(camera_setting.position.x())) {
    camera_setting.position.x() = vehicle_setting.position.x();
  }

  if (std::isnan(camera_setting.position.y())) {
    camera_setting.position.y() = vehicle_setting.position.y();
  }

  if (std::isnan(camera_setting.position.z())) {
    camera_setting.position.z() = vehicle_setting.position.z();
  }

  if (std::isnan(camera_setting.rotation.yaw)) {
    camera_setting.rotation.yaw = vehicle_setting.rotation.yaw;
  }

  if (std::isnan(camera_setting.rotation.pitch)) {
    camera_setting.rotation.pitch = vehicle_setting.rotation.pitch;
  }

  if (std::isnan(camera_setting.rotation.roll)) {
    camera_setting.rotation.roll = vehicle_setting.rotation.roll;
  }

}

void
AirsimROSWrapper::set_nans_to_zeros_in_pose(const VehicleSetting& vehicle_setting, LidarSetting& lidar_setting) const
{
  if (std::isnan(lidar_setting.position.x())) {
    lidar_setting.position.x() = vehicle_setting.position.x();
  }

  if (std::isnan(lidar_setting.position.y())) {
    lidar_setting.position.y() = vehicle_setting.position.y();
  }

  if (std::isnan(lidar_setting.position.z())) {
    lidar_setting.position.z() = vehicle_setting.position.z();
  }

  if (std::isnan(lidar_setting.rotation.yaw)) {
    lidar_setting.rotation.yaw = vehicle_setting.rotation.yaw;
  }

  if (std::isnan(lidar_setting.rotation.pitch)) {
    lidar_setting.rotation.pitch = vehicle_setting.rotation.pitch;
  }

  if (std::isnan(lidar_setting.rotation.roll)) {
    lidar_setting.rotation.roll = vehicle_setting.rotation.roll;
  }
}

void AirsimROSWrapper::append_static_vehicle_tf(const std::string& vehicle_name, const VehicleSetting& vehicle_setting)
{
  geometry_msgs::TransformStamped vehicle_tf_msg;
  vehicle_tf_msg.header.frame_id = world_frame_id_;
  vehicle_tf_msg.header.stamp = ros::Time::now();
  if (publish_enu_)
  {
    vehicle_tf_msg.child_frame_id = vehicle_name + "/odom_local_enu";
  }
  else
  {
    vehicle_tf_msg.child_frame_id = vehicle_name + "/odom_local_ned";
  }
  tf2::Quaternion quat;
  quat.setRPY(vehicle_setting.rotation.roll, vehicle_setting.rotation.pitch, vehicle_setting.rotation.yaw);

  if (publish_enu_)
  {
    vehicle_tf_msg.transform.translation.x = vehicle_setting.position.y();
    vehicle_tf_msg.transform.translation.y = vehicle_setting.position.x();
    vehicle_tf_msg.transform.translation.z = -vehicle_setting.position.z();

    vehicle_tf_msg.transform.rotation.x = quat.y();
    vehicle_tf_msg.transform.rotation.y = quat.x();
    vehicle_tf_msg.transform.rotation.z = -quat.z();
    vehicle_tf_msg.transform.rotation.w = quat.w();
  }
  else
  {
    vehicle_tf_msg.transform.translation.x = vehicle_setting.position.x();
    vehicle_tf_msg.transform.translation.y = vehicle_setting.position.y();
    vehicle_tf_msg.transform.translation.z = vehicle_setting.position.z();

    vehicle_tf_msg.transform.rotation.x = quat.x();
    vehicle_tf_msg.transform.rotation.y = quat.y();
    vehicle_tf_msg.transform.rotation.z = quat.z();
    vehicle_tf_msg.transform.rotation.w = quat.w();
  }

  static_tf_msg_vec_.push_back(vehicle_tf_msg);
}

void AirsimROSWrapper::append_static_lidar_tf(const std::string& vehicle_name, const std::string& lidar_name,
                                              const LidarSetting& lidar_setting)
{

  geometry_msgs::TransformStamped lidar_tf_msg;
  if (publish_enu_)
  {
    lidar_tf_msg.header.frame_id = vehicle_name + "/odom_local_enu"; // todo multiple drones
  }
  else
  {
    lidar_tf_msg.header.frame_id = vehicle_name + "/odom_local_ned"; // todo multiple drones
  }
  
  lidar_tf_msg.child_frame_id = lidar_name;
  tf2::Quaternion quat;
  quat.setRPY(lidar_setting.rotation.roll, lidar_setting.rotation.pitch, lidar_setting.rotation.yaw);

  // Coordinate is now in NED format
  // Data from RPC lib and settings file is in NED coordinates, so switch to ENU system
  if (publish_enu_)
  {
    lidar_tf_msg.transform.translation.x = lidar_setting.position.y();
    lidar_tf_msg.transform.translation.y = lidar_setting.position.x();
    lidar_tf_msg.transform.translation.z = -lidar_setting.position.z();

    lidar_tf_msg.transform.rotation.x = quat.y();
    lidar_tf_msg.transform.rotation.y = quat.x();
    lidar_tf_msg.transform.rotation.z = -quat.z();
    lidar_tf_msg.transform.rotation.w = quat.w();
  }
  else
  {
    lidar_tf_msg.transform.translation.x = lidar_setting.position.x();
    lidar_tf_msg.transform.translation.y = lidar_setting.position.y();
    lidar_tf_msg.transform.translation.z = lidar_setting.position.z();

    lidar_tf_msg.transform.rotation.x = quat.x();
    lidar_tf_msg.transform.rotation.y = quat.y();
    lidar_tf_msg.transform.rotation.z = quat.z();
    lidar_tf_msg.transform.rotation.w = quat.w();
  }

  static_tf_msg_vec_.push_back(lidar_tf_msg);
}

void AirsimROSWrapper::append_static_camera_tf(const std::string& vehicle_name, const std::string& camera_name,
                                               const CameraSetting& camera_setting)
{
  geometry_msgs::TransformStamped static_cam_tf_body_msg;
  if (publish_enu_)
  {
    static_cam_tf_body_msg.header.frame_id = vehicle_name + "/odom_local_enu";
  }
  else
  {
    static_cam_tf_body_msg.header.frame_id = vehicle_name + "/odom_local_ned";
  }
  static_cam_tf_body_msg.child_frame_id = camera_name + "_body/static";
  
  // Camera API returns data in NED format, this switches to camera coordinate system
  static_cam_tf_body_msg.transform.translation.x = camera_setting.position.x();
  static_cam_tf_body_msg.transform.translation.y = camera_setting.position.y();
  static_cam_tf_body_msg.transform.translation.z = camera_setting.position.z();
  tf2::Quaternion quat;
  quat.setRPY(camera_setting.rotation.roll, camera_setting.rotation.pitch, camera_setting.rotation.yaw);
  static_cam_tf_body_msg.transform.rotation.x = quat.x();
  static_cam_tf_body_msg.transform.rotation.y = quat.y();
  static_cam_tf_body_msg.transform.rotation.z = quat.z();
  static_cam_tf_body_msg.transform.rotation.w = quat.w();

  geometry_msgs::TransformStamped static_cam_tf_optical_msg = static_cam_tf_body_msg;
  static_cam_tf_optical_msg.child_frame_id = camera_name + "_optical/static";

  tf2::Quaternion quat_cam_body;
  tf2::Quaternion quat_cam_optical;
  tf2::convert(static_cam_tf_body_msg.transform.rotation, quat_cam_body);
  tf2::Matrix3x3 mat_cam_body(quat_cam_body);
  tf2::Matrix3x3 mat_cam_optical;
  mat_cam_optical.setValue(mat_cam_body.getColumn(1).getX(), mat_cam_body.getColumn(2).getX(), mat_cam_body.getColumn(0).getX(),
                           mat_cam_body.getColumn(1).getY(), mat_cam_body.getColumn(2).getY(), mat_cam_body.getColumn(0).getY(),
                           mat_cam_body.getColumn(1).getZ(), mat_cam_body.getColumn(2).getZ(), mat_cam_body.getColumn(0).getZ());
  mat_cam_optical.getRotation(quat_cam_optical);
  quat_cam_optical.normalize();
  tf2::convert(quat_cam_optical, static_cam_tf_optical_msg.transform.rotation);

  if (publish_enu_)
  {
    std::swap(static_cam_tf_body_msg.transform.translation.x,
      static_cam_tf_body_msg.transform.translation.y);
    static_cam_tf_body_msg.transform.translation.z = -static_cam_tf_body_msg.transform.translation.z;

    std::swap(static_cam_tf_body_msg.transform.rotation.x,
      static_cam_tf_body_msg.transform.rotation.y);
    static_cam_tf_body_msg.transform.rotation.z = -static_cam_tf_body_msg.transform.rotation.z;

    std::swap(static_cam_tf_optical_msg.transform.translation.x,
      static_cam_tf_optical_msg.transform.translation.y);
    static_cam_tf_optical_msg.transform.translation.z = -static_cam_tf_optical_msg.transform.translation.z;

    std::swap(static_cam_tf_optical_msg.transform.rotation.x,
      static_cam_tf_optical_msg.transform.rotation.y);
    static_cam_tf_optical_msg.transform.rotation.z = -static_cam_tf_optical_msg.transform.rotation.z;
  }

  static_tf_msg_vec_.push_back(static_cam_tf_body_msg);
  static_tf_msg_vec_.push_back(static_cam_tf_optical_msg);
}

void AirsimROSWrapper::img_response_timer_cb(const ros::TimerEvent& event)
{
  SWRI_PROFILE("img_response_timer_cb")
  try {
    int image_response_idx = 0;
    for (const auto& request_name_pair : airsim_img_request_vehicle_name_pair_vec_) {
      std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
      // This is pretty ugly, but I want to be able to time how long this RPC call takes
      // all by itself.  Pulling this out into a lambda can be removed if you don't
      // care about the profiling.
      auto getImagesFn = [this, request_name_pair]() -> std::vector<ImageResponse>
      {
        SWRI_PROFILE("simGetImages")
        return airsim_client_images_.simGetImages(request_name_pair.first,
                                                  request_name_pair.second);
      };
      const std::vector<ImageResponse>& img_response = getImagesFn();
      lck.unlock();

      if (img_response.empty()) {
        ROS_WARN_THROTTLE(2.0, "simGetImages was empty");
      }
      else {
        ROS_DEBUG("simGetImages response size: %lu", img_response.size());
      }
      if (img_response.size() == request_name_pair.first.size()) {
        process_and_publish_img_response(img_response, image_response_idx, request_name_pair.second);
        image_response_idx += img_response.size();
      }
    }
  }
  catch (rpc::rpc_error& e) {
    std::string msg = e.get_error().as<std::string>();
    ROS_ERROR("Exception raised by the RPC API: %s", msg.c_str());
  }
  catch (rpc::timeout& e) {
    ROS_ERROR("RPC timeout: %s", e.what());
  }
}

void AirsimROSWrapper::lidar_timer_cb(const ros::TimerEvent& event)
{
  SWRI_PROFILE("lidar_timer_cb")
  try {
    // std::lock_guard<std::recursive_mutex> guard(drone_control_mutex_);
    if (!lidar_pub_vec_.empty()) {
      // std::lock_guard<std::recursive_mutex> guard(lidar_mutex_);
      int ctr = 0;
      for (const auto& vehicle_lidar_pair: vehicle_lidar_map_) {
        std::unique_lock<std::recursive_mutex> lck(drone_control_mutex_);
        auto lidar_data = airsim_client_lidar_.getLidarData(vehicle_lidar_pair.first,
                                                            vehicle_lidar_pair.second); // airsim api is imu_name, vehicle_name
        lck.unlock();
        sensor_msgs::PointCloud2 lidar_msg = get_lidar_msg_from_airsim(
          lidar_data); // todo make const ptr msg to avoid copy
        lidar_msg.header.frame_id = vehicle_lidar_pair.first; // sensor frame name. todo add to doc
        lidar_msg.header.stamp = ros::Time::now();
        lidar_pub_vec_[ctr].publish(lidar_msg);
        ctr++;
      }
    }

  }
  catch (rpc::rpc_error& e) {
    std::string msg = e.get_error().as<std::string>();
    ROS_ERROR("Exception raised by the RPC API: %s", msg.c_str());
  }
  catch (rpc::timeout& e) {
    ROS_ERROR("RPC timeout: %s", e.what());
  }
}

sensor_msgs::ImagePtr AirsimROSWrapper::manual_decode_depth(const ImageResponse& img_response) const
{
  SWRI_PROFILE("manual_decode_depth")
  // UE's FloatRGBA is 64-bit, we need to make it 32-bit
  auto* bytes = img_response.image_data_uint8.get();
  cv::Mat depth_img(img_response.height, img_response.width, CV_64FC1, reinterpret_cast<double*>(bytes->data()));
  cv::Mat img32f(img_response.height, img_response.width, CV_32FC1);
  depth_img.convertTo(img32f, CV_32FC1);
  return cv_bridge::CvImage(std_msgs::Header(), "32FC1", img32f).toImageMsg();
}

sensor_msgs::ImagePtr AirsimROSWrapper::get_img_msg_from_response(const ImageResponse& img_response,
                                                                  const ros::Time curr_ros_time,
                                                                  const std::string frame_id) const
{
  SWRI_PROFILE("get_img_msg_from_response")
  ROS_DEBUG("Getting image for %s", img_response.camera_name.c_str());
  sensor_msgs::ImagePtr img_msg_ptr;
  if (img_response.pixels_as_float) {
    img_msg_ptr = manual_decode_depth(img_response);
  }
  else {
    img_msg_ptr = boost::make_shared<sensor_msgs::Image>();
    auto* data = img_response.image_data_uint8.get();
    img_msg_ptr->data.assign(data->begin(), data->end());
    img_msg_ptr->encoding = sensor_msgs::image_encodings::BGRA8;
  }
  img_msg_ptr->header.stamp = curr_ros_time;
  img_msg_ptr->header.frame_id = frame_id;
  img_msg_ptr->height = img_response.height;
  img_msg_ptr->width = img_response.width;
  img_msg_ptr->step = img_response.width * 4; //img_msg_ptr->data.size() / img_response.height;
  img_msg_ptr->is_bigendian = 0;
  return img_msg_ptr;
}

// todo have a special stereo pair mode and get projection matrix by calculating offset wrt drone body frame?
sensor_msgs::CameraInfo AirsimROSWrapper::generate_cam_info(const std::string& camera_name,
                                                            const CameraSetting& camera_setting,
                                                            const CaptureSetting& capture_setting) const
{
  sensor_msgs::CameraInfo cam_info_msg;
  cam_info_msg.header.frame_id =
    camera_name + "/" + image_type_int_to_string_map_.at(capture_setting.image_type) + "_optical/static";
  cam_info_msg.height = capture_setting.height;
  cam_info_msg.width = capture_setting.width;
  float f_x = (capture_setting.width / 2.0) / tan(math_common::deg2rad(capture_setting.fov_degrees / 2.0));
  // todo focal length in Y direction should be same as X it seems. this can change in future a scene capture component which exactly correponds to a cine camera
  // float f_y = (capture_setting.height / 2.0) / tan(math_common::deg2rad(fov_degrees / 2.0));
  cam_info_msg.K = {
    f_x, 0.0, capture_setting.width / 2.0, 0.0, f_x, capture_setting.height / 2.0, 0.0, 0.0, 1.0};
  cam_info_msg.P = {
    f_x, 0.0, capture_setting.width / 2.0, 0.0, 0.0, f_x, capture_setting.height / 2.0, 0.0, 0.0, 0.0, 1.0, 0.0};
  return cam_info_msg;
}

void AirsimROSWrapper::process_and_publish_img_response(const std::vector<ImageResponse>& img_response_vec,
                                                        const int img_response_idx, const std::string& vehicle_name)
{
  SWRI_PROFILE("process_and_publish_img_response")
  ROS_DEBUG("process_and_publish_img_response %d", img_response_idx);
  // todo add option to use airsim time (image_response.TTimePoint) like Gazebo /use_sim_time param
  ros::Time curr_ros_time = ros::Time::now();
  int img_response_idx_internal = img_response_idx;

  for (const auto& curr_img_response : img_response_vec) {
    // todo publishing a tf for each capture type seems stupid. but it foolproofs us against render thread's async stuff, I hope. 
    // Ideally, we should loop over cameras and then captures, and publish only one tf.  
    if (publish_enu_)
    {
      publish_camera_tf(curr_img_response, curr_ros_time, vehicle_name + "/odom_local_enu", curr_img_response.camera_name);
    }
    else
    {
      publish_camera_tf(curr_img_response, curr_ros_time, vehicle_name + "/odom_local_ned", curr_img_response.camera_name);
    }

    // todo simGetCameraInfo is wrong + also it's only for image type -1.  
    // msr::airlib::CameraInfo camera_info = airsim_client_.simGetCameraInfo(curr_img_response.camera_name);

    // update timestamp of saved cam info msgs
    camera_info_msg_vec_[img_response_idx_internal].header.stamp = curr_ros_time;
    cam_info_pub_vec_[img_response_idx_internal].publish(camera_info_msg_vec_[img_response_idx_internal]);

    // DepthPlanner / DepthPerspective / DepthVis / DisparityNormalized
    /*if (curr_img_response.pixels_as_float) {
      image_pub_vec_[img_response_idx_internal].publish(get_depth_img_msg_from_response(curr_img_response,
                                                                                        curr_ros_time,
                                                                                        curr_img_response.camera_name +
                                                                                        "_optical"));
    }
      // Scene / Segmentation / SurfaceNormals / Infrared
    else { */
    ROS_DEBUG("Publishing to %s", image_pub_vec_[img_response_idx_internal].getTopic().c_str());
      image_pub_vec_[img_response_idx_internal].publish(get_img_msg_from_response(curr_img_response,
                                                                                  curr_ros_time,
                                                                                  curr_img_response.camera_name +
                                                                                  "_optical"));
    //}
    img_response_idx_internal++;
  }

}

// publish camera transforms
// camera poses are obtained from airsim's client API which are in (local) NED frame. 
// We first do a change of basis to camera optical frame (Z forward, X right, Y down) 
void AirsimROSWrapper::publish_camera_tf(const ImageResponse& img_response, const ros::Time& ros_time,
                                         const std::string& frame_id, const std::string& child_frame_id)
{
  SWRI_PROFILE("publish_camera_tf")
  geometry_msgs::TransformStamped cam_tf_body_msg;
  cam_tf_body_msg.header.stamp = ros_time;
  cam_tf_body_msg.header.frame_id = child_frame_id + "_body/static";
  cam_tf_body_msg.child_frame_id = child_frame_id + "_body";
  cam_tf_body_msg.transform.translation.x = img_response.camera_position.x();
  cam_tf_body_msg.transform.translation.y = img_response.camera_position.y();
  cam_tf_body_msg.transform.translation.z = img_response.camera_position.z();
  cam_tf_body_msg.transform.rotation.x = img_response.camera_orientation.x();
  cam_tf_body_msg.transform.rotation.y = img_response.camera_orientation.y();
  cam_tf_body_msg.transform.rotation.z = img_response.camera_orientation.z();
  cam_tf_body_msg.transform.rotation.w = img_response.camera_orientation.w();

  geometry_msgs::TransformStamped cam_tf_optical_msg;
  cam_tf_optical_msg.header.stamp = ros_time;
  cam_tf_optical_msg.header.frame_id = child_frame_id + "_optical/static";
  cam_tf_optical_msg.child_frame_id = child_frame_id + "_optical";
  cam_tf_optical_msg.transform.translation.x = cam_tf_body_msg.transform.translation.x;
  cam_tf_optical_msg.transform.translation.y = cam_tf_body_msg.transform.translation.y;
  cam_tf_optical_msg.transform.translation.z = cam_tf_body_msg.transform.translation.z;

  tf2::Quaternion quat_cam_body;
  tf2::Quaternion quat_cam_optical;
  tf2::convert(cam_tf_body_msg.transform.rotation, quat_cam_body);
  tf2::Matrix3x3 mat_cam_body(quat_cam_body); 
  // tf2::Matrix3x3 mat_cam_optical = matrix_cam_body_to_optical_ * mat_cam_body * matrix_cam_body_to_optical_inverse_;
  // tf2::Matrix3x3 mat_cam_optical = matrix_cam_body_to_optical_ * mat_cam_body;
  tf2::Matrix3x3 mat_cam_optical;
  mat_cam_optical.setValue(mat_cam_body.getColumn(1).getX(), mat_cam_body.getColumn(2).getX(), mat_cam_body.getColumn(0).getX(), 
                           mat_cam_body.getColumn(1).getY(), mat_cam_body.getColumn(2).getY(), mat_cam_body.getColumn(0).getY(),
                           mat_cam_body.getColumn(1).getZ(), mat_cam_body.getColumn(2).getZ(), mat_cam_body.getColumn(0).getZ()); 
  mat_cam_optical.getRotation(quat_cam_optical);
  quat_cam_optical.normalize();
  tf2::convert(quat_cam_optical, cam_tf_optical_msg.transform.rotation);

  if (publish_enu_)
  {
    std::swap(cam_tf_body_msg.transform.translation.x,
      cam_tf_body_msg.transform.translation.y);
    cam_tf_body_msg.transform.translation.z = -cam_tf_body_msg.transform.translation.z;

    std::swap(cam_tf_body_msg.transform.rotation.x,
      cam_tf_body_msg.transform.rotation.y);
    cam_tf_body_msg.transform.rotation.z = -cam_tf_body_msg.transform.rotation.z;

    std::swap(cam_tf_optical_msg.transform.translation.x,
      cam_tf_optical_msg.transform.translation.y);
    cam_tf_optical_msg.transform.translation.z = -cam_tf_optical_msg.transform.translation.z;

    std::swap(cam_tf_optical_msg.transform.rotation.x,
      cam_tf_optical_msg.transform.rotation.y);
    cam_tf_optical_msg.transform.rotation.z = -cam_tf_optical_msg.transform.rotation.z;
  }
  tf_broadcaster_.sendTransform(cam_tf_body_msg);
  tf_broadcaster_.sendTransform(cam_tf_optical_msg);
}

void AirsimROSWrapper::convert_yaml_to_simple_mat(const YAML::Node& node, SimpleMatrix& m) const
{
  int rows, cols;
  rows = node["rows"].as<int>();
  cols = node["cols"].as<int>();
  const YAML::Node& data = node["data"];
  for (int i = 0; i < rows * cols; ++i) {
    m.data[i] = data[i].as<double>();
  }
}

void AirsimROSWrapper::read_params_from_yaml_and_fill_cam_info_msg(const std::string& file_name,
                                                                   sensor_msgs::CameraInfo& cam_info) const
{
  std::ifstream fin(file_name.c_str());
  YAML::Node doc = YAML::Load(fin);

  cam_info.width = doc[WIDTH_YML_NAME].as<int>();
  cam_info.height = doc[HEIGHT_YML_NAME].as<int>();

  SimpleMatrix K_(3, 3, &cam_info.K[0]);
  convert_yaml_to_simple_mat(doc[K_YML_NAME], K_);
  SimpleMatrix R_(3, 3, &cam_info.R[0]);
  convert_yaml_to_simple_mat(doc[R_YML_NAME], R_);
  SimpleMatrix P_(3, 4, &cam_info.P[0]);
  convert_yaml_to_simple_mat(doc[P_YML_NAME], P_);

  cam_info.distortion_model = doc[DMODEL_YML_NAME].as<std::string>();

  const YAML::Node& D_node = doc[D_YML_NAME];
  int D_rows, D_cols;
  D_rows = D_node["rows"].as<int>();
  D_cols = D_node["cols"].as<int>();
  const YAML::Node& D_data = D_node["data"];
  cam_info.D.resize(D_rows * D_cols);
  for (int i = 0; i < D_rows * D_cols; ++i) {
    cam_info.D[i] = D_data[i].as<float>();
  }
}
