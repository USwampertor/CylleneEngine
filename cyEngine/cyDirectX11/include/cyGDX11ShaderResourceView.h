#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGShaderResourceView.h>

namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11ShaderResourceView : public GShaderResourceView
{
public:
  GDX11ShaderResourceView() = default;

  ~GDX11ShaderResourceView() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

public:

  ID3D11ShaderResourceView* m_pd3d11SRV = nullptr;
  ID3D11UnorderedAccessView* m_pd3d11UAV = nullptr;

};

}
