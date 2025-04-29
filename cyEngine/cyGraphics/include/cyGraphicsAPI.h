#pragma once

#include "cyGraphicsPrerequisites.h"
#include <cyModule.h>
#include <cyShader.h>
#include <cyTexture.h>
#include <cyColor.h>

#include "cyGraphicsBuffer.h"
#include "cyShaderResourceView.h"
#include "cyRenderTargetView.h"
#include "cyDepthStencilView.h"
#include "cyGTexture.h"
#include "cyGShader.h"

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

  GraphicsAPI(void* pHandle);
  
	~GraphicsAPI() = default;

	void
	addToRenderPool(const uint32& index);

	void
	removeFromRenderPool(const uint32& index);

	SPtr<GTexture>
  createTexture(SPtr<TextureResource> texture,
								uint32 bindFlags, 
								uint32 cpuAccessFlags = 0,
								uint32 mipFlags = 1,
								ShaderResourceView* ppSRV = nullptr,
								RenderTargetView* ppRTV = nullptr,
								DepthStencilView* ppDSV = nullptr);

	SPtr<GVertexShader>
	createVertexShader(SPtr<ShaderResource> shader);

  SPtr<GPixelShader>
	createPixelShader(SPtr<ShaderResource> shader);

  SPtr<GraphicsBuffer>
	createVertexBuffer(const Vector<char>& data);
  
	SPtr<GraphicsBuffer>
	createIndexBuffer(const Vector<char>& data);

  SPtr<GraphicsBuffer>
  createConstantBuffer(const Vector<char>& data);

	void
  writeToBuffer(const SPtr<GraphicsBuffer>& buffer, 
								const Vector<char>& data);

	void
	queryInterface(int32 width, int32 height);

	virtual void
	clear(const Color& color) = 0;

	virtual void
	present() = 0;

	virtual void
	setViewport(int32 x, int32 y, int32 width, int32 height) = 0;


};


}

