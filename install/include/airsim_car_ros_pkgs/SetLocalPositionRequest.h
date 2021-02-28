// Generated by gencpp from file airsim_car_ros_pkgs/SetLocalPositionRequest.msg
// DO NOT EDIT!


#ifndef AIRSIM_CAR_ROS_PKGS_MESSAGE_SETLOCALPOSITIONREQUEST_H
#define AIRSIM_CAR_ROS_PKGS_MESSAGE_SETLOCALPOSITIONREQUEST_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace airsim_car_ros_pkgs
{
template <class ContainerAllocator>
struct SetLocalPositionRequest_
{
  typedef SetLocalPositionRequest_<ContainerAllocator> Type;

  SetLocalPositionRequest_()
    : x(0.0)
    , y(0.0)
    , z(0.0)
    , yaw(0.0)
    , vehicle_name()  {
    }
  SetLocalPositionRequest_(const ContainerAllocator& _alloc)
    : x(0.0)
    , y(0.0)
    , z(0.0)
    , yaw(0.0)
    , vehicle_name(_alloc)  {
  (void)_alloc;
    }



   typedef double _x_type;
  _x_type x;

   typedef double _y_type;
  _y_type y;

   typedef double _z_type;
  _z_type z;

   typedef double _yaw_type;
  _yaw_type yaw;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _vehicle_name_type;
  _vehicle_name_type vehicle_name;





  typedef boost::shared_ptr< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> const> ConstPtr;

}; // struct SetLocalPositionRequest_

typedef ::airsim_car_ros_pkgs::SetLocalPositionRequest_<std::allocator<void> > SetLocalPositionRequest;

typedef boost::shared_ptr< ::airsim_car_ros_pkgs::SetLocalPositionRequest > SetLocalPositionRequestPtr;
typedef boost::shared_ptr< ::airsim_car_ros_pkgs::SetLocalPositionRequest const> SetLocalPositionRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator1> & lhs, const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator2> & rhs)
{
  return lhs.x == rhs.x &&
    lhs.y == rhs.y &&
    lhs.z == rhs.z &&
    lhs.yaw == rhs.yaw &&
    lhs.vehicle_name == rhs.vehicle_name;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator1> & lhs, const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace airsim_car_ros_pkgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "4733a77595a94b3eecdaa7e677d5bf86";
  }

  static const char* value(const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x4733a77595a94b3eULL;
  static const uint64_t static_value2 = 0xecdaa7e677d5bf86ULL;
};

template<class ContainerAllocator>
struct DataType< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "airsim_car_ros_pkgs/SetLocalPositionRequest";
  }

  static const char* value(const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "#Request : expects position setpoint via x, y, z\n"
"#Request : expects yaw setpoint via yaw (send yaw_valid=true)\n"
"float64 x\n"
"float64 y\n"
"float64 z\n"
"float64 yaw\n"
"string vehicle_name\n"
;
  }

  static const char* value(const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.x);
      stream.next(m.y);
      stream.next(m.z);
      stream.next(m.yaw);
      stream.next(m.vehicle_name);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct SetLocalPositionRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::airsim_car_ros_pkgs::SetLocalPositionRequest_<ContainerAllocator>& v)
  {
    s << indent << "x: ";
    Printer<double>::stream(s, indent + "  ", v.x);
    s << indent << "y: ";
    Printer<double>::stream(s, indent + "  ", v.y);
    s << indent << "z: ";
    Printer<double>::stream(s, indent + "  ", v.z);
    s << indent << "yaw: ";
    Printer<double>::stream(s, indent + "  ", v.yaw);
    s << indent << "vehicle_name: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.vehicle_name);
  }
};

} // namespace message_operations
} // namespace ros

#endif // AIRSIM_CAR_ROS_PKGS_MESSAGE_SETLOCALPOSITIONREQUEST_H
