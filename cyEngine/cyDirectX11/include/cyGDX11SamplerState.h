#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGSamplerState.h>

namespace CYLLENE_SDK
{
class CY_DX11_EXPORT GDX11SamplerState : public GSamplerState
{
public:
  GDX11SamplerState() = default;

  ~GDX11SamplerState() override;
  
  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

public:

  ID3D11SamplerState* m_pSamplerState = nullptr;

};
}
