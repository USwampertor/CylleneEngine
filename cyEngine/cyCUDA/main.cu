#include "cuda.h"
#include "cuda_runtime.h"
#include "device_functions.h"
#include "device_launch_parameters.h"
#include "math.h"

#include <FreeImage/FreeImage.h>

#include <stdio.h>

#include <cyTime.h>
#include <cyMath.h>
#include <cyVector3f.h>

using namespace CYLLENE_SDK;

bool
LoadTexture(String texturePath, uint8** output, uint32& width, uint32& height);

/**
* gridDim (dim3)
*   dimensions of grid
* blockDim (dim3)
*   dimensions of block
* blockIdx (uint3)
*   block index within grid
* threadIdx (uint3)
*   thread index within block
*/

__device__ int
GetLinearIndex() {
  int blockId = (gridDim.x * gridDim.y * blockIdx.z) +
                (blockIdx.y * gridDim.x) + blockIdx.x;

  int threadId = ((blockDim.x * blockDim.y * blockDim.z) * blockId) +
                 ((blockDim.x * blockDim.y) * threadIdx.z) +
                 ((threadIdx.x * blockDim.y) + threadIdx.x);
  return threadId;
}

__global__ void
InitializeBuffer(uint32 totalPixels, unsigned int* histogram) {
  unsigned int pixelIndex = (blockDim.x * blockIdx.x) + threadIdx.x;
  
  if (pixelIndex >= totalPixels) return;

  const uint32 IMAGE_CHANNELS = 4;

  const unsigned int channel = blockIdx.y;

  histogram[(pixelIndex * 3) + channel] = 0;
}

__global__ void
Histogram(uint8* bytes, int* histogram, uint32 totalPixels) {
  unsigned int pixelIndex = (blockDim.x * blockIdx.x) + threadIdx.x;

  if (pixelIndex >= totalPixels) return;

  const uint32 IMAGE_CHANNELS = 4;

  const unsigned int channel = blockIdx.y;

  uint8 pixel_channel = bytes[(pixelIndex * IMAGE_CHANNELS) + channel];

  atomicAdd(&histogram[(pixel_channel * 3) + channel], 1);
  //atomicAdd(&histogram[channel], 1);
}

int
main(int argc, char** argv) {
  auto secEnum = DELTA_TYPE::E::SECOND;

  cudaError_t cudaStatus;

  uint32 imageWidth;
  uint32 imageHeight;
  uint32 totalPixels;

  uint8* bytes = nullptr;
  int* histogramValues = nullptr;

  std::string currentDirectory(argv[0]);
  currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of('\\') + 1);
  printf("Current path: %s\n", currentDirectory.c_str());

  {
    uint8* imgBytes = nullptr;
    if (!LoadTexture(currentDirectory + "wench.jpg", &imgBytes, imageWidth, imageHeight)) {
      return -1;
    }

    cudaStatus = cudaMallocManaged(&bytes,
                                   imageWidth * imageHeight * 4 * sizeof(uint8));
    if (cudaStatus != cudaSuccess) {
      return -1;
    }

    std::memcpy(bytes, imgBytes, imageWidth * imageHeight * 4 * sizeof(uint8));

    delete[] imgBytes;
  }

  totalPixels = imageWidth * imageHeight;

  printf("Image loaded. Size = %dx%d\n", imageWidth, imageHeight);

  cudaStatus = cudaMallocManaged(&histogramValues, 256 * sizeof(int) * 3);
  if (cudaStatus != cudaSuccess) {
    return -1;
  }

  for (int i = 0; i < 256; ++i) {
    histogramValues[(i * 3) + 0] = 0;
    histogramValues[(i * 3) + 1] = 0;
    histogramValues[(i * 3) + 2] = 0;
  }

  Time timer;
  timer.init();

  dim3 blockSize(1024, 1, 1);
  dim3 dispatchSize(Math::ceil(totalPixels / 1024.0f), 3, 1);

  Histogram<<<dispatchSize, blockSize>>>(bytes, histogramValues, totalPixels);
  cudaStatus = cudaGetLastError();
  if (cudaStatus != cudaSuccess) {
    fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
  }

  cudaDeviceSynchronize();

  timer.update();
  printf("Initialization time: %f\n", timer.deltaTime(secEnum));

  for (int i = 0; i < 256; ++i) {
    int r = histogramValues[(i * 3) + 0];
    int g = histogramValues[(i * 3) + 1];
    int b = histogramValues[(i * 3) + 2];

    printf("Pixel %d: %d, %d, %d\n", i, r, g, b);
  }

  timer.update();
  printf("Time required to compute: %f\n", timer.deltaTime(secEnum));

  cudaFree(bytes);
  cudaFree(histogramValues);

  return 0;
}


bool
LoadTexture(String texturePath, uint8** output, uint32& width, uint32& height) {
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(texturePath.c_str());

  uint32 flags = 0;
  FIBITMAP* fileImage = FreeImage_Load(fif, texturePath.c_str(), flags);

  if (fileImage == nullptr) {
    printf("Error while loading image\n");
    return false;
  }

  FIBITMAP* image = FreeImage_ConvertTo32Bits(fileImage);
  FreeImage_Unload(fileImage);

  if (image == nullptr) {
    printf("Error while converting image\n");
    return false;
  }

  BYTE* bits = FreeImage_GetBits(image);

  if (bits == nullptr) {
    printf("Error while getting image bytes\n");
    return false;
  }

  width = FreeImage_GetWidth(image);
  height = FreeImage_GetHeight(image);
  unsigned int bytes = FreeImage_GetBPP(image) / 8;

  (*output) = reinterpret_cast<uint8*>(malloc(width * height * bytes));
  std::memcpy((*output), &bits[0], width * height * bytes);

  FreeImage_Unload(image);
  return true;
}