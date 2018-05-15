#include <ros/ros.h>
#include "internal_use.h"
#include <iostream>

int main (int argc, char **argv) {
  ros::init(argc, argv, "unforgettable___mE__super_hero");
  DynRe dd;
  
  while (ros::ok()) {
    dd.CodeName();
    ros::spinOnce();
    sleep(1);
  }
}
