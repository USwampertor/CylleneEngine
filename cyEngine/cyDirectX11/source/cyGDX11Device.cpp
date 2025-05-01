#include "cyGDX11Device.h"

namespace CYLLENE_SDK {

GDX11Device::~GDX11Device() {
  DX11_SAFE_RELEASE(m_pDevice);
}
void* GDX11Device::get() {
  return m_pDevice;
}

void GDX11Device::set(void* pHandle) {
  m_pDevice = static_cast<ID3D11Device*>(pHandle);
}

void 
GDX11Device::queryInterface(SPtr<GSwapChain> swapChain, 
                            SPtr<GDepthStencilView> backBuffer, 
                            int32 width, 
                            int32 height) {
  
  if (!swapChain) {
    return;
  }



  SPtr<GTexture> texture = swapChain->getBuffer(0);


}

void
GDX11Device::queryInterface(SPtr<GSwapChain> swapChain,
                            SPtr<GDepthStencilView> backBuffer,
                            Vector2i size) {
  return queryInterface(swapChain, backBuffer, size.x, size.y);
}





}

