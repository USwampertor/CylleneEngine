#include "cyGDX11InputLayout.h"

namespace CYLLENE_SDK
{
GDX11InputLayout::~GDX11InputLayout() {
  DX11_SAFE_RELEASE(m_pInputLayout);
}

void* 
GDX11InputLayout::get() {
  return m_pInputLayout;
}
void 
GDX11InputLayout::set(void* pHandle) {
  m_pInputLayout = static_cast<ID3D11InputLayout*>(pHandle);
}

}
