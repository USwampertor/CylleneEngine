#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

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


class CY_GRAPHICS_EXPORT GTexture : public Graphic
{
public:
  GTexture() = default;
  virtual ~GTexture() {}
  
  virtual void bind() = 0;
  virtual void* getBlob() = 0;
  virtual void* getResource() = 0;

};


}