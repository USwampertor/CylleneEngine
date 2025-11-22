#include "cyGDX11RasterizerState.h"

namespace CYLLENE_SDK
{

GDX11RasterizerState::~GDX11RasterizerState() {
  DX11_SAFE_RELEASE(m_pRasterizerState);
}

void* GDX11RasterizerState::get() {
  return m_pRasterizerState;
}

void GDX11RasterizerState::set(void* pHandle) {
  m_pRasterizerState = static_cast<ID3D11RasterizerState1*>(pHandle);
}

}
