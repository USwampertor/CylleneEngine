#include "cyGDX11DeviceContext.h"

namespace CYLLENE_SDK
{

GDX11DeviceContext::~GDX11DeviceContext() {
  DX11_SAFE_RELEASE(m_pDeviceContext);
}

void* GDX11DeviceContext::get() {
  return m_pDeviceContext;
}

void GDX11DeviceContext::set(void* pHandle) {
  m_pDeviceContext = static_cast<ID3D11DeviceContext1*>(pHandle);
}

}