#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

using namespace std;
using namespace ros;

int main (int argc, char **argv) {
  ros::init(argc, argv, "littlespringspring_pub");
  ros::NodeHandle nh;
  
  ros::Publisher pose_pub_ = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
  //ros::Publisher encoder_pub_ = nh.advertise<description::PosCmd>("encoder", 10);
  ros::Publisher serial_por_pub = nh.advertise<std_msgs::String>("schedule_serial", 10);
  
  double count = 0.0;
  geometry_msgs::Twist twi_msg;
  std_msgs::String str_msg;
  
  ros::Rate loop_rate(10);
  loop_rate.sleep();
  
  while (ros::ok()) {
    Time current_time = ros::Time::now();
    twi_msg.linear.x = count;
    twi_msg.linear.y = count++;
    twi_msg.angular.z = current_time.toNSec() / 1e9;
    pose_pub_.publish(twi_msg);
    
    str_msg.data = to_string(current_time.toNSec());
    str_msg.data.insert(10, ".");
    serial_por_pub.publish(str_msg);
  
    //ros::spinOnce();
    loop_rate.sleep();
  }
}
