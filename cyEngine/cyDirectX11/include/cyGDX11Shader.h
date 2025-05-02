#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGShader.h>

#include <d3d11_2.h>

namespace CYLLENE_SDK
{

struct GDX11ShaderBlob : public GShaderBlob
{
public:

  /*
    * The idea is that the DX11 Shaderblob has a D3D11Blob
    */
  virtual void*
    getBlob() override {
    return m_pBlob;
  }

  uint32 size;
  uint32 type;
  String entry;
  String model;
  String data;
  String error;
  bool isCompiled = false;

  ID3DBlob* m_pBlob;
};

class CY_DX11_EXPORT GDX11VertexShader : public GVertexShader
{
public:

  GDX11VertexShader() = default;

  virtual ~GDX11VertexShader() override;

  virtual void*
  getBlob() override {
    return m_pBlob;
  }

public:

  GDX11ShaderBlob* m_pBlob = nullptr;

  ID3D11VertexShader* m_pVertexShader = nullptr;

};

class CY_DX11_EXPORT GDX11PixelShader : public GPixelShader
{
public:

  GDX11PixelShader() = default;

  virtual ~GDX11PixelShader() override;
  
  virtual void*
    getBlob() override {
    return m_pBlob;
  }

public:

  GDX11ShaderBlob* m_pBlob = nullptr;

  ID3D11PixelShader* m_pPixelShader = nullptr;

};


}