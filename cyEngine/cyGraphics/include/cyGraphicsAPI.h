#pragma once

#include "cyGraphicsPrerequisites.h"
#include <cyModule.h>

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

	void*
  createTexture(int32 width, 
								int32 height, 
								int32 format, 
								int32 usage, 
								uint32 bindFlags, 
								uint32 cpuAccessFlags = 0,
								uint32 mipFlags = 1,
								void* ppSRV = nullptr,
								void* ppRTV = nullptr,
								void* ppDSV = nullptr);

};


}

