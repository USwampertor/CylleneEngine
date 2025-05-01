#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGTexture.h>
#include <d3d11_2.h>

namespace CYLLENE_SDK {

class GDX11Texture : public GTexture
{
public:
  GDX11Texture() = default;

  virtual ~GDX11Texture() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  virtual void 
  bind() override;
  
  virtual void* 
  getBlob() override;
  
  virtual void* 
  getResource() override;

  ID3D11Texture2D* m_texture;
};

}

