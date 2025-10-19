#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGBlendState.h>

namespace CYLLENE_SDK {

class CY_DX11_EXPORT GDX11BlendState : public GBlendState
{
public:
  GDX11BlendState() = default;
  
  virtual ~GDX11BlendState() override;
  
  virtual void*
  get() override;
  
  virtual void
  set(void* pHandle) override;

public:

  ID3D11BlendState1* m_pBlendState = nullptr;

};

}

