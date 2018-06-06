#include <ros/ros.h>
#include <iostream>

int main(int argc, char** argv) {
  ros::init(argc, argv, "printkp");
  
  ros::NodeHandle nh;
  
  double kp = 0.0; 
  
  nh.getParam("Kp", kp);
  ROS_INFO("%f", kp);
  return 0;
}
