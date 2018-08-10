#include <ros/ros.h>
#include <iostream>
#include "stdio.h"
#include <thread>
#include <chrono>
#include <std_msgs/Float64.h>

using namespace ros;
using namespace std;

typedef std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> NanoClock;

bool alive = true;
double zero = 0.0333333333;
double step = 0.0001;

inline double TNow(NanoClock *prev_entime) {
  NanoClock tnow = chrono::high_resolution_clock::now();
  chrono::duration<double> _dt(tnow - *prev_entime);
  *prev_entime = tnow;
  
  return _dt.count();
}

void Load() {
  /*double x=0.0, y=1.0, rz=2.0, gama=100.0, rw = 10.0;
  double back_right=0.0, front_right=0.0, front_left=0.0, back_left=0.0;
  for (int i=0;i<100;i++) {
    back_right  += (1.0 / rw) * (x - y + rz * gama);
    front_right += (1.0 / rw) * (x + y + rz * gama);
    front_left  += (1.0 / rw) * (-x +y + rz * gama);
    back_left   += (1.0 / rw) * (-x -y + rz * gama);
    
    x += back_right + front_right + front_left + back_left;
    y *= gama * (1.0 / rw);
    rz = sqrt(x*x + y*y);
  }*/
}


void LoopROSSleep() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_rossleep = ph.advertise<std_msgs::Float64>("dt_rossleep", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  //ros::Rate rate(20);//20Hz
  ros::Rate rate(30);//30Hz
  //ros::Rate rate(80);//80Hz
  //ros::Rate rate(100);//100Hz
  
  while(alive) {
    _dt.data = TNow(&prev_entime) - zero + 1.0*step;
    pub_rossleep.publish(_dt);
    Load();
    rate.sleep();
  }
}

void LoopSleep() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_sleep = ph.advertise<std_msgs::Float64>("dt_chronosleep", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  //long t = 50000000;//20Hz
  long t = 33333333;//30Hz
  //long t = 12500000;//80Hz
  //long t = 9920000;//100Hz
  
  while(alive) {
    _dt.data = TNow(&prev_entime) - zero + 2.0*step;
    pub_sleep.publish(_dt);
    Load();
    std::this_thread::sleep_for(std::chrono::nanoseconds(t));
  }
}

void LoopUSleep() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_usleep = ph.advertise<std_msgs::Float64>("dt_usleep", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  //unsigned int t = 50000;//20Hz
  unsigned int t = 33333;//30Hz
  //unsigned int t = 12500;//80Hz
  //unsigned int t = 9920;//100Hz
  
  while(alive) {
    _dt.data = TNow(&prev_entime) - zero + 3.0*step;
    pub_usleep.publish(_dt);
    Load();
    usleep(t);
  }
}

void LoopNanoSleep() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_nanosleep = ph.advertise<std_msgs::Float64>("dt_nanosleep", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  timespec t, rem, remrem;
  t.tv_sec = 0;
  //t.tv_nsec = 50000000;//20Hz
  t.tv_nsec = 33333333;//30Hz
  //t.tv_nsec = 12500000;//80Hz
  //t.tv_nsec = 9900000;//100Hz
  
  while(alive) {
    _dt.data = TNow(&prev_entime) - zero + 4.0*step;
    pub_nanosleep.publish(_dt);
    Load();
    if (nanosleep(&t, &rem) == -1)
      nanosleep(&rem, &remrem);
  }
}

void LoopClockNanoSleep() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_clocknanosleep = ph.advertise<std_msgs::Float64>("dt_clocknanosleep", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
 
  
  while(alive && ros::ok()) {
    _dt.data = TNow(&prev_entime) - zero + 5.0*step;
    pub_clocknanosleep.publish(_dt);
    Load();
    
    timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    //t.tv_nsec += 50000000;//20Hz
    t.tv_nsec += 33333333;//30Hz
    //t.tv_nsec += 12500000;//80Hz
    //t.tv_nsec += 9900000;//100Hz
    if(t.tv_nsec >= 1000000000) {
      t.tv_nsec -= 1000000000;
      t.tv_sec++;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
  }
}

void LoopSelect() {
  ros::NodeHandle ph("~");
  ros::Publisher pub_select = ph.advertise<std_msgs::Float64>("dt_select", 1);
  NanoClock prev_entime = chrono::high_resolution_clock::now();
  std_msgs::Float64 _dt;
  
  while(alive) {
    _dt.data = TNow(&prev_entime) - zero + 6.0*step;
    pub_select.publish(_dt);
    Load();
    
    timeval t;
    t.tv_sec = 0;
    //t.tv_usec = 50000;//20Hz
    t.tv_usec = 33333;//30Hz
    //t.tv_usec = 12500;//80Hz
    //t.tv_usec = 9920;//100Hz
    select(0, NULL, NULL, NULL, &t);
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "looop_delay_test_nano");
  
  ros::AsyncSpinner spinner(2);
  spinner.start();
  
  //std::thread th_ROSsleep(&LoopROSSleep);
  //std::thread th_sleep(&LoopSleep);
  //std::thread th_usleep(&LoopUSleep);
  //std::thread th_nanosleep(&LoopNanoSleep);
  //std::thread th_clock_nanosleep(&LoopClockNanoSleep);
  //std::thread th_select(&LoopSelect);
  
  //ros::waitForShutdown();
  
  LoopClockNanoSleep();
  
  //alive = false;
  /*if (th_sleep.joinable())
    th_sleep.join();
  if (th_usleep.joinable())
    th_usleep.join();
  if (th_ROSsleep.joinable())
    th_ROSsleep.join();
  if (th_nanosleep.joinable())
    th_nanosleep.join();
  if (th_clock_nanosleep.joinable())
    th_clock_nanosleep.join();
  if (th_select.joinable())
    th_select.join();*/
}

