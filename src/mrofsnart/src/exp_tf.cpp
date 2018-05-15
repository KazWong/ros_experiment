#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "laser_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  double laser_x, laser_y, laser_z;
  double cam_x, cam_y, cam_z;

  n.param("/experiment/laser_x", laser_x, 0.0);
  n.param("/experiment/laser_y", laser_y, 0.0);
  n.param("/experiment/laser_z", laser_z, 0.0);
  
  n.param("/experiment/cam_x", cam_x, 0.0);
  n.param("/experiment/cam_y", cam_y, 0.0);
  n.param("/experiment/cam_z", cam_z, 0.1);

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(laser_x, laser_y, laser_z)),
        ros::Time::now(),"base_footprint", "zed_camera"));
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(laser_x, laser_y, laser_z)),
        ros::Time::now(),"base_footprint", "sweep_lidar"));
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(cam_x, cam_y, cam_z)),
        ros::Time::now(),"base_footprint", "cam"));
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(laser_x, laser_y, laser_z)),
        ros::Time::now(),"base_footprint", "base_laser"));
        
        
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"odom", "base_footprint"));
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.0, 0.0, 0.0)),
        ros::Time::now(),"map", "odom"));
    r.sleep();
  }
}
