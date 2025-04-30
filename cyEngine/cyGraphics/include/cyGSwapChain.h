#pragma once
#include "cyGraphicsPrerequisites.h"

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

class GSwapChain
{
  GSwapChain() = default;
  
  ~GSwapChain() = default;

  virtual void
    present() = 0;
};

}
