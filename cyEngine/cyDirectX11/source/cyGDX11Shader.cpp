#include "cyGDX11Shader.h"

namespace CYLLENE_SDK
{

GDX11VertexShader::~GDX11VertexShader()
{
  DX11_SAFE_RELEASE(m_pVertexShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void* 
GDX11VertexShader::getShader() {
  return m_pVertexShader;
}

void 
GDX11VertexShader::set(void* pHandle) {
  m_pVertexShader = static_cast<ID3D11VertexShader*>(pHandle);
}

void* 
GDX11VertexShader::get() {
  return m_pVertexShader;
}

GDX11PixelShader::~GDX11PixelShader()
{
  DX11_SAFE_RELEASE(m_pPixelShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void* 
GDX11PixelShader::getShader() {
  return m_pPixelShader;
}

void 
GDX11PixelShader::set(void* pHandle) {
  m_pPixelShader = static_cast<ID3D11PixelShader*>(pHandle);
}

void* 
GDX11PixelShader::get() {
  return m_pPixelShader;
}

}
