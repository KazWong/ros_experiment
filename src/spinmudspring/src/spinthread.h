#ifndef SPINTHREAD_H_
#define SPINTHREAD_H_

#include <ros/ros.h>
#include <memory>
#include <mutex>
#include <thread>

class SpinThread {
private:
  bool alive;
  ros::Rate rate;
  std::thread callback_th;
  
  void Spin() {
    while (alive) {
      ros::spinOnce();
      rate.sleep();
    }
  };
public:
  SpinThread(int _rate) : rate(_rate), alive(true), callback_th(&SpinThread::Spin, this) {};
  ~SpinThread() {
    alive = false;
    if (callback_th.joinable())
      callback_th.join();
  };
};

#endif
