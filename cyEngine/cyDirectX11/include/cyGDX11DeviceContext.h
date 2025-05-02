#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGDeviceContext.h>
#include <d3d11_2.h>

namespace CYLLENE_SDK
{
class GDX11DeviceContext : public GDeviceContext
{
public:
  GDX11DeviceContext() = default;

  virtual ~GDX11DeviceContext();

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;


public:

  ID3D11DeviceContext1* m_pDeviceContext = nullptr;

};
}
