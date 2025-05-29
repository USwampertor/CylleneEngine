#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{

namespace BLEND_MASK
{
  BETTER_ENUM(E, uint32, RED_CHANNEL = 1 << 0, GREEN_CHANNEL = 1 << 1, BLUE_CHANNEL = 1 << 2, ALPHA_CHANNEL = 1 << 3, ALL_CHANNELS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
}

struct GBlendElement
{
  bool enabled = false;
  uint32 writeMask = BLEND_MASK::E::ALL_CHANNELS;
};

class CY_GRAPHICS_EXPORT GBlendState : public GGraphic
{
public:
  GBlendState() = default;
  virtual ~GBlendState() {}
};

}

