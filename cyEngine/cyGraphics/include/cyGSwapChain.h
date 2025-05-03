#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGTexture.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK
{

struct GSwapChainElement
{
  void* windowHandle;
  String description;
  uint32 width;
  uint32 height;
  uint32 format;
  bool stereo;
  uint32 bufferCount;
  uint32 bufferUsage;
  uint32 sampleCount;
  uint32 sampleQuality;
  uint32 swapEffect;
  uint32 alphaMode;
  uint32 scaling;
  uint32 flags;
};

class CY_GRAPHICS_EXPORT GSwapChain : public Graphic
{
public:
  GSwapChain() = default;
  
  virtual ~GSwapChain() {}

  virtual void
  present(uint32 syncInterval = 0, uint32 flags = 0) = 0;

  virtual SPtr<GTexture>
  getBuffer(uint32 num) = 0;


};

}
