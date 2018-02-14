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


int main(int argc, char** argv){
  init(argc, argv, "format_test");
  NodeHandle n;
  NodeHandle pn("~");
   
  {
  // Get the keys for all the parameters in the parameter server. 
  std::vector<string> my_double_list;
  n.getParamNames(my_double_list);
  
  cout << "****all the parameters Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  }
  
  {
  // Get the keys for private namespace in the parameter server. 
  std::vector<string> my_double_list;
  pn.getParamNames(my_double_list);
  
  cout << "****private parameters Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  }


/*
  {
  // getParam vector quick
  std::vector<double> my_double_list;
  n.getParam("quick", my_double_list);
  
  cout << "****quick vector Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  
  // getParam map
  map<string, double> map_s;
  n.getParam("CS", map_s);
  
  map<string, double>::iterator it;
  int i=0;
  cout << "****CS Map Param size : " << map_s.size() << endl;
  for(it=map_s.begin(); it != map_s.end(); it++, i++) {
    cout << "key: " << it->first << " value: " << it->second << endl;
  }
  }
  
  

  {
  // getParam vector qq
  vector<double> my_double_list;
  n.getParam("why", my_double_list);
  
  cout << "****qq vector Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  
  // getParam map 
  map<string, double> map_s;
  n.getParam("EE", map_s);
  
  map<string, double>::iterator it;
  int i=0;
  cout << "****EE Map Param size : " << map_s.size() << endl;
  for(it=map_s.begin(); it != map_s.end(); it++, i++) {
    cout << "key: " << it->first << " value: " << it->second << endl;
  }
*/
}
