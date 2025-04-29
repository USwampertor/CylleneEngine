#pragma once
#include "cyGraphicsPrerequisites.h"


namespace CYLLENE_SDK
{

struct GInputElement
{
  String   semanticName;
  uint32_t semanticIndex;
  uint32_t format;
  uint32_t inputSlot;
  uint32_t alignedByteOffset;
  uint32_t inputSlotClass;
  uint32_t instanceDataStepRate;
};

class GInputLayout
{
};
}