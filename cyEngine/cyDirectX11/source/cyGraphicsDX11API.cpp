#include "cyGraphicsDX11API.h"

#define WIN32_LEAN_AND_MEAN
#include <d3dcompiler.h>
#include <d3d11_2.h>
#include <dxgi.h>

#include "cyGDX11Device.h" 
#include "cyGDX11DeviceContext.h" 

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

}

}
