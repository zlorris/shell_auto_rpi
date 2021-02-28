// Generated by gencpp from file airsim_car_ros_pkgs/LandGroupRequest.msg
// DO NOT EDIT!


#ifndef AIRSIM_CAR_ROS_PKGS_MESSAGE_LANDGROUPREQUEST_H
#define AIRSIM_CAR_ROS_PKGS_MESSAGE_LANDGROUPREQUEST_H


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
struct LandGroupRequest_
{
  typedef LandGroupRequest_<ContainerAllocator> Type;

  LandGroupRequest_()
    : vehicle_names()
    , waitOnLastTask(false)  {
    }
  LandGroupRequest_(const ContainerAllocator& _alloc)
    : vehicle_names(_alloc)
    , waitOnLastTask(false)  {
  (void)_alloc;
    }



   typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _vehicle_names_type;
  _vehicle_names_type vehicle_names;

   typedef uint8_t _waitOnLastTask_type;
  _waitOnLastTask_type waitOnLastTask;





  typedef boost::shared_ptr< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> const> ConstPtr;

}; // struct LandGroupRequest_

typedef ::airsim_car_ros_pkgs::LandGroupRequest_<std::allocator<void> > LandGroupRequest;

typedef boost::shared_ptr< ::airsim_car_ros_pkgs::LandGroupRequest > LandGroupRequestPtr;
typedef boost::shared_ptr< ::airsim_car_ros_pkgs::LandGroupRequest const> LandGroupRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator1> & lhs, const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator2> & rhs)
{
  return lhs.vehicle_names == rhs.vehicle_names &&
    lhs.waitOnLastTask == rhs.waitOnLastTask;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator1> & lhs, const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace airsim_car_ros_pkgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "93573f0951a404e62756fb4ba226140a";
  }

  static const char* value(const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x93573f0951a404e6ULL;
  static const uint64_t static_value2 = 0x2756fb4ba226140aULL;
};

template<class ContainerAllocator>
struct DataType< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "airsim_car_ros_pkgs/LandGroupRequest";
  }

  static const char* value(const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string[] vehicle_names\n"
"bool waitOnLastTask \n"
;
  }

  static const char* value(const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.vehicle_names);
      stream.next(m.waitOnLastTask);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct LandGroupRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::airsim_car_ros_pkgs::LandGroupRequest_<ContainerAllocator>& v)
  {
    s << indent << "vehicle_names[]" << std::endl;
    for (size_t i = 0; i < v.vehicle_names.size(); ++i)
    {
      s << indent << "  vehicle_names[" << i << "]: ";
      Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.vehicle_names[i]);
    }
    s << indent << "waitOnLastTask: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.waitOnLastTask);
  }
};

} // namespace message_operations
} // namespace ros

#endif // AIRSIM_CAR_ROS_PKGS_MESSAGE_LANDGROUPREQUEST_H
