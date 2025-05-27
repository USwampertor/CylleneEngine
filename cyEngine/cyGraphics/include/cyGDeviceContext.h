#pragma once
#include "cyGraphicsPrerequisites.h"

#include "cyGGraphic.h"
#include "cyGDepthStencilView.h"
#include "cyGInputLayout.h"
#include "cyGraphicsBuffer.h"
#include "cyGRenderTargetView.h"
#include "cyGSamplerState.h"
#include "cyGShader.h"
#include "cyGShaderResourceView.h"
#include "cyGViewport.h"
#include "cyGMesh.h"

#include <cyColor.h>

namespace CYLLENE_SDK
{

struct GSubResourceElement
{
  int32 index;
  void* data;
  int32 pitch;
  int32 depth;
};


class CY_GRAPHICS_EXPORT GDeviceContext : public GGraphic
{
public:

  GDeviceContext() = default;

  virtual ~GDeviceContext() {}

  virtual void
  queryInterface() = 0;

  virtual void
  setViewPort(uint32 numViewports, const GViewport& viewport) = 0;

  virtual void
  updateSubresource(SPtr<GGraphic> resource, const GSubResourceElement& data) = 0;

  virtual void
  setRenderTargets(uint32 numRenderTargets,
                   Vector<SPtr<GRenderTargetView>> renderTargets,
                   SPtr<GDepthStencilView> depthStencil) = 0;

  virtual void
  clearRenderTargetView(SPtr<GRenderTargetView> renderTarget,
                        const Color& color) = 0;

  virtual void 
  clearDepthStencilView(SPtr<GDepthStencilView> depthStencil,
                        uint32 clearFlags,
                        float depth, 
                        uint8 stencil) = 0;

  virtual void
  setVertexShader(SPtr<GVertexShader> shader, 
                  void* classInstance = nullptr, 
                  uint32 numClasses = 0) = 0;

  virtual void
  setPixelShader(SPtr<GPixelShader> shader, 
                 void* classInstance = nullptr, 
                 uint32 numClasses = 0) = 0;

  virtual void
  setGeometryShader(SPtr<GGeometryShader> shader, 
                    void* classInstance = nullptr, 
                    uint32 numClasses = 0) = 0;

  virtual void
  setComputeShader(SPtr<GComputeShader> shader, 
                   void* classInstance = nullptr, 
                   uint32 numClasses = 0) = 0;

  virtual void
  setShader(SPtr<GShader> shader, 
            void* classInstance = nullptr, 
            uint32 numClasses = 0) = 0;

  virtual void
  setInputLayout(SPtr<GInputLayout> layout) = 0;

  virtual void
  setPrimitiveTopology(uint32 topology) = 0;

  virtual void
  setVertexBuffers(uint32 startSlot, 
                   uint32 numBuffers, 
                   Vector<SPtr<GraphicsBuffer>> buffers,
                   Vector<uint32> strides,
                   Vector<uint32> offsets) = 0;

  virtual void
  setIndexBuffer(SPtr<GraphicsBuffer> buffer,
                 uint32 format,
                 uint32 offset) = 0;

  virtual void
  setConstantBuffer(uint32 slot,
                    uint32 numBuffers,
                    Vector<SPtr<GraphicsBuffer>> buffers) = 0;

  virtual void
  setShaderResources(Vector<SPtr<GShaderResourceView>> resource,
                    uint32 slot,
                    uint32 numViews) = 0;

  virtual void
  setSamplers(uint32 slot,
              uint32 numSamplers,
              Vector<SPtr<GSamplerState>> samplers) = 0;

  virtual void
  drawIndexed(SPtr<GMesh> mesh) = 0;

};
}
