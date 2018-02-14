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
#include <XmlRpcValue.h>

using namespace std;
using namespace ros;

void global_relative() {
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
  {
  // getParam global vector quick
  std::vector<double> my_double_list;
  n.getParam("quick", my_double_list);
  
  cout << "****global quick Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  
  // getParam private vector quick
  my_double_list.clear();
  pn.getParam("quick", my_double_list);
  
  cout << "****private quick Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  
  // getParam private quick from global nodehandle
  my_double_list.clear();
  n.getParam("format_test/quick", my_double_list);
  
  cout << "****global private quick Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  
  // getParam private quick by namespaced nodehandle
  my_double_list.clear();
  gpn.getParam("quick", my_double_list);
  
  cout << "****namespaced nodehandle quick Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  }
}

void keys() {
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
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
  gpn.getParamNames(my_double_list); //both pn or gpn are the same
  
  cout << "****private parameters Param size : " << my_double_list.size() << endl;
  for(int i=0; i < my_double_list.size(); i++) {
    cout << my_double_list[i] << endl;
  }
  }
}

void nodehandle_names() {
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
  {
  const std::string list1 = n.getUnresolvedNamespace();
  const std::string list2 = pn.getUnresolvedNamespace();
  const std::string list3 = gpn.getUnresolvedNamespace();
  
  cout << "n   " << list1 << endl;
  cout << "pn  " << list2 << endl;
  cout << "gpn " << list3 << endl;
  }
}

void vector_map() {
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
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
  
  cout << "****why vector Param size : " << my_double_list.size() << endl;
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
  }
}

void tf_format_test() {
/*
ds:
  d:
    - 2.0
    - 2.1
    - 2.2
    - 2.3
    - 2.4
    - 2.5
  s: mama
*/
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
  
  {
  // getParam ds double+string
  std::vector<double> list;
  string parent;
  n.getParam("ds/d", list);
  n.getParam("ds/s", parent);
  
  cout << "****quick vector Param size : " << list.size() << endl;
  for(int i=0; i < list.size(); i++) {
    cout << list[i] << endl;
  }
  cout << parent << endl;
  }
}

void tf_format_test2() {
/*
dns:
  d:
    - 2.0
    - 2.1
    - 2.2
    - 2.3
    - 2.4
    - 2.5
dns:
  s: mama
  
result : overwrite while param has the same name, type doesnt matter
*/
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
  
  {
  // getParam ds double+string
  std::vector<double> list;
  string parent;
  n.getParam("dns/d", list);
  n.getParam("dns/s", parent);
  
  cout << "****quick vector Param size : " << list.size() << endl;
  for(int i=0; i < list.size(); i++) {
    cout << list[i] << endl;
  }
  cout << parent << endl;
  }
}

int main(int argc, char** argv){
  init(argc, argv, "format_test");
  
  NodeHandle n;
  NodeHandle pn("~");
  NodeHandle gpn("some_namespace");
   
  //keys();
  //nodehandle_names();
  //global_relative();
  //vector_map();
  //tf_format_test();
  //tf_format_test2();//fail
}
