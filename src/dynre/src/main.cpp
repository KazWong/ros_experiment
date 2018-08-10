#include <ros/ros.h>
#include "internal_use.h"
#include <iostream>
#include <chrono>

int main(int argc, char **argv) {
  ros::init(argc, argv, "unforgettable___mE__super_hero");
  DynRe dd;
  
  //std::chrono::steady_clock::time_point
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> stnow;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> prev_stime;
  
  //std::chrono::high_resolution_clock::time_point
  std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> htnow;
  std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> prev_htime;
  
  ros::Time rtnow;
  ros::Time prev_rtime;
  while (ros::ok()) {
    dd.CodeName();
    
    stnow = std::chrono::steady_clock::now();
    htnow = std::chrono::high_resolution_clock::now();
    rtnow = ros::Time::now();
    
    std::chrono::duration<float> sdt(stnow - prev_stime);
    std::chrono::duration<float> hdt(htnow - prev_htime);
    double rdt = (rtnow - prev_rtime).toSec();
    
    //std::time_t snow_c = std::chrono::steady_clock::to_time_t(stnow);
    //std::time_t hnow_c = std::chrono::high_resolution_clock::to_time_t(htnow);
    long snow_c = std::chrono::duration_cast<std::chrono::nanoseconds>(stnow.time_since_epoch()).count();
    long hnow_c = std::chrono::duration_cast<std::chrono::nanoseconds>(htnow.time_since_epoch()).count();
  
    std::cout << "steady_clock\t\t" << (long)(snow_c/1e9) << "." << (long)snow_c%(long)1e9 << "\t" << sdt.count() << std::endl;
    std::cout << "high_resolution_clock\t" << (long)(hnow_c/1e9) << "." << (long)hnow_c%(long)1e9 << "\t" << hdt.count() << std::endl;
    std::cout << "ros::Time\t\t" << rtnow.sec << "." << rtnow.nsec << "\t" << rdt << std::endl;
    
    ros::spinOnce();
    sleep(1);
    prev_stime = stnow;
    prev_htime = htnow;
    prev_rtime = rtnow;
  }
}
