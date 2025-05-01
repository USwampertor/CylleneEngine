#include "cyGDX11SwapChain.h"

namespace CYLLENE_SDK
{

  GDX11SwapChain::~GDX11SwapChain() {
    DX11_SAFE_RELEASE(m_pSwapChain);
  }
  
  void*
  GDX11SwapChain::get() {
    return m_pSwapChain;
  }
  
  void
  GDX11SwapChain::set(void* pHandle) {
    m_pSwapChain = static_cast<IDXGISwapChain1*>(pHandle);
  }

}