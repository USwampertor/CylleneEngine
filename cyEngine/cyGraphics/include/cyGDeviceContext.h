#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{
class GDeviceContext
{
public:

  GDeviceContext() = default;
  ~GDeviceContext() = default;

  virtual void
  queryInterface() = 0;

  virtual void
  setViewPort() = 0;


};
}
