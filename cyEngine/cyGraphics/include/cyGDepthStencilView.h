#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyGTexture.h"

namespace CYLLENE_SDK
{
struct GDepthStencilViewElement
{
  void* resource;
  int32 width;
  int32 height;
  uint32 format;
  uint32 viewDimension;
  uint32 mipLevels;
  uint32 firstMipLevel;
  uint32 firstArraySlice;
  uint32 arraySize;
  uint32 minLOD;
  uint32 maxLOD;
  uint32 flags;
  SPtr<GTexture> texture = nullptr;
};

class CY_GRAPHICS_EXPORT GDepthStencilView : public GGraphic
{
public:
  GDepthStencilView() = default;

  virtual ~GDepthStencilView() {}

  virtual SPtr<GTexture>
  getTexture() = 0;

};
}
