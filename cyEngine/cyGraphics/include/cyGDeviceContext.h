#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"
#include "cyGViewport.h"


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
  setViewPort(uint32 numViewports, const GViewport& viewport) = 0;

  virtual void
  updateSubresource(Graphic* resource, uint32 index, uint32 data) = 0;
};
}
