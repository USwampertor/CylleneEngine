#include "cyGDX11DepthStencilView.h"

namespace CYLLENE_SDK {

GDX11DepthStencilView::~GDX11DepthStencilView() {
  DX11_SAFE_RELEASE(m_pDSV);
}


void* 
GDX11DepthStencilView::get() {
  return m_pDSV;
}

void 
GDX11DepthStencilView::set(void* pHandle) {
  m_pDSV = static_cast<ID3D11DepthStencilView*>(pHandle);
}

SPtr<GTexture>
GDX11DepthStencilView::getTexture() {
  return m_pTexture;
}

}
