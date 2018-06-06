#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include "spinthread.h"

using namespace std;
using namespace ros;

void VelCallback(const geometry_msgs::Twist::ConstPtr& cmd_msg) {
  double vx, vy, omega;

  
  vx = cmd_msg->linear.x;
  vy = cmd_msg->linear.y;
  omega = cmd_msg->angular.z;

  ROS_INFO("Input vel: %f %f %f\n", vx, vy, omega);
}

void readCommand(const std_msgs::String::ConstPtr& msg) {
  int i=0;
  string string_cmd;
  for(; i<msg->data.length();i++) {
    string_cmd[i] = msg->data[i];
  }
  string_cmd[i] = '\0';
  
  ROS_INFO("Received Command: %s", string_cmd.c_str());
}

int main (int argc, char **argv) {
  ros::init(argc, argv, "littlespringspring_sub");
  ros::NodeHandle nh;
  
  SpinThread th(50);
  
  ros::Subscriber pose_sub_ = nh.subscribe<geometry_msgs::Twist>("cmd_vel", 10, VelCallback);
  //ros::Subscriber encoder_sub_ = nh.subscribe<description::PosCmd>("encoder", 10, ENCallback);
  ros::Subscriber serial_por_sub = nh.subscribe<std_msgs::String>("schedule_serial", 1, readCommand);
  
  sleep(10);
  
  //ros::AsyncSpinner spinner(3);
  //spinner.start();
  
  ros::Rate loop_rate(10);
  while (ros::ok()) {
    std::cout << "s1" << endl;
    //ros::spinOnce(); 
    std::cout << "s2" << endl;
    //ros::spinOnce(); 
    std::cout << "s3" << endl;
    //ros::spinOnce(); 
    std::cout << "s4" << endl;
    //ros::spinOnce(); 
    loop_rate.sleep();
    std::cout << "s5" << endl;
    //ros::spinOnce(); 
    loop_rate.sleep();
    loop_rate.sleep();
    std::cout << "s6" << endl;
    //ros::spinOnce(); 
  }
}
