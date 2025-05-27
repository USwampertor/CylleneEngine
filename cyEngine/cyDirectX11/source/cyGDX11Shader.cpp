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


GDX11GeometryShader::~GDX11GeometryShader()
{
  DX11_SAFE_RELEASE(m_pGeometryShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void*
GDX11GeometryShader::getShader() {
  return m_pGeometryShader;
}

void
GDX11GeometryShader::set(void* pHandle) {
  m_pGeometryShader = static_cast<ID3D11GeometryShader*>(pHandle);
}

void*
GDX11GeometryShader::get() {
  return m_pGeometryShader;
}


GDX11ComputeShader::~GDX11ComputeShader()
{
  DX11_SAFE_RELEASE(m_pComputeShader);
  DX11_SAFE_RELEASE(m_pBlob->m_pBlob);
}

void*
GDX11ComputeShader::getShader() {
  return m_pComputeShader;
}

void
GDX11ComputeShader::set(void* pHandle) {
  m_pComputeShader = static_cast<ID3D11ComputeShader*>(pHandle);
}

void*
GDX11ComputeShader::get() {
  return m_pComputeShader;
}


}
