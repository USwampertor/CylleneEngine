#include "cyGDX11ShaderResourceView.h"

namespace CYLLENE_SDK
{

GDX11ShaderResourceView::~GDX11ShaderResourceView() {
  DX11_SAFE_RELEASE(m_pd3d11SRV);
}

void* GDX11ShaderResourceView::get() {
  return m_pd3d11SRV;
}

void GDX11ShaderResourceView::set(void* pHandle) {
  m_pd3d11SRV = static_cast<ID3D11ShaderResourceView*>(pHandle);
}

}
