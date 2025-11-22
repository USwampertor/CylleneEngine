#include "cyGDX11RenderTargetView.h"

namespace CYLLENE_SDK
{

GDX11RenderTargetView::~GDX11RenderTargetView() {
  DX11_SAFE_RELEASE(m_pRTV);
}

void* 
GDX11RenderTargetView::get() {
  return m_pRTV;
}

void
GDX11RenderTargetView::set(void* pHandle) {
  m_pRTV = static_cast<ID3D11RenderTargetView*>(pHandle);
}

SPtr<GTexture>
GDX11RenderTargetView::getTexture() {
  return m_pTexture;
}

SPtr<GDepthStencilView>
GDX11RenderTargetView::getDepthStencil() {
  return m_pDSV;
}



}
