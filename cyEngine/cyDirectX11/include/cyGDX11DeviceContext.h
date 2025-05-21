#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGGraphic.h>
#include <cyGDeviceContext.h>

#include <d3d11_2.h>

namespace CYLLENE_SDK
{
class CY_DX11_EXPORT GDX11DeviceContext : public GDeviceContext
{
public:
  GDX11DeviceContext() = default;

  virtual ~GDX11DeviceContext() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  virtual void
  queryInterface() override;

  virtual void
  setViewPort(uint32 numViewports, const GViewport& viewport) override;

  virtual void
  updateSubresource(SPtr<GGraphic> resource, const GSubResourceElement& data) override;

  virtual void
  setRenderTargets(uint32 numRenderTargets,
                   Vector<SPtr<GRenderTargetView>> renderTargets,
                   SPtr<GDepthStencilView> depthStencil) override;

  virtual void
  clearRenderTargetView(SPtr<GRenderTargetView> renderTarget,
                        const Color& color) override;

  virtual void 
  clearDepthStencilView(SPtr<GDepthStencilView> depthStencil,
                        uint32 clearFlags,
                        float depth, 
                        uint8 stencil) override;

  virtual void
  setVertexShader(SPtr<GVertexShader> shader, 
                  void* classInstance = nullptr, 
                  uint32 numClasses = 0) override;

  virtual void
  setPixelShader(SPtr<GPixelShader> shader, 
                 void* classInstance = nullptr, 
                 uint32 numClasses = 0) override;

  virtual void
  setInputLayout(SPtr<GInputLayout> layout) override;

  virtual void
  setPrimitiveTopology(uint32 topology) override;

  virtual void
  setVertexBuffers(uint32 startSlot, 
                   uint32 numBuffers, 
                   Vector<SPtr<GraphicsBuffer>> buffers,
                   Vector<uint32> strides,
                   Vector<uint32> offsets) override;

  virtual void
  setIndexBuffer(SPtr<GraphicsBuffer> buffer,
                 uint32 format,
                 uint32 offset) override;

  virtual void
  setConstantBuffer(uint32 slot,
                    uint32 numBuffers,
                    Vector<SPtr<GraphicsBuffer>> buffers) override;

  virtual void
  setShaderResource(SPtr<GShaderResourceView> resource,
                    uint32 slot,
                    uint32 numViews) override;

  virtual void
  setSamplers(uint32 slot,
              uint32 numSamplers,
              Vector<SPtr<GSamplerState>> samplers) override;

  virtual void
  drawIndexed(SPtr<GMesh> mesh) override;

public:

  ID3D11DeviceContext1* m_pDeviceContext = nullptr;

};
}
