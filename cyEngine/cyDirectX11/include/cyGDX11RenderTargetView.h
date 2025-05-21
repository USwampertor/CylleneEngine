#pragma once
#include "cyDirectX11Prerequisites.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11Texture.h"

#include <cyGRenderTargetView.h>
#include <d3d11_2.h>

namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11RenderTargetView : public GRenderTargetView
{
public:
  GDX11RenderTargetView() = default;

  virtual ~GDX11RenderTargetView() override;
  
  virtual void* 
  get() override;
  
  virtual void 
  set(void* pHandle) override;

  virtual SPtr<GTexture>
  getTexture() override;

  virtual SPtr<GDepthStencilView>
  getDepthStencil() override;

  
public:

  ID3D11RenderTargetView* m_pRTV = nullptr;

  SPtr<GDX11Texture> m_pTexture = nullptr;

  SPtr<GDX11DepthStencilView> m_pDSV = nullptr;

};

}

