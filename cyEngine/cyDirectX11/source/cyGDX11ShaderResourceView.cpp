#include "cyGDX11ShaderResourceView.h"

namespace CYLLENE_SDK
{

GDX11ShaderResourceView::~GDX11ShaderResourceView() {
  DX11_SAFE_RELEASE(m_pSRV);
}

void* GDX11ShaderResourceView::get() {
  return m_pSRV;
}

void GDX11ShaderResourceView::set(void* pHandle) {
  m_pSRV = static_cast<ID3D11ShaderResourceView*>(pHandle);
}

}
