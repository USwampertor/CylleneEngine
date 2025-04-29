#pragma once

#include "cyGraphicsPrerequisites.h"
#include <cyModule.h>
#include <cyShader.h>
#include <cyTexture.h>
#include <cyColor.h>

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

class CY_GRAPHICS_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{
public:
  GraphicsAPI() = default;

  GraphicsAPI(void* pHandle);
  
	virtual ~GraphicsAPI() = default;

	void
	addToRenderPool(const uint32& index);

	void
	removeFromRenderPool(const uint32& index);

	void 
	registerToRenderPool(const uint32& index);

	virtual SPtr<GDevice>
	createDevice() = 0;

  virtual SPtr<GDeviceContext>
  createDeviceContext() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView() = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView() = 0;

	virtual SPtr<GSwapChain>
	createSwapChain() = 0;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView() = 0;

	virtual SPtr<GTexture>
  createTexture(SPtr<TextureResource> texture,
								uint32 bindFlags, 
								uint32 cpuAccessFlags = 0,
								uint32 mipFlags = 1,
								GShaderResourceView* ppSRV = nullptr,
								GRenderTargetView* ppRTV = nullptr,
								GDepthStencilView* ppDSV = nullptr) = 0;

	virtual SPtr<GVertexShader>
	createVertexShader(SPtr<ShaderResource> shader) = 0;

  virtual SPtr<GPixelShader>
	createPixelShader(SPtr<ShaderResource> shader) = 0;

  virtual SPtr<GInputLayout>
  createInputLayout(Vector<GInputElement> descriptor,
										const SPtr<GVertexShader>& desc) = 0;

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
	clear(const Color& color) = 0;

	virtual void
	present() = 0;

	virtual void
	setViewport(int32 x, int32 y, int32 width, int32 height) = 0;

public:

	void*
	m_pWHandle = nullptr;

  GDevice* 
	m_pDevice = nullptr;

	GDeviceContext* 
	m_pDeviceContext = nullptr;
	
	GSwapChain* 
	m_pSwapChain = nullptr;
  
	GRenderTargetView* 
	m_pRenderTargetView = nullptr;
	
	GDepthStencilView* 
	m_pDepthStencilView = nullptr;


};


}

