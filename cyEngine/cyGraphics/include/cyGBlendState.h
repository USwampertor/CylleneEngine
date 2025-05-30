#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK {

  namespace BLEND_MASK
  {
    BETTER_ENUM(E, uint32, 
      RED   = 1 << 0, 
      GREEN = 1 << 1, 
      BLUE  = 1 << 2, 
      ALPHA = 1 << 3, 
      ALL   = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
  }

struct GBlendElement
{
  bool enabled = false;
  uint32 writeMask = BLEND_MASK::E::ALL;
};


class CY_GRAPHICS_EXPORT GBlendState : public GGraphic
{
  GBlendState() = default;
  virtual ~GBlendState() {}
};

}