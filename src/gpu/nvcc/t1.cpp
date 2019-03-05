#include <iostream>
#include <math.h>
#include <gperftools/profiler.h>

// function to add the elements of two arrays
void add(long n, float *x, float *y)
{
  for (long i = 0; i < n; i++)
      y[i] = x[i] + y[i];
}

int main(void)
{
  ProfilerStart("add_main.log");
  long N = 1<<25; // 1M elements


  float *x = new float[N];
  float *y = new float[N];

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
  std::cout << "Max error: " << maxError << std::endl;


  // Free memory
  delete [] x;
  delete [] y;

  ProfilerStop();
  
  return 0;
}
