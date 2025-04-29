#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{
class GDevice
{
public:

  GDevice() = default;
  ~GDevice() = default;

  virtual void
  queryInterface() = 0;


};
}
