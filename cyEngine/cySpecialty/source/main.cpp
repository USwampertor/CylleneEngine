#include <iostream>
#include <CL/cl.h>
#include <vector>
#include <fstream>
#include <filesystem>

#include <CL/cl_platform.h>
#include <CL/cl_d3d11.h>
#include <CL/cl_d3d11_ext.h>
#include <d3d11.h>
#include <dxgi.h>

#include <cyUnitTesting.h>

#include <cyDLLLoader.h>
#include <cyGraphicsAPI.h>
#include <cyWindow.h> 
#include <cyLogger.h> 
#include <cyResourceManager.h>
#include <cyFileSystem.h>
#include <cyGShader.h>
#include <cyGMesh.h>
#include <cyGTexture.h>
#include <cyTime.h>
#include <cyGInputLayout.h>
#include <cyMatrix4.h>
#include <cyRModel.h>
#include <cyRShader.h>
#include <cyBBeing.h>
#include <cyCCamera.h>
#include <cyCTransform.h>
#include <cyCMeshRenderer.h>
#include <cyScene.h>
#include <cySceneManager.h>
#include <cyMath.h> 
#include <cyWindowEvent.h>
#include <cyCLight.h>
#include <cyCorePrerequisites.h>
#include <cyStdHeaders.h>


#define THWOW_ON_ERROR(err, msg) \
  if (err != CL_SUCCESS) { \
    std::cerr << "OpenCL Error: " << err << " message: " << msg << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    exit(EXIT_FAILURE); \
  }

using namespace CYLLENE_SDK;


int initDevice() {
  ID3D11Device* device = nullptr;
  ID3D11DeviceContext* context = nullptr;
  D3D_FEATURE_LEVEL featureLevel;
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT flags = 0;
  HRESULT hr = D3D11CreateDevice(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    flags,
    featureLevels,
    sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
    D3D11_SDK_VERSION,
    &device,
    &featureLevel,
    &context
  );
  return 0;
}

int main() {
  Logger::startUp();
  ResourceManager::startUp();
  Time::startUp();
  SceneManager::startUp();

 
  cl_int err;
  cl_uint platformCount = 0;

  err = clGetPlatformIDs(0, nullptr, &platformCount);
  THWOW_ON_ERROR(err, "Failed to get platform count");

  Vector<cl_platform_id> platforms(platformCount);
  err = clGetPlatformIDs(platformCount, platforms.data(), nullptr);
  THWOW_ON_ERROR(err, "Failed to get platform IDs");

  auto hasExtension = [](cl_platform_id p, const char* nameExt) {
    size_t size = 0;
    clGetPlatformInfo(p, CL_PLATFORM_EXTENSIONS, 0, nullptr, &size);
    Vector<char> extData(size, '\0');
    clGetPlatformInfo(p, CL_PLATFORM_EXTENSIONS, size, extData.data(), nullptr);
    return String(extData.data()).find(nameExt) != String::npos;
  };

  cl_platform_id selectedPlatform = nullptr;
  for (auto p : platforms) {
    if (hasExtension(p, "cl_nv_d3d11_sharing")) {
      selectedPlatform = p;
      break;
    }
  }

  auto clGetDeviceIDsFromD3D11NV = 
    (clGetDeviceIDsFromD3D11NV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clGetDeviceIDsFromD3D11NV"
    );

  auto clCreateFromD3D11BufferNV =
    (clCreateFromD3D11BufferNV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clCreateFromD3D11BufferNV"
    );

  auto clCreateFromD3D11Texture2DNV =
    (clCreateFromD3D11Texture2DNV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clCreateFromD3D11Texture2DNV"
    );

  auto clCreateFromD3D11Texture3DNV =
    (clCreateFromD3D11Texture3DNV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clCreateFromD3D11Texture3DNV"
    );

  auto clEnqueueAcquireD3D11ObjectsNV =
    (clEnqueueAcquireD3D11ObjectsNV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clEnqueueAcquireD3D11ObjectsNV"
    );

  auto clEnqueueReleaseD3D11ObjectsNV =
    (clEnqueueReleaseD3D11ObjectsNV_fn)clGetExtensionFunctionAddressForPlatform(
      selectedPlatform,
      "clEnqueueReleaseD3D11ObjectsNV"
    );

  WindowManager::startUp();
  WindowManager::instance().init();
  WindowManager::instance().createWindow("Test", Vector2i(640, 420));
  
  void* hwnd = WindowManager::instance().getWindowHandle(0);
  loadGFXModule(GFXTYPE::E::eDX11);
  GraphicsAPI::instance().initialize(hwnd);

  // ID3D11Device* d3d11Device = nullptr;
  // ID3D11DeviceContext* d3d11Context = nullptr;
  // D3D_FEATURE_LEVEL d3d11FeatureLevel;
  // D3D_FEATURE_LEVEL d3d11FeatureLevels[] = {
  //   D3D_FEATURE_LEVEL_11_0,
  //   D3D_FEATURE_LEVEL_10_0,
  // };
  // UINT flags = 0;
  HRESULT hr;
  // HRESULT hr = D3D11CreateDevice(
  //   nullptr,
  //   D3D_DRIVER_TYPE_HARDWARE,
  //   nullptr,
  //   flags,
  //   d3d11FeatureLevels,
  //   sizeof(d3d11FeatureLevels) / sizeof(D3D_FEATURE_LEVEL),
  //   D3D11_SDK_VERSION,
  //   &d3d11Device,
  //   &d3d11FeatureLevel,
  //   &d3d11Context
  // );


  // Iterate through platforms and devices
  cl_device_id device;

  bool useOldMethod = false;

  ID3D11Device* d3d11Device = reinterpret_cast<ID3D11Device*>(GraphicsAPI::instance().getDevice()->get());
  ID3D11DeviceContext* d3d11Context = reinterpret_cast<ID3D11DeviceContext*>(GraphicsAPI::instance().getDeviceContext()->get());
  if (useOldMethod) {
    for (cl_uint i = 0; i < platformCount; ++i) {

      cl_platform_id& platform = platforms[i];
      cl_uint deviceCount = 0;
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
      THWOW_ON_ERROR(err, "Failed to get OpenCL device count");

      Vector<cl_device_id> devices(deviceCount);
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, deviceCount, devices.data(), nullptr);


      for (cl_uint j = 0; j < deviceCount; ++j) {
        cl_device_id& deviceID = devices[j];
        size_t infoSize = 0;
        err = clGetDeviceInfo(deviceID, CL_DEVICE_NAME, 0, nullptr, &infoSize);
        THWOW_ON_ERROR(err, "Failed to get device info size");

        Vector<char> deviceName(infoSize);
        err = clGetDeviceInfo(deviceID, CL_DEVICE_NAME, infoSize, deviceName.data(), nullptr);
        THWOW_ON_ERROR(err, "Failed to get device info");

        std::cout << "Platform " << i << ", Device " << j << ": " << deviceName.data() << std::endl;
      }


      if (deviceCount > 0) {
        device = devices[0];
      }
      else {
        std::cout << "No devices found for platform " << i << std::endl;
        continue;
      }
    }
  }
  else {
    // Get device from D3D11
    cl_uint numDevices = 0;
    err = clGetDeviceIDsFromD3D11NV(selectedPlatform,
                                    CL_D3D11_DEVICE_NV,
                                    d3d11Device,
                                    CL_PREFERRED_DEVICES_FOR_D3D11_NV,
                                    0,
                                    nullptr,
                                    &numDevices);

    THWOW_ON_ERROR(err, "Failed to get device count from D3D11");

    Vector<cl_device_id> devices(numDevices);

    err = clGetDeviceIDsFromD3D11NV(selectedPlatform,
                                    CL_D3D11_DEVICE_NV,
                                    d3d11Device,
                                    CL_PREFERRED_DEVICES_FOR_D3D11_NV,
                                    numDevices,
                                    devices.data(),
                                    nullptr);

    device = devices[0];
  }

  cl_context_properties contextProperties[] = {
    CL_CONTEXT_PLATFORM, (cl_context_properties)selectedPlatform,
    CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)d3d11Device,//GraphicsAPI::instance().getDevice()->get(),
    0
  };

  // Create OpenCL device context
  cl_context context = clCreateContext(contextProperties, 1, &device, nullptr, nullptr, &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL context");

  // Create command queue
  cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, nullptr, &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL command queue");

  // D3D11_TEXTURE2D_DESC desc;
  // memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
  // desc.Width = 512;
  // desc.Height = 512;
  // desc.MipLevels = 1;
  // desc.ArraySize = 1;
  // desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  // desc.SampleDesc.Count = 1;
  // desc.Usage = D3D11_USAGE_DEFAULT;
  // desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
  // ID3D11Texture2D* d3d11Texture = nullptr;
  // hr = d3d11Device->CreateTexture2D(&desc, nullptr, &d3d11Texture);
  // THWOW_ON_ERROR(hr, "Failed to create D3D11 texture");
  
  SPtr<GTextureElement> textDesc = makeSharedPtr<GTextureElement>();
  textDesc->width = 512;
  textDesc->height = 512;
  textDesc->mipLevels = 1;
  textDesc->arraySize = 1;
  textDesc->format = COLORFORMAT::E::RGBA_8_UNORM;
  textDesc->sampleCount = 1;
  textDesc->usage = GRESOURCE_USAGE::E::eDEFAULT;
  textDesc->bindFlags = GRESOURCE_BINDINGS::E::SHADERRESOURCE | GRESOURCE_BINDINGS::E::UNORDEREDACCESS;

  auto texture = GraphicsAPI::instance().createTexture2D(textDesc);
  ID3D11Texture2D* d3d11Texture = reinterpret_cast<ID3D11Texture2D*>(texture->get());

  cl_mem d3d11Image = clCreateFromD3D11Texture2DNV(
    context,
    CL_MEM_READ_WRITE,
    d3d11Texture,
    0,
    &err
  );

  d3d11Context->Flush();

  cl_mem memObjects[] = { d3d11Image };
  clEnqueueAcquireD3D11ObjectsNV(
    queue,
    1,
    memObjects,
    0,
    nullptr,
    nullptr
  );

  std::fstream kernelFile("kernel.cl", std::ios::in | std::ios::ate);
  if (!kernelFile.is_open()) {
    THWOW_ON_ERROR(-1, "Failed to open kernel.cl file");
  }

  size_t kernelFileSize = static_cast<size_t>(kernelFile.tellg());
  kernelFile.seekg(0, std::ios::beg);
  Vector<char> kernelFileContent(kernelFileSize, 0);
  kernelFile.read(kernelFileContent.data(), kernelFileSize);
  kernelFile.close();

  const char* pFileData = kernelFileContent.data();

  // Create and build the OpenCL program
  cl_program program = clCreateProgramWithSource(context, 1, &pFileData, nullptr, &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL program");

  // Build
  err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

  if (err == CL_BUILD_PROGRAM_FAILURE) {
    size_t logSize = 0;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
    Vector<char> buildLog(logSize);
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);
    std::cerr << "Build Log:\n" << buildLog.data() << std::endl;
  }

  THWOW_ON_ERROR(err, "Failed to build OpenCL program");

  // Create the OpenCL kernel
  cl_kernel kernelVectorAdd = clCreateKernel(program, "vectorAdd", &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL kernel: vectorAdd");

  cl_kernel kernelMatrixAdd = clCreateKernel(program, "matrixAdd", &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL kernel: matrixAdd");

  cl_kernel kernelHistogram2D = clCreateKernel(program, "histogram_kernel_2D", &err);
  THWOW_ON_ERROR(err, "Failed to create OpenCL kernel: histogram2D");

  // Prepare data
  const unsigned int numElements = 1024;
  Vector<float> h_A(numElements, 1.0f);
  Vector<float> h_B(numElements, 2.0f);
  Vector<float> h_C(numElements, 0.0f);

  // Create buffers
  cl_mem d_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * numElements, h_A.data(), &err);
  THWOW_ON_ERROR(err, "Failed to create buffer for A");
  cl_mem d_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * numElements, h_B.data(), &err);
  THWOW_ON_ERROR(err, "Failed to create buffer for B");
  cl_mem d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * numElements, nullptr, &err);
  THWOW_ON_ERROR(err, "Failed to create buffer for C");

  // Configure kernel arguments vectorAdd
  err = clSetKernelArg(kernelVectorAdd, 0, sizeof(cl_mem), &d_A);
  err |= clSetKernelArg(kernelVectorAdd, 1, sizeof(cl_mem), &d_B);
  err |= clSetKernelArg(kernelVectorAdd, 2, sizeof(cl_mem), &d_C);
  err |= clSetKernelArg(kernelVectorAdd, 3, sizeof(unsigned int), &numElements);
  THWOW_ON_ERROR(err, "Failed to set kernel arguments: vectorAdd");

  // Launch the kernel vectorAdd
  size_t globalWorkSize = numElements;
  clEnqueueNDRangeKernel(queue, kernelVectorAdd, 1, nullptr, &globalWorkSize, nullptr, 0, nullptr, nullptr);

  // Configure kernel arguments matrixAdd

  unsigned int width = 32;
  unsigned int height = 32;
  err = clSetKernelArg(kernelMatrixAdd, 0, sizeof(cl_mem), &d_A);
  err |= clSetKernelArg(kernelMatrixAdd, 1, sizeof(cl_mem), &d_B);
  err |= clSetKernelArg(kernelMatrixAdd, 2, sizeof(cl_mem), &d_C);
  err |= clSetKernelArg(kernelMatrixAdd, 3, sizeof(unsigned int), &width);
  err |= clSetKernelArg(kernelMatrixAdd, 4, sizeof(unsigned int), &height);
  THWOW_ON_ERROR(err, "Failed to set kernel arguments: vectorMatrix");

  size_t globalWorkSizeMatrix[2] = { (size_t)height, (size_t)width };
  clEnqueueNDRangeKernel(queue, kernelMatrixAdd, 2, nullptr, globalWorkSizeMatrix, nullptr, 0, nullptr, nullptr);

  // local(16,16)
  // Global ceil(width/16)*16, ceil(height/16)*16
  size_t local2D[2] = { 16, 16 };
  size_t global2D[2] = { (size_t)((width + 15) / 16) * 16, (size_t)((height + 15) / 16) * 16 };
  clEnqueueNDRangeKernel(queue, kernelHistogram2D, 2, nullptr, global2D, local2D, 0, nullptr, nullptr);

  clFinish(queue);

  // Read back the results
  clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, sizeof(float) * numElements, h_C.data(), 0, nullptr, nullptr);

  bool success = true;
  for (unsigned int i = 0; i < numElements; ++i) {
    if (h_C[i] != h_A[i] + h_B[i]) {
      success = false;
      std::cerr << "Mismatch at index " << i << ": " << h_C[i] << " != " << h_A[i] + h_B[i] << std::endl;
      break;
    }
  }

  if (success) {
    std::cout << "Vector addition successful!" << std::endl;
  }
  else {
    std::cout << "Vector addition failed!" << std::endl;
  }


  // Clean up
  clReleaseMemObject(d_A);
  clReleaseMemObject(d_B);
  clReleaseMemObject(d_C);
  clReleaseKernel(kernelVectorAdd);
  clReleaseKernel(kernelMatrixAdd);
  clReleaseKernel(kernelHistogram2D);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
  clReleaseDevice(device);

  return 0;
}




void histogram(const unsigned char* colorChannel,
  const size_t numPixels,
  unsigned int* numLevels) {
  // memset(&numLevels, 0, sizeof(unsigned int) * 256);

  for (size_t i = 0; i < numPixels; ++i) {
    numLevels[colorChannel[i]] += 1;
  }

}

