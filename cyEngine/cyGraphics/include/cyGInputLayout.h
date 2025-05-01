#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

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

class CY_GRAPHICS_EXPORT GInputLayout : public Graphic
{
public:
  GInputLayout() = default;
  virtual ~GInputLayout() {}
};
}