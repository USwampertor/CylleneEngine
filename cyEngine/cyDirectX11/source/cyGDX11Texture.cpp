#include "cyGDX11Texture.h"

namespace CYLLENE_SDK
{

GDX11Texture::~GDX11Texture() {
  DX11_SAFE_RELEASE(m_texture);
}

void*
GDX11Texture::get() {
  return m_texture;
}

void
GDX11Texture::set(void* pHandle) {
  m_texture = static_cast<ID3D11Texture2D*>(pHandle);
}

void
GDX11Texture::bind() {

}

void*
GDX11Texture::getBlob() {
  return nullptr;
}

void*
GDX11Texture::getResource() {
  return m_pSRV;
}


}
