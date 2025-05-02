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
  // Binding logic for DirectX 11 textures
  // This is typically done in the context of a rendering pipeline
  // For example, you might bind the texture to a shader resource view
}

void*
GDX11Texture::getBlob() {
  // Logic to get the blob data from the texture
  // This might involve using ID3D11DeviceContext::Map to access the texture data
  return nullptr; // Placeholder
}

void*
GDX11Texture::getResource() {
  // Logic to get the resource associated with the texture
  // This might involve querying the texture for its resource view
  return nullptr; // Placeholder
}


}
