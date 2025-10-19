#include "cyDX11GraphicsBuffer.h"

namespace CYLLENE_SDK {

DX11GraphicsBuffer::~DX11GraphicsBuffer() {
  DX11_SAFE_RELEASE(m_pBuffer);
}

void*
DX11GraphicsBuffer::get() {
  return m_pBuffer;
}

void
DX11GraphicsBuffer::set(void* pHandle) {
  m_pBuffer = static_cast<ID3D11Buffer*>(pHandle);
}

}
