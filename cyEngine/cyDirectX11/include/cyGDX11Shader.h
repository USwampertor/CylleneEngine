#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGShader.h>

#include <d3d11_2.h>

namespace CYLLENE_SDK
{

struct CY_DX11_EXPORT GDX11ShaderBlob : public GShaderBlob
{
public:

  /*
    * The idea is that the DX11 Shaderblob has a D3D11Blob
    */
  virtual void*
    getBlob() override {
    return m_pBlob;
  }

  ID3DBlob* m_pBlob;
};

class CY_DX11_EXPORT GDX11VertexShader : public GVertexShader
{
public:

  GDX11VertexShader() = default;

  virtual ~GDX11VertexShader() override;

  virtual void*
  getBlob() override {
    return reinterpret_cast<void*>(m_pBlob.get());
  }

  virtual void*
  getShader();

  virtual void
  set(void* pHandle) override;

  virtual void*
  get() override;

  virtual void
  reflect() override;

public:

  SPtr<GDX11ShaderBlob> m_pBlob = nullptr;

  ID3D11VertexShader* m_pVertexShader = nullptr;

};

class CY_DX11_EXPORT GDX11PixelShader : public GPixelShader
{
public:

  GDX11PixelShader() = default;

  virtual ~GDX11PixelShader() override;
  
  virtual void*
  getBlob() override {
    return reinterpret_cast<void*>(m_pBlob.get());
  }

  virtual void*
  getShader();

  virtual void
  set(void* pHandle) override;

  virtual void*
  get() override;

  virtual void
  reflect() override;

public:

  SPtr<GDX11ShaderBlob> m_pBlob = nullptr;


  ID3D11PixelShader* m_pPixelShader = nullptr;

};

class CY_DX11_EXPORT GDX11GeometryShader : public GGeometryShader
{
public:

  GDX11GeometryShader() = default;

  virtual ~GDX11GeometryShader() override;
  
  virtual void*
  getBlob() override {
    return reinterpret_cast<void*>(m_pBlob.get());
  }

  virtual void*
  getShader();

  virtual void
  set(void* pHandle) override;

  virtual void*
  get() override;

  virtual void
  reflect() override;

public:

  SPtr<GDX11ShaderBlob> m_pBlob = nullptr;


  ID3D11GeometryShader* m_pGeometryShader = nullptr;

};


class CY_DX11_EXPORT GDX11ComputeShader : public GComputeShader
{
public:

  GDX11ComputeShader() = default;

  virtual ~GDX11ComputeShader() override;
  
  virtual void*
  getBlob() override {
    return reinterpret_cast<void*>(m_pBlob.get());
  }

  virtual void*
  getShader();

  virtual void
  set(void* pHandle) override;

  virtual void*
  get() override;

  virtual void
  reflect() override;

public:

  SPtr<GDX11ShaderBlob> m_pBlob = nullptr;


  ID3D11ComputeShader* m_pComputeShader = nullptr;

};



}