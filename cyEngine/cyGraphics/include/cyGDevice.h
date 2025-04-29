#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGDepthStencilView.h"

namespace CYLLENE_SDK
{
class GDevice
{
public:

  GDevice() = default;

  ~GDevice() = default;

  virtual void
  queryInterface() = 0;

  virtual SPtr<GDepthStencilView>
  createDepthStencilView() = 0;

  virtual SPtr<GRenderTargetView>
  createRenderTargetView() = 0;

  virtual SPtr<GShaderResourceView>
  createShaderResourceView() = 0;

};
}
