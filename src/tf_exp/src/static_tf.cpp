#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char** argv){
  ros::init(argc, argv, "ar_tf_broadcaster_ht");
  ros::NodeHandle n;
  tf::Quaternion q;
  tf::Vector3 pos;

  ros::Rate r(50);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    ros::Time t = ros::Time::now();
    t += ros::Duration(0.5);
    
    q.setRPY(2.101, -1.434, 2.642);
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform( q, tf::Vector3( -0.065, -0.498, 1.114)),
        t,"ar_marker_0", "ar_parked_HT"));


    q.setRPY(-1.501, 0.154, -3.095);
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform( q, tf::Vector3(0.063, -0.223, 0.522)), //-0.309
        t,"ar_marker_0", "ht_takepic"));

    q.setRPY(0,0,0);
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform( q, tf::Vector3( 0.4, 0, 0)),
        t,"ar_parked_HT", "ar_unparked_HT"));

    ros::spinOnce();
    r.sleep();
  }
}
