#include <ros/ros.h>
#include <iostream>
#include "stdio.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <memory>
#include <std_msgs/Float64.h>

using namespace ros;
using namespace std;

typedef std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> NanoClock;

int __argc; 
char** __argv;
bool alive = true;
bool static_err = false;
double zero = 0.0333333333;
double step = 0.0;//0.0001;

vector<double> ros_data;
vector<double> sleep_data;
vector<double> u_data;
vector<double> nano_data;
vector<double> select_data;

inline double TNow(NanoClock *prev_entime) {
  NanoClock tnow = chrono::high_resolution_clock::now();
  chrono::duration<double> _dt(tnow - *prev_entime);
  *prev_entime = tnow;
  
  return _dt.count();
}

void Load() {
  double x=0.0, y=1.0, rz=2.0, gama=100.0, rw = 10.0;
  double back_right=0.0, front_right=0.0, front_left=0.0, back_left=0.0;
  for (int i=0;i<100;i++) {
    back_right  += (1.0 / rw) * (x - y + rz * gama);
    front_right += (1.0 / rw) * (x + y + rz * gama);
    front_left  += (1.0 / rw) * (-x +y + rz * gama);
    back_left   += (1.0 / rw) * (-x -y + rz * gama);
    
    x += back_right + front_right + front_left + back_left;
    y *= gama * (1.0 / rw);
    rz = sqrt(x*x + y*y);
  }
}

void PubData() {
  int has_data = 0;
  int i=0;

  int ros_max = ros_data.size();
  int sleep_max = sleep_data.size();
  int u_max = u_data.size();
  int nano_max = nano_data.size();
  int select_max = select_data.size();
  int total_max = ros_max + sleep_max + u_max + nano_max + select_max;
  
  ros::init(__argc, __argv, "looop_delay_test");
  ros::NodeHandle ph("~");
  ros::Publisher pub_select = ph.advertise<std_msgs::Float64>("dt_select", 1);
  ros::Publisher pub_nanosleep = ph.advertise<std_msgs::Float64>("dt_nanosleep", 1);
  ros::Publisher pub_usleep = ph.advertise<std_msgs::Float64>("dt_usleep", 1);
  ros::Publisher pub_sleep = ph.advertise<std_msgs::Float64>("dt_chronosleep", 1);
  ros::Publisher pub_rossleep = ph.advertise<std_msgs::Float64>("dt_rossleep", 1);
  
  ros::Rate rate(30);
  while (has_data < total_max && ros::ok()) {
    std_msgs::Float64 _dt;
    if (i < ros_max) {
      _dt.data = ros_data[i];
      pub_rossleep.publish(_dt);
      has_data++;
    }
    if (i < sleep_max) {
      _dt.data = sleep_data[i];
      pub_sleep.publish(_dt);
      has_data++;
    }
    if (i < u_max) { 
      _dt.data = u_data[i];
      pub_usleep.publish(_dt);
      has_data++;
    }
    if (i < nano_max) {
      _dt.data = nano_data[i];
      pub_nanosleep.publish(_dt);
      has_data++;
    }
    if (i < select_max) {
      _dt.data = select_data[i];
      pub_select.publish(_dt);
      has_data++;
    }
    
    rate.sleep();
    i++;
  }
}


void LoopROSSleep() {
  ros::init(__argc, __argv, "looop_delay_test");
  ros::NodeHandle ph("~");
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  
  //ros::Rate rate(20);//20Hz
  ros::Rate rate(30);//30Hz
  //ros::Rate rate(80);//80Hz
  //ros::Rate rate(100);//100Hz
  
  while(alive) {
    ros_data.push_back( TNow(&prev_entime) - zero + 1.0*step );
    Load();
    rate.sleep();
  }
}

void LoopSleep() {
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  //long t = 50000000;//20Hz
  long t = 33333333;//30Hz
  //long t = 12500000;//80Hz
  //long t = 10000000;//100Hz
  
  if (static_err)
    t -= 130000;
  
  while(alive) {
    sleep_data.push_back( TNow(&prev_entime) - zero + 2.0*step );
    Load();
    std::this_thread::sleep_for(std::chrono::nanoseconds(t));
  }
}

void LoopUSleep() {
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  //unsigned int t = 50000;//20Hz
  unsigned int t = 33333;//30Hz
  //unsigned int t = 12500;//80Hz
  //unsigned int t = 10000;//100Hz
  
  if (static_err)
    t -= 130;
  
  while(alive) {
    u_data.push_back( TNow(&prev_entime) - zero + 3.0*step );
    Load();
    usleep(t);
  }
}

void LoopNanoSleep() {
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  timespec t, rem, remrem;
  t.tv_sec = 0;
  //t.tv_nsec = 50000000;//20Hz
  t.tv_nsec = 33333333;//30Hz
  //t.tv_nsec = 12500000;//80Hz
  //t.tv_nsec = 10000000;//100Hz
  
  if (static_err)
    t.tv_nsec -= 130000;
  
  while(alive) {
    nano_data.push_back( TNow(&prev_entime) - zero + 4.0*step );
    Load();
    nanosleep(&t, &rem);
    //if (nanosleep(&t, &rem) == -1)
    //  nanosleep(&rem, &remrem);
  }
}

void LoopSelect() {
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  
  while(alive) {
    timeval t;
    t.tv_sec = 0;
    //t.tv_usec = 50000;//20Hz
    t.tv_usec = 33333;//30Hz
    //t.tv_usec = 12500;//80Hz
    //t.tv_usec = 10000;//100Hz
    
    if (static_err)
      t.tv_usec -= 130;
    
    select_data.push_back( TNow(&prev_entime) - zero + 5.0*step );
    Load();
    select(0, NULL, NULL, NULL, &t);
  }
}

int main(int argc, char** argv)
{
  __argc = argc;
  __argv = argv;
  std::thread th_ROSsleep(&LoopROSSleep);
  //std::thread th_sleep(&LoopSleep);
  //std::thread th_usleep(&LoopUSleep);
  std::thread th_nanosleep(&LoopNanoSleep);
  std::thread th_select(&LoopSelect);
  
  sleep(30);
  
  alive = false;
  /*if (th_sleep.joinable())
    th_sleep.join();
  if (th_usleep.joinable())
    th_usleep.join();*/
  if (th_ROSsleep.joinable())
    th_ROSsleep.join();
  if (th_nanosleep.joinable())
    th_nanosleep.join();
  if (th_select.joinable())
    th_select.join();
  
  
  PubData();
}

