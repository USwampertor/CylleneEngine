#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGraphicsAPI.h>

namespace CYLLENE_SDK {

class CY_DX11_EXPORT GraphicsDX11API : public GraphicsAPI
{
public:
  GraphicsDX11API() = default;
  
  virtual ~GraphicsDX11API() override;
  
	virtual GFXTYPE::E
	getType() const { return GFXTYPE::E::eDX11; }

  virtual void
  initialize(void* pHandle) override;

  virtual void
  shutdown() override;

//   virtual SPtr<GDevice>
// 	createDevice(const GDeviceElement& deviceParams) override;
// 
//   virtual SPtr<GDeviceContext>
//   createDeviceContext() override;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(SPtr<GTexture> shaderResourceView,
												 SPtr<GRenderTargetViewElement> srvParams) override;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GTexture> depthStencilView,
												 SPtr<GDepthStencilViewElement> dsvParams) override;

	virtual SPtr<GSwapChain>
	createSwapChain(SPtr<GDevice> device, 
									SPtr<GSwapChainElement> swapChainParams) override;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
													 SPtr<GShaderResourceViewElement> srvParams) override;

	virtual SPtr<GTexture>
  createTexture2D(SPtr<RTexture> texture,
									uint32 bindFlags, 
									uint32 cpuAccessFlags = 0,
									uint32 mipFlags = 1,
									SPtr<GShaderResourceView> ppSRV = nullptr,
									SPtr<GRenderTargetView> ppRTV = nullptr,
									SPtr<GDepthStencilView> ppDSV = nullptr) override;

	virtual SPtr<GTexture>
  createTexture2D(const Vector2i& size,
									uint32 bindFlags,
									uint32 format,
									uint32 usage,		
									uint32 cpuAccessFlags = 0,
									uint32 mipFlags = 1,
									SPtr<GShaderResourceView> ppSRV = nullptr,
									SPtr<GRenderTargetView> ppRTV = nullptr,
									SPtr<GDepthStencilView> ppDSV = nullptr) override;

	virtual SPtr<GTexture>
	createTexture2D(SPtr<GTextureElement> textureParams,
									SPtr<GShaderResourceView> ppSRV = nullptr,
									SPtr<GRenderTargetView>		ppRTV = nullptr,
									SPtr<GDepthStencilView>		ppDSV = nullptr) override;

	virtual SPtr<GShaderBlob>
	compileShader(const String& data, const String& entry, const String& model) override;

	virtual SPtr<GVertexShader>
	createVertexShader(SPtr<RShader> shader, const String& entry) override;

  virtual SPtr<GPixelShader>
	createPixelShader(SPtr<RShader> shader, const String& entry) override;
	
  virtual SPtr<GGeometryShader>
	createGeometryShader(SPtr<RShader> shader, const String& entry) override;

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputLayoutElement>& descriptor,
										SPtr<GVertexShader> desc) override;

  virtual SPtr<GraphicsBuffer>
	createVertexBuffer(const Vector<char>& data) override;
  
	virtual SPtr<GraphicsBuffer>
	createIndexBuffer(const Vector<char>& data) override;

  virtual SPtr<GraphicsBuffer>
  createConstantBuffer(const Vector<char>& data) override;

	virtual void
  writeToBuffer(const SPtr<GraphicsBuffer>& buffer, 
								const Vector<char>& data) override;

	virtual void
	queryInterface(int32 width, int32 height) override;

	virtual void
	queryInterface(const Vector2i& size) override;

	virtual void
	clear(const Color& color) override;

	virtual void
	present() override;

	virtual void
	setViewport(int32 x, int32 y, int32 width, int32 height) override;
	
	virtual void
	setViewport(const Rect& size) override;

};



}

