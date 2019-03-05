#include <iostream>
#include <math.h>

// CUDA Kernel function to add the elements of two arrays
__global__
void add(long n, float *x, float *y)
{
/**
  for (int i = 0; i < n; i++)
      y[i] = x[i] + y[i];
**/
/**
  int index = threadIdx.x;
  int stride = blockDim.x;
  for (int i = index; i < n; i += stride)
      y[i] = x[i] + y[i];
**/
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (long i = index; i < n; i += stride)
    y[i] = x[i] + y[i];
}

int main(void)
{
  long N = 1<<25; // 1M elements

/**
  float *x = new float[N];
  float *y = new float[N];
**/
  // Allocate Unified Memory -- accessible from CPU or GPU
  float *x, *y;
  cudaMallocManaged(&x, N*sizeof(float));
  cudaMallocManaged(&y, N*sizeof(float));


  // initialize x and y arrays on the host
  for (long i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  // Run kernel on 1M elements on the CPU
//  add<<<1, 256>>>(N, x, y);
  int blockSize = 256;
  int numBlocks = (N + blockSize - 1) / blockSize;
  add<<<numBlocks, blockSize>>>(N, x, y);

  // Wait for GPU to finish before accessing on host
  cudaDeviceSynchronize();

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (long i = 0; i < N; i++)
    maxError = fmax(maxError, fabs(y[i]-3.0f));
  std::cout << "Max error: " << maxError << std::endl;

/**
  // Free memory
  delete [] x;
  delete [] y;
**/
  // Free memory
  cudaFree(x);
  cudaFree(y);

  return 0;
}
