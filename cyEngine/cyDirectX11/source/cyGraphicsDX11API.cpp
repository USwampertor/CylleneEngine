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

  SPtr<GDX11Device> sPtrDevice = std::make_shared<GDX11Device>();
  SPtr<GDX11Device> sPtrDevice = std::make_shared<GDX11Device>();

}

}
