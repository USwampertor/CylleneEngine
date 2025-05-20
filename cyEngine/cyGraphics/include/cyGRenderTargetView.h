#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyGTexture.h"

namespace CYLLENE_SDK
{

struct GRenderTargetViewElement
{
  void* resource;
  uint32 format;
  uint32 viewDimension;
  uint32 mipLevels;
  uint32 firstMipLevel;
  uint32 firstArraySlice;
  uint32 arraySize;
  uint32 minLOD;
  uint32 maxLOD;
  uint32 flags;

};

class CY_GRAPHICS_EXPORT GRenderTargetView : public GGraphic
{
public:
  GRenderTargetView() = default;
  
  virtual ~GRenderTargetView() {}
  
  virtual SPtr<GTexture>
  getTexture() = 0;

};

}
