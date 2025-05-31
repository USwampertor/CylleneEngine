#pragma once

#include "cyGraphicsPrerequisites.h"


// TODO: Check what things can be fast forwarded
#include <cyColor.h>
#include <cyCCamera.h>
#include <cyBBeing.h>
#include <cyModule.h>
#include <cyRShader.h>
#include <cyRTexture.h>
#include <cyRMesh.h>
#include <cyRModel.h>
#include <cyRResource.h>
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
#include "cyGGraphicPass.h"

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

	bool
	registerGraphicPass(SPtr<GGraphicPass> newPass);

// 	virtual SPtr<GDevice>
// 	createDevice(const GDeviceElement& deviceParams) = 0;
// 
//   virtual SPtr<GDeviceContext>
//   createDeviceContext() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView(SPtr<GRenderTargetViewElement> srvParams,
												 SPtr<GTexture> texture = nullptr) = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView(SPtr<GDepthStencilViewElement> dsvParams,
												 SPtr<GTexture> texture = nullptr) = 0;

	virtual SPtr<GSwapChain>
	createSwapChain(SPtr<GDevice> device, 
									SPtr<GSwapChainElement> swapChainParams) = 0;

  virtual void // SPtr<GShaderResourceView>
  createShaderResourceView(SPtr<GTexture> shaderResourceView,
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

	virtual SPtr<GComputeShader>
	createComputeShader(SPtr<RShader> shader, const String& entry) = 0;

	virtual SPtr<GShader>
	createShader(SPtr<RShader> shader, const String& entry) = 0;

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
	draw(SPtr<CCamera> refCamera, SPtr<BBeing> redObject) = 0;

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

	template<typename T,
	typename = std::enable_if_t<std::is_base_of<RResource, T>::value>>
	String
	generateResourceID(const String& assetName) {
	  RESOURCE_TYPE::E type = T::staticType();
	  String realName = Utils::format("%s_%s", type._to_string(), assetName.c_str());
	  return realName;
	}

	void
  registerResource(SPtr<RResource> resource) {
		RESOURCE_TYPE::E type = resource->getType();
    String realName = Utils::format("%s_%s", type._to_string(), resource->getName().c_str());
    
    // Check if resource is mesh or texture
    if (resource->getType() == RESOURCE_TYPE::E::eMODEL) {
      SPtr<RModel> model = std::reinterpret_pointer_cast<RModel>(resource);

			for (auto& mesh : model->m_meshes) {
        SPtr<GMesh> newGMesh = createMesh(mesh);
        if (newGMesh != nullptr) {
					type = RESOURCE_TYPE::E::eMESH;
          realName = Utils::format("%s_%s", type._to_string(), mesh->getName().c_str());
          m_meshRenderPool.try_emplace(Hash<String>()(realName), newGMesh);
        }
			}

      
    }
    else if (resource->getType() == RESOURCE_TYPE::E::eTEXTURE) {
      SPtr<RTexture> texture = std::reinterpret_pointer_cast<RTexture>(resource);
			SPtr<GTexture> newGTexture = createTexture2D(texture);
			if (newGTexture != nullptr) {
				m_textureRenderPool.try_emplace(Hash<String>()(realName), newGTexture);
			}
    }
    else if (resource->getType() == RESOURCE_TYPE::E::eSHADER) {
      SPtr<RShader> shader = std::reinterpret_pointer_cast<RShader>(resource);
			SPtr<GShader> newGShader;
			SHADER_TYPE::E shaderType = shader->getShaderType();

			// TODO: Simplify this. We can make just a ShaderElement with default params and
			// maybe wrap the createvertex/pixel/compute into a createshader function
			// TODO: Finish implementing other shaders

			if ( +SHADER_TYPE::E::VERTEX == shaderType) {
				newGShader = createVertexShader(shader, "vertex_main");
			}
			else if (+SHADER_TYPE::E::PIXEL == shaderType) {
        newGShader = createPixelShader(shader, "pixel_main");
			}
			
      if (newGShader != nullptr) {
        m_shaderRenderPool.try_emplace(Hash<String>()(realName), newGShader);
      }
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

	Map<uint32, SPtr<GMesh>>
	m_meshRenderPool;

	Map<uint32, SPtr<GTexture>>
	m_textureRenderPool;

	Map<uint32, SPtr<GShader>>
	m_shaderRenderPool;

};


}

