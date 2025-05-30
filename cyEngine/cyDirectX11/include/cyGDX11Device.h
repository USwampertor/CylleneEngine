#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGDevice.h>

namespace CYLLENE_SDK 
{
class CY_DX11_EXPORT GDX11Device : public GDevice
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
                 SPtr<GDepthStencilView> depthStencil, 
                 SPtr<GRenderTargetView> renderTargetView, 
                 int32 width, 
                 int32 height) override;

  virtual void
  queryInterface(SPtr<GSwapChain> swapChain,
                 SPtr<GDepthStencilView> depthStencil,
                 SPtr<GRenderTargetView> renderTargetView,
                 Vector2i size) override;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GDepthStencilViewElement> dsvParams,
                         SPtr<GTexture> shaderResourceView = nullptr) override;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(// SPtr<GTexture> renderTargetView, 
                         SPtr<GRenderTargetViewElement> rtvParams,
                         SPtr<GTexture> texture = nullptr) override;

  virtual void// SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
                           SPtr<GShaderResourceViewElement> srvParams) override;

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputLayoutElement>& descriptor,
                    SPtr<GVertexShader> desc) override;

  virtual SPtr<GVertexShader>
  createVertexShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GPixelShader>
  createPixelShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GGeometryShader>
  createGeometryShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GComputeShader>
  createComputeShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GShader>
  createShader(SPtr<GShaderBlob> blob) override;

  virtual SPtr<GraphicsBuffer>
  createGraphicsBuffer(SPtr<GBufferElement> bufferParams) override;

  virtual SPtr<GRasterizerState>
  createRasterizerState(SPtr<GRasterizerElement> rasterizerParams) override;

  virtual SPtr<GBlendState>
  createBlendState(SPtr<GBlendElement> blendParams) override;

  virtual SPtr<GSamplerState>
  createSamplerState(SPtr<GSamplerStateElement> samplerParams) override; 

  virtual SPtr<GTexture>
  createTexture2D(SPtr<GTextureElement> textureParams) override;

public:

  ID3D11Device1* m_pd3d11Device = nullptr;

};
}

