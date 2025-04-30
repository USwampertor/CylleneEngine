#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGDevice.h>

#include <d3d11_2.h>

namespace CYLLENE_SDK 
{
class GDX11Device : public GDevice
{
public:

  GDX11Device() = default;

  virtual ~GDX11Device() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  virtual void
  queryInterface(SPtr<GSwapChain> swapChain,
                 SPtr<GDepthStencilView> backBuffer,
                 int32 width, 
                 int32 height) override;

  virtual void
  queryInterface(SPtr<GSwapChain> swapChain,
                 SPtr<GDepthStencilView> backBuffer,
                 Vector2i size) override;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GTexture> depthStencilView,
                         const GDepthStencilViewElement& dsvParams) override;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(SPtr<GTexture> renderTargetView, 
                         const GRenderTargetViewElement& rtvParams) override;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
                           const GShaderResourceViewElement& srvParams) override;

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputLayoutElement>& descriptor,
                    SPtr<GVertexShader> desc) override;

  virtual SPtr<GVertexShader>
  createVertexShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GPixelShader>
  createPixelShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GraphicsBuffer>
  createGraphicsBuffer(const GBufferElement& bufferElement) override;

  virtual SPtr<GRasterizerState>
  createRasterizerState(const GRasterizerElement& rasterizerElement) override;

public:

  ID3D11Device* m_pDevice = nullptr;

};
}

