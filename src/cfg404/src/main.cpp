#include <ros/ros.h>
#include "internal_use.h"
#include <iostream>

int main (int argc, char **argv) {
  ros::init(argc, argv, "universal___no__config_package");
  KisXX dd;
  
  while (ros::ok()) {
    dd.CodeName();
    ros::spinOnce();
    sleep(1);
  }
}
