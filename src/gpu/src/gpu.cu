#include <iostream>
#include <math.h>

// CUDA Kernel function to add the elements of two arrays
__global__
void GPU_add(long n, float *x, float *y)
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

void AssUnifiedMem(long n, float **_x, float **_y) {
  // Allocate Unified Memory -- accessible from CPU or GPU
  cudaMallocManaged(_x, n*sizeof(float));
  cudaMallocManaged(_y, n*sizeof(float));
}

void FreeUnifiedMem(float *_x, float *_y) {
  // Free memory
  cudaFree(_x);
  cudaFree(_y);
}


void add(long n, float *_x, float *_y)
{
  // Run kernel on 1M elements on the CPU
//  add<<<1, 256>>>(N, x, y);
  int blockSize = 256;
  int numBlocks = (n + blockSize - 1) / blockSize;
  GPU_add<<<numBlocks, blockSize>>>(n, _x, _y);

  // Wait for GPU to finish before accessing on host
  cudaDeviceSynchronize();
}
