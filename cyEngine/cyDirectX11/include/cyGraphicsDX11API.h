#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGraphicsAPI.h>

namespace CYLLENE_SDK {


static enum DXGI_FORMAT
colorFormatToDXGI(uint32_t fmt) {
  switch (COLORFORMAT::E::_from_integral_unchecked(fmt)) {
  case COLORFORMAT::E::RGBA_32_FLOAT:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
  case COLORFORMAT::E::RGB_32_FLOAT:      return DXGI_FORMAT_R32G32B32_FLOAT;
  case COLORFORMAT::E::RG_32_FLOAT:       return DXGI_FORMAT_R32G32_FLOAT;
  case COLORFORMAT::E::R_32_FLOAT:        return DXGI_FORMAT_R32_FLOAT;
  case COLORFORMAT::E::D_32_FLOAT:        return DXGI_FORMAT_D32_FLOAT;
  case COLORFORMAT::E::RGBA_32_UINT:      return DXGI_FORMAT_R32G32B32A32_UINT;
  case COLORFORMAT::E::RGB_32_UINT:       return DXGI_FORMAT_R32G32B32_UINT;
  case COLORFORMAT::E::RG_32_UINT:        return DXGI_FORMAT_R32G32_UINT;
  case COLORFORMAT::E::R_32_UINT:         return DXGI_FORMAT_R32_UINT;
  case COLORFORMAT::E::D_32_UINT:         return DXGI_FORMAT_R32_UINT; // There is no D_32_UINT in DXGI, using R32_UINT instead
  case COLORFORMAT::E::RGBA_32_SINT:      return DXGI_FORMAT_R32G32B32A32_SINT;
  case COLORFORMAT::E::RGB_32_SINT:       return DXGI_FORMAT_R32G32B32_SINT;
  case COLORFORMAT::E::RG_32_SINT:        return DXGI_FORMAT_R32G32_SINT;
  case COLORFORMAT::E::R_32_SINT:         return DXGI_FORMAT_R32_SINT;
  case COLORFORMAT::E::D_32_SINT:         return DXGI_FORMAT_R32_SINT; // There is no D_32_SINT in DXGI, using R32_SINT instead
  case COLORFORMAT::E::RGBA_32_TYPELESS:  return DXGI_FORMAT_R32G32B32A32_TYPELESS;
  case COLORFORMAT::E::RGB_32_TYPELESS:   return DXGI_FORMAT_R32G32B32_TYPELESS;
  case COLORFORMAT::E::RG_32_TYPELESS:    return DXGI_FORMAT_R32G32_TYPELESS;
  case COLORFORMAT::E::R_32_TYPELESS:     return DXGI_FORMAT_R32_TYPELESS;
  case COLORFORMAT::E::D_32_TYPELESS:     return DXGI_FORMAT_R32_TYPELESS; // There is no D_32_TYPELESS in DXGI, using R32_TYPELESS instead
  case COLORFORMAT::E::RGBA_8_FLOAT:      return DXGI_FORMAT_R8G8B8A8_TYPELESS; // There is no RGBA_8_FLOAT in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::RGB_8_FLOAT:       return DXGI_FORMAT_R8G8B8A8_TYPELESS; // There is no RGB_8_FLOAT in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::RG_8_FLOAT:        return DXGI_FORMAT_R8G8_TYPELESS; // There is no RG_8_FLOAT in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::R_8_FLOAT:         return DXGI_FORMAT_R8_TYPELESS; // There is no R_8_FLOAT in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::D_8_FLOAT:         return DXGI_FORMAT_R8_TYPELESS; // There is no D_8_FLOAT in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::RGBA_8_UINT:       return DXGI_FORMAT_R8G8B8A8_UINT;
  case COLORFORMAT::E::RGB_8_UINT:        return DXGI_FORMAT_R8G8B8A8_UINT; // There is no RGB_8_UINT in DXGI, using R8G8B8A8_UINT instead
  case COLORFORMAT::E::RG_8_UINT:         return DXGI_FORMAT_R8G8_UINT;
  case COLORFORMAT::E::R_8_UINT:          return DXGI_FORMAT_R8_UINT;
  case COLORFORMAT::E::D_8_UINT:          return DXGI_FORMAT_R8_UINT; // There is no D_8_UINT in DXGI, using R8_UINT instead
  case COLORFORMAT::E::RGBA_8_SINT:       return DXGI_FORMAT_R8G8B8A8_SINT;
  case COLORFORMAT::E::RGB_8_SINT:        return DXGI_FORMAT_R8G8B8A8_SINT; // There is no RGB_8_SINT in DXGI, using R8G8B8A8_SINT instead
  case COLORFORMAT::E::RG_8_SINT:         return DXGI_FORMAT_R8G8_SINT;
  case COLORFORMAT::E::R_8_SINT:          return DXGI_FORMAT_R8_SINT;
  case COLORFORMAT::E::D_8_SINT:          return DXGI_FORMAT_R8_SINT; // There is no D_8_SINT in DXGI, using R8_SINT instead
  case COLORFORMAT::E::RGBA_8_TYPELESS:   return DXGI_FORMAT_R8G8B8A8_TYPELESS;
  case COLORFORMAT::E::RGB_8_TYPELESS:    return DXGI_FORMAT_R8G8B8A8_TYPELESS; // There is no RGB_8_TYPELESS in DXGI, using R8G8B8A8_TYPELESS instead
  case COLORFORMAT::E::RG_8_TYPELESS:     return DXGI_FORMAT_R8G8_TYPELESS;
  case COLORFORMAT::E::R_8_TYPELESS:      return DXGI_FORMAT_R8_TYPELESS;
  case COLORFORMAT::E::D_8_TYPELESS:      return DXGI_FORMAT_R8_TYPELESS; // There is no D_8_TYPELESS in DXGI, using R8_TYPELESS instead
  case COLORFORMAT::E::RGBA_8_UNORM:      return DXGI_FORMAT_R8G8B8A8_UNORM;
  case COLORFORMAT::E::RGB_8_UNORM:       return DXGI_FORMAT_R8G8B8A8_UNORM; // There is no RGB_8_UNORM in DXGI, using R8G8B8A8_UNORM instead
  case COLORFORMAT::E::RG_8_UNORM:        return DXGI_FORMAT_R8G8_UNORM;
  case COLORFORMAT::E::R_8_UNORM:         return DXGI_FORMAT_R8_UNORM;
  case COLORFORMAT::E::D_8_UNORM:         return DXGI_FORMAT_R8_UNORM; // There is no D_8_UNORM in DXGI, using R8_UNORM instead
  case COLORFORMAT::E::D_24_UNORM_S8_UINT: return DXGI_FORMAT_D24_UNORM_S8_UINT;
  case COLORFORMAT::E::BGRA_8_UNORM:       return DXGI_FORMAT_B8G8R8A8_UNORM;
  default:                return DXGI_FORMAT_UNKNOWN;
  }
}

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
  createRenderTargetView(SPtr<GRenderTargetViewElement> srvParams,
												 SPtr<GTexture> texture = nullptr) override;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GDepthStencilViewElement> dsvParams,
												 SPtr<GTexture> texture = nullptr) override;

	virtual SPtr<GSwapChain>
	createSwapChain(SPtr<GDevice> device, 
									SPtr<GSwapChainElement> swapChainParams) override;

  virtual void // SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
													 SPtr<GShaderResourceViewElement> srvParams) override;

	virtual SPtr<GMesh>
	createMesh(SPtr<RMesh> mesh) override;

	//////////////////////////////////////////////////////////////////////////
	// NEEDS REVISIONING
	//////////////////////////////////////////////////////////////////////////


	virtual SPtr<GTexture>
	createTexture2D(SPtr<RTexture> texture) override;

	// virtual SPtr<GTexture>
  // createTexture2D(SPtr<RTexture> texture,
	// 								uint32 bindFlags, 
	// 								uint32 cpuAccessFlags = 0,
	// 								uint32 mipFlags = 1,
	// 								// SPtr<GShaderResourceView> ppSRV = nullptr,
	// 								SPtr<GRenderTargetView> ppRTV = nullptr,
	// 								SPtr<GDepthStencilView> ppDSV = nullptr) override;
	// 
	// virtual SPtr<GTexture>
  // createTexture2D(const Vector2i& size,
	// 								uint32 bindFlags,
	// 								uint32 format,
	// 								uint32 usage,		
	// 								uint32 cpuAccessFlags = 0,
	// 								uint32 mipFlags = 1,
	// 								// SPtr<GShaderResourceView> ppSRV = nullptr,
	// 								SPtr<GRenderTargetView> ppRTV = nullptr,
	// 								SPtr<GDepthStencilView> ppDSV = nullptr) override;

	virtual SPtr<GTexture>
	createTexture2D(SPtr<GTextureElement> textureParams) override; //,
									// SPtr<GShaderResourceView> ppSRV = nullptr,
									// SPtr<GRenderTargetView>		ppRTV = nullptr,
									// SPtr<GDepthStencilView>		ppDSV = nullptr) override;

	//////////////////////////////////////////////////////////////////////////
	// ENDS NEEDS REVISIONING
	//////////////////////////////////////////////////////////////////////////

	virtual SPtr<GShaderBlob>
	compileShader(const String& data, const String& entry, const String& model) override;

	virtual SPtr<GVertexShader>
	createVertexShader(SPtr<RShader> shader, const String& entry) override;

  virtual SPtr<GPixelShader>
	createPixelShader(SPtr<RShader> shader, const String& entry) override;
	
  virtual SPtr<GGeometryShader>
	createGeometryShader(SPtr<RShader> shader, const String& entry) override;

	virtual SPtr<GComputeShader>
	createComputeShader(SPtr<RShader> shader, const String& entry) override;

	virtual SPtr<GShader>
	createShader(SPtr<RShader> shader, const String& entry) override;

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
	draw(SPtr<CCamera> refCamera, SPtr<BBeing> redObject) override;

	virtual void
	present() override;

	virtual void
	setViewport(int32 x, int32 y, int32 width, int32 height) override;
	
	virtual void
	setViewport(const Rect& size) override;

};



}

