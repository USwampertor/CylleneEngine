#include "cyGDX11SwapChain.h"
#include "cyGDX11Texture.h"


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

void
GDX11SwapChain::present() {
  if (m_pSwapChain) {
    m_pSwapChain->Present(0, 0);
  }
}

SPtr<GTexture>
GDX11SwapChain::getBuffer(uint32 num) {
  if (m_pSwapChain) {
    ID3D11Texture2D* pBackBuffer = nullptr;
    m_pSwapChain->GetBuffer(num, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    SPtr<GDX11Texture> pTexture = std::make_shared<GDX11Texture>();
    pTexture->m_texture = pBackBuffer;

    return pTexture;
  }

  return nullptr;
}



}