#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{

struct GInputLayoutElement
{
  String   semanticName;
  uint32_t semanticIndex;
  uint32_t format;
  uint32_t inputSlot;
  uint32_t alignedByteOffset;
  uint32_t inputSlotClass;
  uint32_t instanceDataStepRate;
};


namespace INPUTCLASSIFICATION
{
  BETTER_ENUM(E, uint32, PERVERTEX = 0, PERINSTANCE = 1);
}

class CY_GRAPHICS_EXPORT GInputLayout : public GGraphic
{
public:
  GInputLayout() = default;
  virtual ~GInputLayout() {}
};
}