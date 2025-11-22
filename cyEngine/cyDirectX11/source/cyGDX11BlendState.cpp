#include "cyGDX11BlendState.h"
namespace CYLLENE_SDK
{

  GDX11BlendState::~GDX11BlendState() {
    DX11_SAFE_RELEASE(m_pBlendState);
  }

  void*
    GDX11BlendState::get() {
    return m_pBlendState;
  }

  void
    GDX11BlendState::set(void* pHandle) {
    m_pBlendState = static_cast<ID3D11BlendState1*>(pHandle);
  }

}