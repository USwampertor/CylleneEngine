#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK
{
class GDeviceContext : public Graphic
{
public:

  GDeviceContext() = default;
  ~GDeviceContext() = default;

  virtual void
  queryInterface() = 0;

  virtual void
  setViewPort() = 0;

  virtual void
  updateSubresource(Graphic* resource, uint32 index, uint32 data) = 0;
};
}
