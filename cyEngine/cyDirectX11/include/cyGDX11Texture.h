#pragma once
#include "cyDirectX11Prerequisites.h"
#include "cyGDX11ShaderResourceView.h"
#include <cyGTexture.h>
#include <d3d11_2.h>

namespace CYLLENE_SDK {

class CY_DX11_EXPORT GDX11Texture : public GTexture
{
public:
  GDX11Texture() = default;

  virtual ~GDX11Texture() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  virtual SPtr<GShaderResourceView>
  getResource() override;

  ID3D11Texture2D* m_texture = nullptr;
  SPtr<GDX11ShaderResourceView> m_pSRV = nullptr;
  // ID3D11ShaderResourceView* m_pSRV = nullptr;
};

}

