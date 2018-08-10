#include <ros/ros.h>
#include <iostream>

using namespace std;

int main (int argc, char **argv) {
  ros::init(argc, argv, "envirvar_parameterserver");
  
  while (ros::ok()) {
    char * path;
    path = getenv("PATH");
    
    cout << path << endl;
    
    sleep(1);
  }
}
