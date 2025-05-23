#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyGShaderResourceView.h"

namespace CYLLENE_SDK
{

struct GTextureElement
{
  uint32 width;
  uint32 height;
  uint32 depth;
  uint32 format;
  uint32 mipLevels;
  uint32 arraySize;
  uint32 sampleCount;
  uint32 sampleQuality;
  uint32 usage;
  uint32 bindFlags;
  uint32 cpuAccessFlags;
  uint32 miscFlags;
};


class CY_GRAPHICS_EXPORT GTexture : public GGraphic
{
public:
  GTexture() = default;
  virtual ~GTexture() {}
  
  virtual SPtr<GShaderResourceView> getResource() = 0;

};


}