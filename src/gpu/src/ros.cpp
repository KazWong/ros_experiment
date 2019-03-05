#include <ros/ros.h>
#include <iostream>
#include <gpu.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "gpu_add");
  ROS_INFO("Start gpu_add");
  
  ros::NodeHandle nh();
  long N = 1<<25; // 1M elements


  float *x = nullptr;
  float *y = nullptr;
  
  AssUnifiedMem(N, &x, &y);

  // initialize x and y arrays on the host
  for (long i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  // Run kernel on 1M elements on the CPU
  add(N, x, y);

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (long i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i]-3.0f));
  ROS_INFO("Max error: %f", maxError);

  // Free memory
  FreeUnifiedMem(x, y);

  return 0;
}
