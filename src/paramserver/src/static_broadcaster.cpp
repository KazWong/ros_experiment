#include <ros/ros.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iterator>

using namespace std;
using namespace ros;

Time t;
string priname = "static_tf";
string key1 = "Coordinate";
string key2 = "ID";
string prefix = "Frame_";

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
  init(argc, argv, "paramarap");
  NodeHandle n(priname);
  
  tf2::Quaternion q;
  tf2::Vector3 pos;
  t = Time::now();
  vector<geometry_msgs::TransformStamped> static_tf;
  tf2_ros::StaticTransformBroadcaster static_broadcaster;
  
  std::vector<string> param_name_list;
  n.getParamNames(param_name_list);
  cout << "static_tf param" << endl;
  string a = "1";
  a += std::to_string(2);
  cout << "a" << a << endl;
  for(int i=0; i < param_name_list.size(); i++) {
    int ns = param_name_list[i].find(priname);
    if (ns != std::string::npos) {
      cout << param_name_list[i] << endl;
      int id = param_name_list[i].find(prefix);
      cout << "ns " << param_name_list[i][ns] << " id " << param_name_list[i][id]  << endl;
      cout << "ns " << param_name_list[i][ns+priname.length()+1] << " id " << param_name_list[i][id-2]  << endl;
      /*if (param_name_list[i].find(key1) != std::string::npos) {
      
      } else if (param_name_list[i].find(key2) != std::string::npos) {
      
      }*/
      
    }
  }
  
  /*q.setRPY(-1.635, -0.103, 0.038);
  pos.setValue(0.127, -0.493, 1.235);
  static_tf.push_back(SetTF("ar_marker_0", "ar_parked_CS", pos, q));*/
  
  static_broadcaster.sendTransform(static_tf);
  
  spin();
}
