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

void GDX11Device::queryInterface(int32 width, int32 height) {
  

}




}

