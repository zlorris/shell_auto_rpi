// Generated by gencpp from file airsim_car_ros_pkgs/SetGPSPosition.msg
// DO NOT EDIT!


#ifndef AIRSIM_CAR_ROS_PKGS_MESSAGE_SETGPSPOSITION_H
#define AIRSIM_CAR_ROS_PKGS_MESSAGE_SETGPSPOSITION_H

#include <ros/service_traits.h>


#include <airsim_car_ros_pkgs/SetGPSPositionRequest.h>
#include <airsim_car_ros_pkgs/SetGPSPositionResponse.h>


namespace airsim_car_ros_pkgs
{

struct SetGPSPosition
{

typedef SetGPSPositionRequest Request;
typedef SetGPSPositionResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct SetGPSPosition
} // namespace airsim_car_ros_pkgs


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::airsim_car_ros_pkgs::SetGPSPosition > {
  static const char* value()
  {
    return "419da4302dcfe39986b1a614149a1599";
  }

  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPosition&) { return value(); }
};

template<>
struct DataType< ::airsim_car_ros_pkgs::SetGPSPosition > {
  static const char* value()
  {
    return "airsim_car_ros_pkgs/SetGPSPosition";
  }

  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPosition&) { return value(); }
};


// service_traits::MD5Sum< ::airsim_car_ros_pkgs::SetGPSPositionRequest> should match
// service_traits::MD5Sum< ::airsim_car_ros_pkgs::SetGPSPosition >
template<>
struct MD5Sum< ::airsim_car_ros_pkgs::SetGPSPositionRequest>
{
  static const char* value()
  {
    return MD5Sum< ::airsim_car_ros_pkgs::SetGPSPosition >::value();
  }
  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPositionRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::airsim_car_ros_pkgs::SetGPSPositionRequest> should match
// service_traits::DataType< ::airsim_car_ros_pkgs::SetGPSPosition >
template<>
struct DataType< ::airsim_car_ros_pkgs::SetGPSPositionRequest>
{
  static const char* value()
  {
    return DataType< ::airsim_car_ros_pkgs::SetGPSPosition >::value();
  }
  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPositionRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::airsim_car_ros_pkgs::SetGPSPositionResponse> should match
// service_traits::MD5Sum< ::airsim_car_ros_pkgs::SetGPSPosition >
template<>
struct MD5Sum< ::airsim_car_ros_pkgs::SetGPSPositionResponse>
{
  static const char* value()
  {
    return MD5Sum< ::airsim_car_ros_pkgs::SetGPSPosition >::value();
  }
  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPositionResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::airsim_car_ros_pkgs::SetGPSPositionResponse> should match
// service_traits::DataType< ::airsim_car_ros_pkgs::SetGPSPosition >
template<>
struct DataType< ::airsim_car_ros_pkgs::SetGPSPositionResponse>
{
  static const char* value()
  {
    return DataType< ::airsim_car_ros_pkgs::SetGPSPosition >::value();
  }
  static const char* value(const ::airsim_car_ros_pkgs::SetGPSPositionResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // AIRSIM_CAR_ROS_PKGS_MESSAGE_SETGPSPOSITION_H
