#pragma once
#include "cyGraphicsPrerequisites.h"

#include "cyGraphic.h"
#include "cyGDepthStencilView.h"
#include "cyGInputLayout.h"
#include "cyGraphicsBuffer.h"
#include "cyGRasterizerState.h"
#include "cyGRenderTargetView.h"
#include "cyGSamplerState.h"
#include "cyGShader.h"
#include "cyGShaderResourceView.h"
#include "cyGSwapChain.h"
#include "cyGTexture.h"

#include <cyVector2i.h>

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

class CY_GRAPHICS_EXPORT GDevice : public Graphic
{
public:

  GDevice() = default;

  virtual ~GDevice() {}

  virtual void
  queryInterface(SPtr<GSwapChain> swapChain, 
                 SPtr<GDepthStencilView> depthStencil, 
                 SPtr<GRenderTargetView> renderTargetView, 
                 int32 width, 
                 int32 height) = 0;

  virtual void
  queryInterface(SPtr<GSwapChain> swapChain,
                 SPtr<GDepthStencilView> depthStencil,
                 SPtr<GRenderTargetView> renderTargetView,
                 Vector2i size) = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GTexture> depthStencilView,
                         SPtr<GDepthStencilViewElement> dsvParams) = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(SPtr<GTexture> renderTargetView, 
                         SPtr<GRenderTargetViewElement> rtvParams) = 0;

  virtual void// SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
                           SPtr<GShaderResourceViewElement> srvParams) = 0;

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputLayoutElement>& descriptor,
                    SPtr<GVertexShader> desc) = 0;

  virtual SPtr<GVertexShader>
  createVertexShader(SPtr<GShaderBlob> blob) = 0;

  virtual SPtr<GPixelShader>
  createPixelShader(SPtr<GShaderBlob> blob) = 0;

  virtual SPtr<GraphicsBuffer>
  createGraphicsBuffer(SPtr<GBufferElement> bufferParams) = 0;

  virtual SPtr<GRasterizerState>
  createRasterizerState(SPtr<GRasterizerElement> rasterizerParams) = 0;

  virtual SPtr<GTexture>
  createTexture2D(SPtr<GTextureElement> textureParams) = 0;

};
}
