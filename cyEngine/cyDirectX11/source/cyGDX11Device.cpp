#include "cyGDX11Device.h"
#include "cyGDX11Texture.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"

namespace CYLLENE_SDK {

GDX11Device::~GDX11Device() {
  DX11_SAFE_RELEASE(m_pDevice);
}
void* GDX11Device::get() {
  return m_pDevice;
}

void GDX11Device::set(void* pHandle) {
  m_pDevice = static_cast<ID3D11Device*>(pHandle);
}

void
queryInterface(SPtr<GSwapChain> swapChain, 
               SPtr<GDepthStencilView> depthStencil, 
               SPtr<GRenderTargetView> renderTargetView, 
               int32 width, 
               int32 height) {

}

void
GDX11Device::queryInterface(SPtr<GSwapChain> swapChain,
                            SPtr<GDepthStencilView> depthStencil, 
                            SPtr<GRenderTargetView> renderTargetView,
                            Vector2i size) {
  return queryInterface(swapChain, depthStencil, renderTargetView, size.x, size.y);
}


SPtr<GDepthStencilView>
GDX11Device::createDepthStencilView(SPtr<GTexture> depthStencilView,
                                    SPtr<GDepthStencilViewElement> dsvParams) {
  D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
  dsvDesc.Format; // = format;
  dsvDesc.Texture2D.MipSlice = 0;
  dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

  SPtr<GDX11DepthStencilView> pDepthStencilView = std::make_shared<GDX11DepthStencilView>();
  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(depthStencilView);


  m_pDevice->CreateDepthStencilView(pTexture->m_texture, &dsvDesc, &pDepthStencilView->m_pDSV);

  if (FAILED(m_pDevice->CreateDepthStencilView(pTexture->m_texture, &dsvDesc, &pDepthStencilView->m_pDSV))) {
    return nullptr;
  }
  return std::static_pointer_cast<GDepthStencilView>(pDepthStencilView);
}

SPtr<GRenderTargetView>
GDX11Device::createRenderTargetView(SPtr<GTexture> renderTargetView, 
                                    SPtr<GRenderTargetViewElement> rtvParams) {
  D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
  rtvDesc.Format; // = format;
  rtvDesc.Texture2D.MipSlice = 0;
  rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  
  SPtr<GDX11RenderTargetView> pRenderTargetView = std::make_shared<GDX11RenderTargetView>();
  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(renderTargetView);

  if (FAILED(m_pDevice->CreateRenderTargetView(pTexture->m_texture, &rtvDesc, &pRenderTargetView->m_pRTV))) {
    return nullptr;
  }
  return std::static_pointer_cast<GRenderTargetView>(pRenderTargetView);
}

SPtr<GTexture>
GDX11Device::createTexture2D(SPtr<GTextureElement> textureParams) {

  SPtr<GDX11Texture> pTexture = std::make_shared<GDX11Texture>();

  D3D11_TEXTURE2D_DESC desc;

  memset(&pTexture->m_texture, 0, sizeof(pTexture->m_texture));

  desc.ArraySize = 1;
  desc.BindFlags = textureParams->bindFlags;
  desc.CPUAccessFlags = textureParams->cpuAccessFlags;
  desc.Format;// = textureParams->;
  desc.Height = textureParams->height;
  desc.Width = textureParams->width;
  desc.MipLevels = textureParams->mipLevels;
  desc.MiscFlags = 0;

  desc.SampleDesc.Count = 1; //MSAA
  desc.SampleDesc.Quality = 0;

  desc.Usage; // = textureParams->usage;

  if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, &pTexture->m_texture))) {
    return nullptr;
  }
  return std::static_pointer_cast<GTexture>(pTexture);
}


}

