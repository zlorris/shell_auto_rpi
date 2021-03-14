#include "ros/ros.h"
#include "airsim_car_ros_pkgs/CarCmd.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "car_control");

  // Initialize Publishers/Subscribers
  ros::NodeHandle n;
  ros::Publisher control = n.advertise<airsim_car_ros_pkgs::CarCmd>("/airsim_node/PhysXCar/car_cmd_body_frame", 1);
  while (!control.getNumSubscribers());

  // Car Control Commands
  airsim_car_ros_pkgs::CarCmd cmd;
    
  cmd.throttle = 1.0;
  cmd.gear_immediate = true;

  control.publish(cmd);

  std::cout << "Finished." << std::endl;

  return 0;
}