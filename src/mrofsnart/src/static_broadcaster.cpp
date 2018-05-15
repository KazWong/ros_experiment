#include <ros/ros.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ros;

Time t;

geometry_msgs::TransformStamped SetTF(string frame_id, string child_frame_id, tf2::Vector3 pos, tf2::Quaternion quat) {
  geometry_msgs::TransformStamped static_transformStamped;
  
  static_transformStamped.header.stamp = t;
  static_transformStamped.header.frame_id = frame_id;
  static_transformStamped.child_frame_id = child_frame_id;
  
  static_transformStamped.transform.translation.x = pos.getX();
  static_transformStamped.transform.translation.y = pos.getY();
  static_transformStamped.transform.translation.z = pos.getZ();

  static_transformStamped.transform.rotation.x = quat.x();
  static_transformStamped.transform.rotation.y = quat.y();
  static_transformStamped.transform.rotation.z = quat.z();
  static_transformStamped.transform.rotation.w = quat.w();
  
  return static_transformStamped;
}

int main(int argc, char** argv){
  init(argc, argv, "exp_static_tf_broadcaster");
  NodeHandle n;
  
  tf2::Quaternion q;
  tf2::Vector3 pos;
  t = Time::now();
  vector<geometry_msgs::TransformStamped> static_tf;
  tf2_ros::StaticTransformBroadcaster static_broadcaster;
  
  
  double laser_x, laser_y, laser_z;

  n.getParam("/Laser/laser_x", laser_x);
  n.getParam("/Laser/laser_y", laser_y);
  n.getParam("/Laser/laser_z", laser_z);
  

  //pos.setValue(laser_x, laser_y, laser_z);
  //static_tf.push_back(SetTF("base_footprint", "base_laser", pos, tf::Quaternion(0, 0, 0, 1)));
  
  q.setRPY(0.0, 0.0, 0.0);
  pos.setValue(0.0, 0.0, 0.0);
  static_tf.push_back(SetTF("real_camera_left", "real_camera_left_ei", pos, q));
  
  q.setRPY( 0.0, 0.0, 0.0);
  pos.setValue(0.0, 0.0, 0.0);
  static_tf.push_back(SetTF("real_camera_left", "real_camera_left_eb", pos, q));
  
  
  
  static_broadcaster.sendTransform(static_tf);
  
  spin();
}
