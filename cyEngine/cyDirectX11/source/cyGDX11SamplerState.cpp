#include "cyGDX11SamplerState.h"

namespace CYLLENE_SDK
{

GDX11SamplerState::~GDX11SamplerState() {
  DX11_SAFE_RELEASE(m_pSamplerState);
}

void* GDX11SamplerState::get() {
  return m_pSamplerState;
}

void GDX11SamplerState::set(void* pHandle) {
  m_pSamplerState = static_cast<ID3D11SamplerState*>(pHandle);
}

}