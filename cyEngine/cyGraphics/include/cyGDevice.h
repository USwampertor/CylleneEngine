#pragma once
#include "cyGraphicsPrerequisites.h"

#include "cyGDepthStencilView.h"
#include "cyGInputLayout.h"
#include "cyGraphicsBuffer.h"
#include "cyGRasterizerState.h"
#include "cyGRenderTargetView.h"
#include "cyGSamplerState.h"
#include "cyGShader.h"
#include "cyGShaderResourceView.h"
#include "cyGSwapChain.h"

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
  AdapterElement adapter;
  uint32 width;
  uint32 height;
  uint32 bufferCount;
  uint32 featureLevel;
  bool enableDebugLayer;
  bool enableGPUValidation;
  bool enableVSync;
  bool emableSingleThread;
  bool enableContextSharing;
  uint32 colorFormat;
  uint32 depthFormat;
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

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputElement>& descriptor,
                    SPtr<GVertexShader> desc) = 0;

  virtual SPtr<GVertexShader>
  createVertexShader(SPtr<GShaderBlob> blob) = 0;

  virtual SPtr<GPixelShader>
  createPixelShader(SPtr<GShaderBlob> blob) = 0;

  virtual SPtr<GraphicsBuffer>
  createGraphicsBuffer(const GBufferElement& bufferElement) = 0;

  virtual SPtr<GRasterizerState>
  createRasterizerState(const GRasterizerElement& rasterizerElement) = 0;


};
}
