#include "cyGraphicsDX11API.h"

#define WIN32_LEAN_AND_MEAN
#include <d3dcompiler.h>
#include <d3d11_2.h>
#include <dxgi.h>

#include "cyGDX11Device.h" 
#include "cyGDX11DeviceContext.h" 
#include "cyGDX11SwapChain.h"
#include "cyGDX11Texture.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"

namespace CYLLENE_SDK {

GraphicsDX11API::~GraphicsDX11API() {

}

void
GraphicsDX11API::initialize(void* pHandle) {
  HWND hwnd = static_cast<HWND>(pHandle);
  RECT rc;
  GetClientRect(hwnd, &rc);

  Vector<IDXGIAdapter1*> vAdapters;
  IDXGIAdapter1* pAdapter = nullptr;
  IDXGIFactory1* pFactory = nullptr;

  CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory));

  for (uint32_t i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
    DXGI_ADAPTER_DESC1 desc;
    pAdapter->GetDesc1(&desc);
    vAdapters.push_back(pAdapter);
  }

  DX11_SAFE_RELEASE(pAdapter);
  DX11_SAFE_RELEASE(pFactory);

  Vector<D3D_FEATURE_LEVEL> vFeatureLevels = { D3D_FEATURE_LEVEL_11_1,
                                               D3D_FEATURE_LEVEL_11_0,
                                               D3D_FEATURE_LEVEL_10_1,
                                               D3D_FEATURE_LEVEL_10_0,
                                               D3D_FEATURE_LEVEL_9_3, };

  D3D_FEATURE_LEVEL selectedFeatureLevel;

  ID3D11Device* pDevice = nullptr;
  ID3D11DeviceContext* pDeviceContext = nullptr;

  SPtr<GDX11Device> sPtrDevice = std::static_pointer_cast<GDX11Device>(m_pDevice);
  SPtr<GDX11DeviceContext> sPtrDeviceContext = std::static_pointer_cast<GDX11DeviceContext>(m_pDeviceContext);

  uint32_t deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
  deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  HRESULT hr = D3D11CreateDevice(vAdapters[0],
                                 D3D_DRIVER_TYPE_UNKNOWN,
                                 nullptr,
                                 deviceFlags,
                                 vFeatureLevels.data(),
                                 vFeatureLevels.size(),
                                 D3D11_SDK_VERSION,
                                 &pDevice,
                                 &selectedFeatureLevel,
                                 &pDeviceContext);

  if (FAILED(hr)) {
    MessageBox(hwnd, "Failed to create device", "Error", MB_OK);
    return;
  }

  pDevice->QueryInterface(__uuidof(ID3D11Device1),
  reinterpret_cast<void**>(&sPtrDevice->m_pDevice));

  pDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1),
  reinterpret_cast<void**>(&sPtrDeviceContext->m_pDeviceContext));

  DX11_SAFE_RELEASE(pDeviceContext);
  DX11_SAFE_RELEASE(pDevice);

  DXGI_SWAP_CHAIN_DESC1 scDesc;
  memset(&scDesc, 0, sizeof(scDesc));

  scDesc.Width = rc.right;
  scDesc.Height = rc.bottom;
  // This is because we are using colors as float
  scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // DXGI_FORMAT_R32G32B32A32_FLOAT;
  scDesc.Stereo = false;
  scDesc.SampleDesc.Count = 1; // MSAA
  scDesc.SampleDesc.Quality = 0;
  scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scDesc.BufferCount = 2;
  scDesc.Scaling = DXGI_SCALING_NONE;
  scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

  IDXGIDevice1* pDXGIDevice = nullptr;

  sPtrDevice->m_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice);

  IDXGIAdapter* pDXGIAdapter = nullptr;
  pDXGIDevice->GetAdapter(&pDXGIAdapter);

  IDXGIFactory2* pFactory2 = nullptr;
  pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory2);

  SPtr<GDX11SwapChain> sPtrSwapChain = std::static_pointer_cast<GDX11SwapChain>(m_pSwapChain);

  hr =
  pFactory2->CreateSwapChainForHwnd(sPtrDevice->m_pDevice,
                                    hwnd,
                                    &scDesc,
                                    nullptr,
                                    nullptr,
                                    &sPtrSwapChain->m_pSwapChain);

  pDXGIDevice->SetMaximumFrameLatency(3);

  if (FAILED(hr)) {
    MessageBox(hwnd, "Failed to create swap chain", "Error", MB_OK);
    return;
  }
  queryInterface(scDesc.Width, scDesc.Height);
}


void
GraphicsDX11API::queryInterface(int32 width, int32 height) {

  if (!m_pSwapChain) {
    return;
  }

  SPtr<GDX11Device> pDevice = std::static_pointer_cast<GDX11Device>(m_pDevice);
  SPtr<GDX11DepthStencilView> pDepthStencilView = std::static_pointer_cast<GDX11DepthStencilView>(m_pDepthStencilView);
  SPtr<GDX11RenderTargetView> pRenderTargetView = std::static_pointer_cast<GDX11RenderTargetView>(m_pRenderTargetView);
  
  SPtr<GDX11Texture> pBackBuffer = std::static_pointer_cast<GDX11Texture>(m_pSwapChain->getBuffer(0));
  
  pDevice->m_pDevice->CreateRenderTargetView(pBackBuffer->m_texture, nullptr, &pRenderTargetView->m_pRTV);

  // TODO ?
  DX11_SAFE_RELEASE(pBackBuffer->m_texture);


  SPtr<GDX11Texture> pDepthStencil = std::static_pointer_cast<GDX11Texture>(createTexture2D(Vector2i(width, height),
                                                                                            DXGI_FORMAT_D24_UNORM_S8_UINT,
                                                                                            D3D11_USAGE_DEFAULT,
                                                                                            D3D11_BIND_DEPTH_STENCIL));

  if (!pDepthStencil->m_texture) {
    MessageBox(nullptr, "Failed to create depth stencil", "Error", MB_OK);
    return;
  }

  
  GDepthStencilViewElement e;
  m_pDepthStencilView = m_pDevice->createDepthStencilView(pDepthStencil, e);

  DX11_SAFE_RELEASE(pDepthStencil->m_texture);
}


void
GraphicsDX11API::queryInterface(const Vector2i & size) {
  return queryInterface(size.x, size.y);
}

SPtr<GTexture>
GraphicsDX11API::createTexture2D(const Vector2i& size,
									               uint32 bindFlags,
									               uint32 format,
									               uint32 usage,		
                                 uint32 cpuAccessFlags = 0,
                                 uint32 mipFlags = 1,
                                 GShaderResourceView* ppSRV,
                                 GRenderTargetView* ppRTV,
                                 GDepthStencilView* ppDSV) {
  SPtr<GDX11Texture> pTexture = std::make_shared<GDX11Texture>();
  
  D3D11_TEXTURE2D_DESC desc;

  memset(&pTexture, 0, sizeof(pTexture));

  desc.ArraySize = 1;
  desc.BindFlags = bindFlags;
  desc.CPUAccessFlags = cpuAccessFlags;
  desc.Format = static_cast<DXGI_FORMAT>(format);
  desc.Height = size.x;
  desc.Width = size.y;
  desc.MipLevels = mipFlags;
  desc.MiscFlags = 0;

  desc.SampleDesc.Count = 1; //MSAA
  desc.SampleDesc.Quality = 0;

  desc.Usage = static_cast<D3D11_USAGE>(usage);

  if (FAILED(m_pDevice->m_pDevice->CreateTexture2D(&desc, nullptr, &pTexture))) {
    return nullptr;
  }

  if (ppSRV != nullptr) {
    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
      srvDesc.Format = static_cast<DXGI_FORMAT>(format);
      srvDesc.Texture2D.MipLevels = mipFlags == 1 ? 1 : -1;
      srvDesc.Texture2D.MostDetailedMip = 0;
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, ppSRV);


    }
  }

  if (ppSRV != nullptr) {
    if (bindFlags & D3D11_BIND_RENDER_TARGET) {

      D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
      rtvDesc.Format = static_cast<DXGI_FORMAT>(format);
      rtvDesc.Texture2D.MipSlice = 0;
      rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateRenderTargetView(pTexture, &rtvDesc, ppRTV);


    }
  }

  if (ppDSV != nullptr) {
    if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {

      D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
      dsvDesc.Format = static_cast<DXGI_FORMAT>(format);
      dsvDesc.Texture2D.MipSlice = 0;
      dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      m_pDevice->CreateDepthStencilView(pTexture, &dsvDesc, ppDSV);


    }
  }

  return pTexture;
}


}