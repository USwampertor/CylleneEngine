#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGDepthStencilView.h"

namespace CYLLENE_SDK
{
struct AdapterElement
{
	String description;
  uint32 vendorId;
  uint32 deviceId;
  uint32 subsysId;
  uint32 revision;
  uint32 dedicatedVideoMemory;
  uint32 dedicatedSystemMemory;
  uint32 sharedSystemMemory;
  uint32 outputCount;
  uint32 outputId;
  uint32 outputWidth;
  uint32 outputHeight;
  uint32 outputRefreshRate;
  uint32 outputFormat;
  uint32 outputColorDepth;
	String lowpart;
	float highpart;
	uint32 flags;
};

struct GDeviceElement
{
  void* windowHandle;
  uint32 width;
  uint32 height;
  uint32 bufferCount;
  bool enableVSync;
};

class GDevice
{
public:

  GDevice() = default;

  ~GDevice() = default;

  virtual void
  queryInterface() = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView() = 0;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView() = 0;

};
}
