#pragma once

#include "cyGraphicsPrerequisites.h"
#include <cyColor.h>
#include <cyModule.h>
#include <cyShader.h>
#include <cyTexture.h>
#include <cyVector2i.h>

#include "cyGraphicsBuffer.h"
#include "cyGDepthStencilView.h"
#include "cyGDevice.h"
#include "cyGDeviceContext.h"
#include "cyGInputLayout.h"
#include "cyGRenderTargetView.h"
#include "cyGShader.h"
#include "cyGShaderResourceView.h"
#include "cyGSwapChain.h"
#include "cyGTexture.h"

namespace CYLLENE_SDK {

	namespace GFXTYPE
	{
	BETTER_ENUM(E, uint32, 
							eUNKNOWN = -1,
							eNONE = 0,
							eDX09 = 1,
							eDX10 = 2,
							eDX11 = 3,
							eDX12 = 4,
							eOGL2 = 5,
							eOGL3 = 6,
							eVLKN = 7
	);
	}



class CY_GRAPHICS_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{
public:
  GraphicsAPI() = default;

	virtual void
  initialize(void* pHandle) = 0;
  
	virtual void
  shutdown() = 0;

	virtual ~GraphicsAPI() = default;

	void
	addToRenderPool(uint32 index);

	void
	removeFromRenderPool(uint32 index);

	void 
	registerToRenderPool(uint32 index);

	virtual SPtr<GDevice>
	createDevice(const GDeviceElement& deviceParams) = 0;

  virtual SPtr<GDeviceContext>
  createDeviceContext() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView() = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView() = 0;

	virtual SPtr<GSwapChain>
	createSwapChain(const SPtr<GDevice>& device, 
									const GSwapChainElement& swapChainParams) = 0;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView() = 0;

	virtual SPtr<GTexture>
  createTexture2D(SPtr<TextureResource> texture,
									uint32 bindFlags, 
									uint32 cpuAccessFlags = 0,
									uint32 mipFlags = 1,
									GShaderResourceView* ppSRV = nullptr,
									GRenderTargetView* ppRTV = nullptr,
									GDepthStencilView* ppDSV = nullptr) = 0;

	virtual SPtr<GTexture>
  createTexture2D(const Vector2i& size,
									uint32 bindFlags, 
									uint32 cpuAccessFlags = 0,
									uint32 mipFlags = 1,
									GShaderResourceView* ppSRV = nullptr,
									GRenderTargetView* ppRTV = nullptr,
									GDepthStencilView* ppDSV = nullptr) = 0;

	virtual SPtr<GShaderBlob>
	compileShader(const String& data, const String& entry, const String& model) = 0;

	virtual SPtr<GVertexShader>
	createVertexShader(SPtr<ShaderResource> shader, const String& entry) = 0;

  virtual SPtr<GPixelShader>
	createPixelShader(SPtr<ShaderResource> shader, const String& entry) = 0;
	
  virtual SPtr<GGeometryShader>
	createGeometryShader(SPtr<ShaderResource> shader, const String& entry) = 0;

  virtual SPtr<GInputLayout>
  createInputLayout(const Vector<GInputLayoutElement>& descriptor,
										SPtr<GVertexShader> desc) = 0;

  virtual SPtr<GraphicsBuffer>
	createVertexBuffer(const Vector<char>& data) = 0;
  
	virtual SPtr<GraphicsBuffer>
	createIndexBuffer(const Vector<char>& data) = 0;

  virtual SPtr<GraphicsBuffer>
  createConstantBuffer(const Vector<char>& data) = 0;

	virtual void
  writeToBuffer(const SPtr<GraphicsBuffer>& buffer, 
								const Vector<char>& data) = 0;

	virtual void
	queryInterface(int32 width, int32 height) = 0;

	virtual void
	queryInterface(const Vector2i& size) = 0;

	virtual void
	clear(const Color& color) = 0;

	virtual void
	present() = 0;

	virtual void
	setViewport(int32 x, int32 y, int32 width, int32 height) = 0;

	

public:

	SPtr<void*>
	m_pWHandle = nullptr;

  SPtr<GDevice> 
	m_pDevice = nullptr;

	SPtr<GDeviceContext> 
	m_pDeviceContext = nullptr;
	
	SPtr<GSwapChain>
	m_pSwapChain = nullptr;
  
	SPtr<GRenderTargetView> 
	m_pRenderTargetView = nullptr;
	
	SPtr<GDepthStencilView> 
	m_pDepthStencilView = nullptr;


};


}

