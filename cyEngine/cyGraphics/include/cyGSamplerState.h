#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{

struct GSamplerStateElement
{
  uint32 filter;
  uint32 addressU;
  uint32 addressV;
  uint32 addressW;
  float mipLODBias;
  uint32 maxAnisotropy;
  uint32 comparisonFunc;
  float borderColor[4];
  float minLOD;
  float maxLOD;
};

class CY_GRAPHICS_EXPORT GSamplerState : public GGraphic
{
public:
  GSamplerState() = default;
  virtual ~GSamplerState() {}
};

}
