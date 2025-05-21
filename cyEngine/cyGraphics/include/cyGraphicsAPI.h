#pragma once

#include "cyGraphicsPrerequisites.h"

#include <cyColor.h>
#include <cyModule.h>
#include <cyRShader.h>
#include <cyRTexture.h>
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
#include "cyGMesh.h"
#include "cyRMesh.h"

namespace CYLLENE_SDK {

	namespace GFXTYPE
	{
	BETTER_ENUM(E, int32, 
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

	virtual ~GraphicsAPI() {}
  
	virtual GFXTYPE::E
  getType() const { return GFXTYPE::E::eUNKNOWN; }

	virtual void
  initialize(void* pHandle) = 0;
  
	virtual void
  shutdown() = 0;

	void
	addToRenderPool(uint32 index);

	void
	removeFromRenderPool(uint32 index);

	void 
	registerToRenderPool(uint32 index);

// 	virtual SPtr<GDevice>
// 	createDevice(const GDeviceElement& deviceParams) = 0;
// 
//   virtual SPtr<GDeviceContext>
//   createDeviceContext() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(// SPtr<GTexture> shaderResourceView,
												 SPtr<GRenderTargetViewElement> srvParams) = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(// SPtr<GTexture> depthStencilView,
												 SPtr<GDepthStencilViewElement> dsvParams) = 0;

	virtual SPtr<GSwapChain>
	createSwapChain(SPtr<GDevice> device, 
									SPtr<GSwapChainElement> swapChainParams) = 0;

  virtual void // SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture>& shaderResourceView,
													 SPtr<GShaderResourceViewElement> srvParams) = 0;

	virtual SPtr<GMesh>
	createMesh(SPtr<RMesh> mesh) = 0;

	//////////////////////////////////////////////////////////////////////////
	// NEEDS REVISIONING
	//////////////////////////////////////////////////////////////////////////

	virtual SPtr<GTexture>
	createTexture2D(SPtr<RTexture> texture) = 0;
// 
// 	virtual SPtr<GTexture>
//   createTexture2D(SPtr<RTexture> texture,
// 									uint32 bindFlags,
// 									uint32 cpuAccessFlags = 0,
// 									uint32 mipFlags = 1,
// 									// SPtr<GShaderResourceView> ppSRV = nullptr,
// 									SPtr<GRenderTargetView>		ppRTV = nullptr,
// 									SPtr<GDepthStencilView>		ppDSV = nullptr) = 0;
// 	
// 	virtual SPtr<GTexture>
//   createTexture2D(const Vector2i& size,
// 									uint32 bindFlags,
// 									uint32 format,
// 									uint32 usage,		
// 									uint32 cpuAccessFlags = 0,
// 									uint32 mipFlags = 1,
// 									// SPtr<GShaderResourceView> ppSRV = nullptr,
// 									SPtr<GRenderTargetView>		ppRTV = nullptr,
// 									SPtr<GDepthStencilView>		ppDSV = nullptr) = 0;

  virtual SPtr<GTexture>
	createTexture2D(SPtr<GTextureElement> textureParams) = 0; //,
									// SPtr<GShaderResourceView> ppSRV = nullptr,
									// SPtr<GRenderTargetView>		ppRTV = nullptr,
									// SPtr<GDepthStencilView>		ppDSV = nullptr) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ENDS NEEDS REVISIONING
	//////////////////////////////////////////////////////////////////////////

	virtual SPtr<GShaderBlob>
	compileShader(const String& data, const String& entry, const String& model) = 0;

	virtual SPtr<GVertexShader>
	createVertexShader(SPtr<RShader> shader, const String& entry) = 0;

  virtual SPtr<GPixelShader>
	createPixelShader(SPtr<RShader> shader, const String& entry) = 0;
	
  virtual SPtr<GGeometryShader>
	createGeometryShader(SPtr<RShader> shader, const String& entry) = 0;

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

	virtual void
	setViewport(const Rect& rect) = 0;

  SPtr<GDevice>
  getDevice() const { return m_pDevice; }
  
	SPtr<GDeviceContext>
	getDeviceContext() const { return m_pDeviceContext; }

  SPtr<GSwapChain>
	getpSwapChain() const { return m_pSwapChain; }

  SPtr<GRenderTargetView>
	getRenderTargetView() const { return m_pRenderTargetView; }

  SPtr<GDepthStencilView>
	getDepthStencilView() const { return m_pDepthStencilView; }

  SPtr<GRenderTargetView>
  getRenderTargetView(uint32 index) const {
		if (index < m_pRTVs.size()) {
      return m_pRTVs[index];
		}
		else {
      return nullptr;
		}
  }

  SPtr<GDepthStencilView>
	getDepthStencilView(uint32 index) const {
		if (index < m_pDSVs.size()) {
      return m_pDSVs[index];
		}
		else {
      return nullptr;
		}
  }

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


  Vector<SPtr<GRenderTargetView>>
	m_pRTVs;

  Vector<SPtr<GDepthStencilView>>
  m_pDSVs;

	Vector<SPtr<GMesh>>
	m_renderPool;

};


}

