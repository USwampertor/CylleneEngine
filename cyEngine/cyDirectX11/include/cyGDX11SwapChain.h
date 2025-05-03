#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGSwapChain.h>

#include <d3d11_2.h>

namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11SwapChain : public GSwapChain
{
public:
  
  GDX11SwapChain() = default;
  
  virtual ~GDX11SwapChain() override;
  
  virtual void*
  get() override;
  
  virtual void
  set(void* pHandle) override;

  virtual void
  present() override;

  virtual SPtr<GTexture>
  getBuffer(uint32 num) override;

public:

  IDXGISwapChain1* m_pSwapChain = nullptr;
};

}