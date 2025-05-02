#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGInputLayout.h>

namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11InputLayout : public GInputLayout
{
public:
  GDX11InputLayout() = default;
  
  virtual ~GDX11InputLayout() override;

  void* 
  get() override;
  
  void 
  set(void* pHandle) override;


public:

  ID3D11InputLayout* m_pInputLayout = nullptr;

};

}
