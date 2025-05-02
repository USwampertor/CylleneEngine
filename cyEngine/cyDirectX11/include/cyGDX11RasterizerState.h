#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGRasterizerState.h>

namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11RasterizerState : public GRasterizerState
{
public:
  GDX11RasterizerState() = default;
  
  virtual ~GDX11RasterizerState() override;
  
  virtual void*
  get() override;
  
  virtual void
  set(void* pHandle) override;

public:

  ID3D11RasterizerState1* m_pRasterizerState = nullptr;

};

}
