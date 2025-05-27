#include "cyGraphicsDX11API.h"

#define WIN32_LEAN_AND_MEAN
#include <d3dcompiler.h>
#include <d3d11_2.h>
#include <dxgi.h>

#include "cyGDX11Shader.h"
#include "cyGDX11Device.h" 
#include "cyGDX11DeviceContext.h" 
#include "cyGDX11SwapChain.h"
#include "cyGDX11Texture.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"
#include "cyGDX11InputLayout.h"
#include "cyDX11GraphicsBuffer.h"

#include <cyLogger.h>
#include <cyWindow.h>
#include <cyUtilities.h>
#include <cyResourceManager.h>
#include <cyCMeshRenderer.h>
#include <cyBBeing.h>
#include <cyRMaterial.h>
#include <cyRTexture.h>

namespace CYLLENE_SDK {

GraphicsDX11API::~GraphicsDX11API() {

}

void
GraphicsDX11API::initialize(void* pHandle) {

  // m_pWHandle = std::make_shared<void*>(pHandle);

  HWND hwnd = static_cast<HWND>(pHandle); //static_cast<HWND>(static_cast<void*>(m_pWHandle.get()));
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

  Vector<D3D_FEATURE_LEVEL> vFeatureLevels = { D3D_FEATURE_LEVEL_12_0,
                                               D3D_FEATURE_LEVEL_11_1,
                                               D3D_FEATURE_LEVEL_11_0,
                                               D3D_FEATURE_LEVEL_10_1,
                                               D3D_FEATURE_LEVEL_10_0,
                                               D3D_FEATURE_LEVEL_9_3, };

  D3D_FEATURE_LEVEL selectedFeatureLevel;

  ID3D11Device* pDevice = nullptr;
  ID3D11DeviceContext* pDeviceContext = nullptr;

  m_pDevice = std::make_shared<GDX11Device>();
  m_pDeviceContext = std::make_shared<GDX11DeviceContext>();
  m_pSwapChain = std::make_shared<GDX11SwapChain>();
  m_pRenderTargetView = std::make_shared<GDX11RenderTargetView>();
  m_pDepthStencilView = std::make_shared<GDX11DepthStencilView>();

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
    WindowManager::ShowErrorMessage("Error", Utils::format("Failed to create Device %l", hr));
    return;
  }

  pDevice->QueryInterface(__uuidof(ID3D11Device1),
  reinterpret_cast<void**>(&sPtrDevice->m_pd3d11Device));

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

  sPtrDevice->m_pd3d11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice);

  IDXGIAdapter* pDXGIAdapter = nullptr;
  pDXGIDevice->GetAdapter(&pDXGIAdapter);

  IDXGIFactory2* pFactory2 = nullptr;
  pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory2);

  SPtr<GDX11SwapChain> sPtrSwapChain = std::static_pointer_cast<GDX11SwapChain>(m_pSwapChain);

  hr =
  pFactory2->CreateSwapChainForHwnd(sPtrDevice->m_pd3d11Device,
                                    hwnd,
                                    &scDesc,
                                    nullptr,
                                    nullptr,
                                    &sPtrSwapChain->m_pSwapChain);

  pDXGIDevice->SetMaximumFrameLatency(3);

  if (FAILED(hr)) {
    WindowManager::ShowErrorMessage("Error", Utils::format("Failed to create swap Chain %l", hr));
    
    return;
  }
  queryInterface(scDesc.Width, scDesc.Height);

  GViewport vp;
  vp.setViewport(0, 0, scDesc.Width, scDesc.Height);
  m_pDeviceContext->setViewPort(1, vp);

  ResourceManager::instance().m_resourceLoaded +=
    std::bind(&GraphicsAPI::registerResource, this, std::placeholders::_1);
}


void
GraphicsDX11API::shutdown() {
  
}

// SPtr<GDevice>
// GraphicsDX11API::createDevice(const GDeviceElement& deviceParams) {
// 
// }

SPtr<GRenderTargetView>
GraphicsDX11API::createRenderTargetView(SPtr<GRenderTargetViewElement> srvParams,
												                SPtr<GTexture> texture) {
  return m_pDevice->createRenderTargetView(srvParams, texture);
}

SPtr<GDepthStencilView>
GraphicsDX11API::createDepthStencilView(SPtr<GDepthStencilViewElement> dsvParams,
                                        SPtr<GTexture> texture) {
  return m_pDevice->createDepthStencilView(dsvParams, texture);
}

SPtr<GSwapChain>
GraphicsDX11API::createSwapChain(SPtr<GDevice> device,
                                 SPtr<GSwapChainElement> swapChainParams) {
  SPtr<GDX11Device> sPtrDevice = std::static_pointer_cast<GDX11Device>(m_pDevice);
  HWND hwnd = static_cast<HWND>(static_cast<void*>(m_pWHandle.get()));

  IDXGIDevice1* pDXGIDevice = nullptr;
  
  sPtrDevice->m_pd3d11Device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDevice);

  IDXGIAdapter* pDXGIAdapter = nullptr;
  pDXGIDevice->GetAdapter(&pDXGIAdapter);

  IDXGIFactory2* pFactory2 = nullptr;
  pDXGIAdapter->GetParent(__uuidof(IDXGIFactory2), (void**)&pFactory2);

  SPtr<GDX11SwapChain> sPtrSwapChain = std::static_pointer_cast<GDX11SwapChain>(m_pSwapChain);

  DXGI_SWAP_CHAIN_DESC1 scDesc;
  memset(&scDesc, 0, sizeof(scDesc));
  RECT rc;
  GetClientRect(hwnd, &rc);
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

  HRESULT hr =
    pFactory2->CreateSwapChainForHwnd(sPtrDevice->m_pd3d11Device,
                                      hwnd,
                                      &scDesc,
                                      nullptr,
                                      nullptr,
                                      &sPtrSwapChain->m_pSwapChain);

  pDXGIDevice->SetMaximumFrameLatency(3);
  return std::static_pointer_cast<GSwapChain>(sPtrSwapChain);
}

void // SPtr<GShaderResourceView>
GraphicsDX11API::createShaderResourceView(SPtr<GTexture> shaderResourceView,
                                          SPtr<GShaderResourceViewElement> srvParams) {
  m_pDevice->createShaderResourceView(shaderResourceView, srvParams);
}

SPtr<GMesh>
GraphicsDX11API::createMesh(SPtr<RMesh> mesh) {

  // TODO: Given that this is now mostly wrapped, we may move this into the graphics branch

  SPtr<GMesh> gMesh = std::make_shared<GMesh>();
  gMesh->m_baseVertex = 0;
  gMesh->m_numVertices = mesh->m_vertexBuffer.size();

  Vector<char> vertexData;
  vertexData.resize(mesh->m_vertexBuffer.size() * sizeof(Vertex));
  memcpy(vertexData.data(), 
         mesh->m_vertexBuffer.data(), 
         mesh->m_vertexBuffer.size() * sizeof(Vertex));
  gMesh->m_pVertexBuffer = createVertexBuffer(vertexData);
  if (!gMesh->m_pVertexBuffer) {
    // TODO: Throw a warning here
  }
  gMesh->m_baseIndex = 0;
  gMesh->m_numIndices = mesh->m_indexBuffer.size();

  Vector<char> indexData;
  indexData.resize(mesh->m_indexBuffer.size() * sizeof(uint32));
  memcpy(indexData.data(), mesh->m_indexBuffer.data(), mesh->m_indexBuffer.size() * sizeof(uint32));
  gMesh->m_pIndexBuffer = createIndexBuffer(indexData);

  if (!gMesh->m_pIndexBuffer) {
    // TODO: Throw a warning here
  }

  return gMesh;
}

SPtr<GTexture>
GraphicsDX11API::createTexture2D(SPtr<RTexture> texture) {
  // return createTexture2D(texture, D3D11_BIND_SHADER_RESOURCE, 0, 1, nullptr, nullptr);
  SPtr<GTextureElement> textureParams = std::make_shared<GTextureElement>();
  textureParams->width = texture->m_img->getWidth();
  textureParams->height = texture->m_img->getHeight();
  textureParams->bindFlags = D3D11_BIND_SHADER_RESOURCE;
  textureParams->format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureParams->usage = D3D11_USAGE_DEFAULT;
  textureParams->cpuAccessFlags = 0;
  textureParams->mipLevels = 1;
  SPtr<GTexture> pTexture = createTexture2D(textureParams);
  if (pTexture) {
    GSubResourceElement subresourceParams;
    subresourceParams.data = texture->m_img->m_pixels.data();
    subresourceParams.pitch = texture->m_img->m_metadata.m_width * 16;
    subresourceParams.index = 0;
    subresourceParams.depth = 0;
    m_pDeviceContext->updateSubresource(pTexture, subresourceParams);
  }
  return pTexture;
}

// SPtr<GTexture>
// GraphicsDX11API::createTexture2D(SPtr<RTexture> texture,
//                                  uint32 bindFlags,
//                                  uint32 cpuAccessFlags,
//                                  uint32 mipFlags,
//                                  // SPtr<GShaderResourceView> ppSRV,
//                                  SPtr<GRenderTargetView> ppRTV,
//                                  SPtr<GDepthStencilView> ppDSV) {
// 
//   SPtr<GTextureElement> textureParams = std::make_shared<GTextureElement>();
//   textureParams->width = texture->m_img->getWidth();
//   textureParams->height = texture->m_img->getHeight();
//   textureParams->bindFlags = bindFlags | D3D11_BIND_SHADER_RESOURCE;
//   textureParams->format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//   textureParams->usage = D3D11_USAGE_DEFAULT;
//   textureParams->cpuAccessFlags = cpuAccessFlags;
//   textureParams->mipLevels = mipFlags;
// 
//   SPtr<GTexture> gTexture = createTexture2D(textureParams, ppRTV, ppDSV);
//   if (gTexture) {
//     GSubResourceElement data;
//     data.index = 0;
//     data.data = reinterpret_cast<void*>(texture->m_img->m_pixels.data());
//     data.depth = 0;
//     data.pitch = texture->m_img->getWidth() * 16; // TODO: When changing the size of our colors change this
//     m_pDeviceContext->updateSubresource(gTexture, data);
//   }
//   return gTexture;
// }
// 
// 
// SPtr<GTexture>
// GraphicsDX11API::createTexture2D(const Vector2i& size, 
//                                  uint32 bindFlags, 
//                                  uint32 format, 
//                                  uint32 usage, 
//                                  uint32 cpuAccessFlags /* = 0 */, 
//                                  uint32 mipFlags /* = 1 */, 
//                                  // SPtr<GShaderResourceView> ppSRV /* = nullptr */, 
//                                  SPtr<GRenderTargetView> ppRTV /* = nullptr */, 
//                                  SPtr<GDepthStencilView> ppDSV /* = nullptr */) {
//   SPtr<GTextureElement> textureParams = std::make_shared<GTextureElement>();
//   textureParams->width = size.x;
//   textureParams->height = size.y;
//   textureParams->bindFlags = bindFlags;
//   textureParams->format = format;
//   textureParams->usage = usage;
//   textureParams->cpuAccessFlags = cpuAccessFlags;
//   textureParams->mipLevels = mipFlags;
// 
//   return createTexture2D(textureParams, /*ppSRV,*/ ppRTV, ppDSV);
// }


SPtr<GTexture>
GraphicsDX11API::createTexture2D(SPtr<GTextureElement> textureParams) { //,
                                 // SPtr<GShaderResourceView> ppSRV,
                                 // SPtr<GRenderTargetView> ppRTV,
                                 // SPtr<GDepthStencilView> ppDSV) {

  return m_pDevice->createTexture2D(textureParams);
  //SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(m_pDevice->createTexture2D(textureParams));

//   if (textureParams->bindFlags & D3D11_BIND_SHADER_RESOURCE) {
//     m_pDevice->createShaderResourceView(pTexture, nullptr);
//   }
// 
//   if (ppRTV != nullptr) {
//     if (textureParams->bindFlags & D3D11_BIND_RENDER_TARGET) {
//       ppRTV = m_pDevice->createRenderTargetView(pTexture, nullptr);
//     }
//   }
// 
//   if (ppDSV != nullptr) {
//     if (textureParams->bindFlags & D3D11_BIND_DEPTH_STENCIL) {
//       ppDSV = m_pDevice->createDepthStencilView(pTexture, nullptr);
//     }
//   }

  
}

SPtr<GShaderBlob>
GraphicsDX11API::compileShader(const String& data,
                               const String& entry,
                               const String& model) {
  ID3DBlob* pBlob = nullptr;
  ID3DBlob* pErrorBlob = nullptr;
  HRESULT hr = D3DCompile(data.c_str(),
                          data.size(),
                          nullptr,
                          nullptr,
                          nullptr,
                          entry.c_str(),
                          model.c_str(),
                          D3DCOMPILE_ENABLE_STRICTNESS,
                          0,
                          &pBlob,
                          &pErrorBlob);

  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::make_shared<GDX11ShaderBlob>();
  
  if (FAILED(hr)) {
    if (pErrorBlob) {
      sPtrShaderBlob->error = static_cast<const char*>(pErrorBlob->GetBufferPointer());
      sPtrShaderBlob->size = pErrorBlob->GetBufferSize();
      sPtrShaderBlob->isCompiled = false;
      MessageBoxA(nullptr, static_cast<const char*>(pErrorBlob->GetBufferPointer()), "Error", MB_OK);
      pErrorBlob->Release();
    }
    return sPtrShaderBlob;
  }
  sPtrShaderBlob->size = pBlob->GetBufferSize();
  sPtrShaderBlob->entry = entry;
  sPtrShaderBlob->model = model;
  sPtrShaderBlob->type = 0; // TODO: Make something to define the type of shader
  sPtrShaderBlob->data = data;
  sPtrShaderBlob->isCompiled = true;
  sPtrShaderBlob->m_pBlob = pBlob;
  return std::static_pointer_cast<GShaderBlob>(sPtrShaderBlob);
}

SPtr<GVertexShader>
GraphicsDX11API::createVertexShader(SPtr<RShader> shader, 
                                    const String& entry) {
  SPtr<GShaderBlob> sPtrShaderBlob = compileShader(shader->m_data, entry, "vs_5_0");

  if (!sPtrShaderBlob->isCompiled) {
    return nullptr;
  }

  SPtr<GVertexShader> sPtrShader = m_pDevice->createVertexShader(sPtrShaderBlob);

  return sPtrShader;
}

SPtr<GPixelShader>
GraphicsDX11API::createPixelShader(SPtr<RShader> shader,
                                   const String& entry) {
  SPtr<GShaderBlob> sPtrShaderBlob = compileShader(shader->m_data, entry, "ps_5_0");

  if (!sPtrShaderBlob->isCompiled) {
    return nullptr;
  }

  SPtr<GPixelShader> sPtrShader = m_pDevice->createPixelShader(sPtrShaderBlob);

  return sPtrShader;
}

SPtr<GGeometryShader>
GraphicsDX11API::createGeometryShader(SPtr<RShader> shader,
                                      const String& entry) {
  SPtr<GShaderBlob> sPtrShaderBlob = compileShader(shader->m_data, entry, "gs_5_0");
  if (!sPtrShaderBlob->isCompiled) {
    return nullptr;
  }
  SPtr<GGeometryShader> sPtrShader = m_pDevice->createGeometryShader(sPtrShaderBlob);
  return sPtrShader;
}

SPtr<GComputeShader>
GraphicsDX11API::createComputeShader(SPtr<RShader> shader,
                                     const String& entry) {
  SPtr<GShaderBlob> sPtrShaderBlob = compileShader(shader->m_data, entry, "cs_5_0");
  if (!sPtrShaderBlob->isCompiled) {
    return nullptr;
  }
  SPtr<GComputeShader> sPtrShader = m_pDevice->createComputeShader(sPtrShaderBlob);
  return sPtrShader;
}

SPtr<GShader>
GraphicsDX11API::createShader(SPtr<RShader> shader,
                              const String& entry) {
  String model;
  SPtr<GShaderBlob> sPtrShaderBlob = compileShader(shader->m_data, entry, model);
  if (!sPtrShaderBlob->isCompiled) {
    return nullptr;
  }
  SPtr<GShader> sPtrShader = m_pDevice->createShader(sPtrShaderBlob);
  return sPtrShader;
}


SPtr<GInputLayout>
GraphicsDX11API::createInputLayout(const Vector<GInputLayoutElement>& descriptor, 
                                   SPtr<GVertexShader> desc) {
  
  if (descriptor.empty()) { // || pVertexShader.expired()) {
    return nullptr;
  }

  return m_pDevice->createInputLayout(descriptor, desc);

}

SPtr<GraphicsBuffer>
GraphicsDX11API::createVertexBuffer(const Vector<char>& data) {
  
  SPtr<GBufferElement> bufferParams = std::make_shared<GBufferElement>();
  bufferParams->data = data;
  bufferParams->byteSize = data.size();
  bufferParams->usage = D3D11_USAGE_DEFAULT;
  bufferParams->bindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferParams->cpuAccessFlags = 0;
  SPtr<GraphicsBuffer> pBuffer = m_pDevice->createGraphicsBuffer(bufferParams);
  return pBuffer;
}

SPtr<GraphicsBuffer>
GraphicsDX11API::createIndexBuffer(const Vector<char>& data) {

  SPtr<GBufferElement> bufferParams = std::make_shared<GBufferElement>();
  bufferParams->data = data;
  bufferParams->byteSize = data.size();
  bufferParams->usage = D3D11_USAGE_DEFAULT;
  bufferParams->bindFlags = D3D11_BIND_INDEX_BUFFER;
  bufferParams->cpuAccessFlags = 0;
  SPtr<GraphicsBuffer> pBuffer = m_pDevice->createGraphicsBuffer(bufferParams);
  return pBuffer;
}

SPtr<GraphicsBuffer>
GraphicsDX11API::createConstantBuffer(const Vector<char>& data) {
  SPtr<GBufferElement> bufferParams = std::make_shared<GBufferElement>();
  bufferParams->data = data;
  bufferParams->byteSize = data.size();
  bufferParams->usage = D3D11_USAGE_DEFAULT;
  bufferParams->bindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bufferParams->cpuAccessFlags = 0;
  SPtr<GraphicsBuffer> pBuffer = m_pDevice->createGraphicsBuffer(bufferParams);
  return pBuffer;
}

void
GraphicsDX11API::writeToBuffer(const SPtr<GraphicsBuffer>& buffer,
                               const Vector<char>& data) {
  SPtr<GDX11DeviceContext> pDeviceContext = std::static_pointer_cast<GDX11DeviceContext>(m_pDeviceContext);
  SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffer);
  // D3D11_MAPPED_SUBRESOURCE mappedResource;
  // pDeviceContext->m_pDeviceContext->Map(pBuffer->m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  // memcpy(mappedResource.pData, data.data(), data.size());
  // pDeviceContext->m_pDeviceContext->Unmap(pBuffer->m_pBuffer, 0);
  pDeviceContext->m_pDeviceContext->UpdateSubresource1(pBuffer->m_pBuffer, 0, nullptr, data.data(), 0, 0, 0);


}

void
GraphicsDX11API::queryInterface(int32 width, int32 height) {

  if (!m_pSwapChain) {
    return;
  }

  SPtr<GDX11Device> pDevice = std::static_pointer_cast<GDX11Device>(m_pDevice);

  // SPtr<GDX11RenderTargetView> pRenderTargetView = std::static_pointer_cast<GDX11RenderTargetView>(m_pRenderTargetView);

  SPtr<GTexture> pBackBuffer = m_pSwapChain->getBuffer(0);

  // pDevice->m_pDevice->CreateRenderTargetView(pBackBuffer->m_texture, nullptr, &pRenderTargetView->m_pRTV);
  // SPtr<GRenderTargetViewElement> pRTVParams = std::make_shared<GRenderTargetViewElement>();
  m_pRenderTargetView = createRenderTargetView(nullptr, pBackBuffer);
  // m_pRenderTargetView = pDevice->createRenderTargetView(pBackBuffer, nullptr);

  // TODO ?
  DX11_SAFE_RELEASE(std::static_pointer_cast<GDX11Texture>(pBackBuffer)->m_texture);



  // TODO: Change this to the new format
  //////////////////////////////////////////////////////////////////////////
  SPtr<GTextureElement> textureParams = std::make_shared<GTextureElement>();
  textureParams->width = width;
  textureParams->height = height;
  textureParams->cpuAccessFlags = 0;
  textureParams->mipLevels = 1;
  textureParams->format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  textureParams->usage = D3D11_USAGE_DEFAULT;
  textureParams->bindFlags = D3D11_BIND_DEPTH_STENCIL;
  SPtr<GTexture> pDepthStencil = createTexture2D(textureParams);

  m_pDepthStencilView = createDepthStencilView(nullptr, pDepthStencil);

  // SPtr<GDX11Texture> pDepthStencil = 
  //   std::static_pointer_cast<GDX11Texture>(createTexture2D(Vector2i(width, height),
  //                                                          D3D11_BIND_DEPTH_STENCIL,
  //                                                          DXGI_FORMAT_D24_UNORM_S8_UINT,
  //                                                          D3D11_USAGE_DEFAULT));
  // 
  if (!m_pDepthStencilView) {
    MessageBox(nullptr, "Failed to create depth stencil", "Error", MB_OK);
    return;
  }

  // m_pDepthStencilView = m_pDevice->createDepthStencilView(pDepthStencil, nullptr);
  //////////////////////////////////////////////////////////////////////////

  DX11_SAFE_RELEASE(std::static_pointer_cast<GDX11Texture>(pDepthStencil)->m_texture);
}


void
GraphicsDX11API::queryInterface(const Vector2i & size) {
  return queryInterface(size.x, size.y);
}

void
GraphicsDX11API::clear(const Color& color) {
  m_pDeviceContext->clearRenderTargetView(m_pRenderTargetView, color);
  m_pDeviceContext->clearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void
GraphicsDX11API::draw(SPtr<CCamera> refCamera, SPtr<BBeing> refBeing) {
  // TODO: Some of this objects are still placeholders
  struct MatrixCollection
  {
    Matrix4 world;
    Matrix4 view;
    Matrix4 projection;
  } matrices;
  // Get reference to textures needed and meshes needed
  // Vector<SPtr<GTexture>> textures = refBeing->getComponent<CMeshRenderer>();
  // 
  // 
  // Vector<SPtr<GraphicsBuffer>> vertexBuffer;
  // vertexBuffer.push_back(refMesh->m_pVertexBuffer);
  // 
  // Vector<uint32> vertexStrides;
  // vertexStrides.push_back(vertexStride);
  // 
  // Vector<uint32> vertexOffsets;
  // vertexOffsets.push_back(vertexOffset);
  // 
  // GraphicsDX11API::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);
  // 
  // GraphicsDX11API::instance().getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
  // 
  // Vector<SPtr<GShaderResourceView>> srvVector;
  // srvVector.push_back(refTexture->getResource());
  // 
  // GraphicsDX11API::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);
  // 
  // Vector<SPtr<GSamplerState>> ssVec1;
  // Vector<SPtr<GSamplerState>> ssVec2;
  // ssVec1.push_back(pointSampler);
  // ssVec2.push_back(linearSampler);
  // GraphicsDX11API::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
  // GraphicsDX11API::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);
  // 
  // matrices.world = refObject.getTransform()->m_tMatrix;
  // matrices.view = refCamera->m_view;
  // matrices.projection = refCamera->m_projection;
  // matrices.world.transpose();
  // 
  // matrixData.clear();
  // matrixData.resize(sizeof(matrices));
  // memcpy(matrixData.data(), &matrices, sizeof(matrices));
  // 
  // GraphicsDX11API::instance().writeToBuffer(matrixConstantBuffer, matrixData);
  // Vector<SPtr<GraphicsBuffer>> gbVector;
  // gbVector.push_back(matrixConstantBuffer);
  // GraphicsDX11API::instance().getDeviceContext()->setConstantBuffer(1, 1, gbVector);
  // 
  // GraphicsDX11API::instance().getDeviceContext()->drawIndexed(refMesh);
}

void
GraphicsDX11API::present() {
  SPtr<GDX11SwapChain> pSwapChain = std::static_pointer_cast<GDX11SwapChain>(m_pSwapChain);
  pSwapChain->m_pSwapChain->Present(0, 0);
}

void
GraphicsDX11API::setViewport(int32 x, int32 y, int32 width, int32 height) {
  D3D11_VIEWPORT vp;
  vp.TopLeftX = static_cast<float>(x);
  vp.TopLeftY = static_cast<float>(y);
  vp.Width = static_cast<float>(width);
  vp.Height = static_cast<float>(height);
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  SPtr<GDX11DeviceContext> pDeviceContext = std::static_pointer_cast<GDX11DeviceContext>(m_pDeviceContext);
  pDeviceContext->m_pDeviceContext->RSSetViewports(1, &vp);
}

void
GraphicsDX11API::setViewport(const Rect& rect) {
  setViewport(rect.x, rect.y, rect.width, rect.height);
}


MS_ALIGN(16)
extern "C" CY_DX11_EXPORT GraphicsAPI*
createPluginAPI() {
  return new GraphicsDX11API();
}
GCC_ALIGN(16)


}