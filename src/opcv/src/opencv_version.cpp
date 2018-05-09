#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv) {
  //ros::init(argc, argv, "check_opencv_version");
  std::cout << "OpenCV version " << CV_VERSION << std::endl;
  return 0;
}
