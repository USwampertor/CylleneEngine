#include "cyGraphicsDX11API.h"

#define WIN32_LEAN_AND_MEAN
#include <d3dcompiler.h>
#include <d3d11_2.h>
#include <dxgi.h>

#include "cyGDX11Device.h" 
#include "cyGDX11DeviceContext.h" 
#include "cyGDX11SwapChain.h"

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

  SPtr<GDX11Device> sPtrDevice = std::make_shared<GDX11Device>();
  SPtr<GDX11DeviceContext> sPtrDeviceContext = std::make_shared<GDX11DeviceContext>();

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

  SPtr<GDX11SwapChain> sPtrSwapChain = std::make_shared<GDX11SwapChain>();

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
  SPtr<GDX11SwapChain> sPtrSwapChain = std::static_pointer_cast<GDX11SwapChain>(m_pSwapChain);
  SPtr<GDX11DeviceContext> sPtrDeviceContext = std::static_pointer_cast<GDX11DeviceContext>(m_pDeviceContext);
  ID3D11Texture2D* pBackBuffer = nullptr;
  sPtrSwapChain->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
  rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  rtvDesc.Texture2D.MipSlice = 0;
  ID3D11RenderTargetView* pRenderTargetView = nullptr;
  sPtrDeviceContext->m_pDeviceContext->CreateRenderTargetView(pBackBuffer, &rtvDesc, &pRenderTargetView);
  pBackBuffer->Release();
  m_pRenderTargetView = pRenderTargetView;
  m_pDeviceContext->m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);


}
