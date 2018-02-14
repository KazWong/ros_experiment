#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "laser_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  double laser_x, laser_y, laser_z;

  n.getParam("/Laser/laser_x", laser_x);
  n.getParam("/Laser/laser_y", laser_y);
  n.getParam("/Laser/laser_z", laser_z);

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(laser_x, laser_y, laser_z)),
        ros::Time::now(),"base_footprint", "base_laser"));
    r.sleep();
  }
}
