#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGDepthStencilView.h>

#include <d3d11_1.h>

namespace CYLLENE_SDK
{

class GDX11DepthStencilView : public GDepthStencilView
{
public:
  GDX11DepthStencilView() = default;

  virtual ~GDX11DepthStencilView() override;

  virtual void* 
  get() override;
  
  virtual void 
  set(void* pHandle) override;

public:

  ID3D11DepthStencilView* m_pDSV = nullptr;
  
};

}
