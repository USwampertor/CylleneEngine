#include "cyGDX11Device.h"
#include "cyGDX11Texture.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"
#include "cyGDX11InputLayout.h"
#include "cyGDX11Shader.h"
#include "cyDX11GraphicsBuffer.h"
#include "cyGDX11RasterizerState.h"
#include "cyGDX11ShaderResourceView.h"
#include "cyGDX11SamplerState.h"

#include <cyWindow.h>
#include <cyRTexture.h>

namespace CYLLENE_SDK {

GDX11Device::~GDX11Device() {
  DX11_SAFE_RELEASE(m_pd3d11Device);
}
void* GDX11Device::get() {
  return m_pd3d11Device;
}

void GDX11Device::set(void* pHandle) {
  m_pd3d11Device = static_cast<ID3D11Device1*>(pHandle);
}

void 
GDX11Device::queryInterface(SPtr<GSwapChain> swapChain, 
                            SPtr<GDepthStencilView> depthStencil, 
                            SPtr<GRenderTargetView> renderTargetView, 
                            int32 width, 
                            int32 height) {
  // Implementation of queryInterface for DirectX 11
  // This typically involves setting up the swap chain and render target views
  // based on the provided parameters.
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
GDX11Device::createDepthStencilView(// SPtr<GTexture> depthStencilView,
                                    SPtr<GDepthStencilViewElement> dsvParams,
                                    SPtr<GTexture> texture) {

  D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = dsvParams != nullptr ? new CD3D11_DEPTH_STENCIL_VIEW_DESC() : nullptr;

  if (dsvDesc != nullptr) {
    // D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
    // dsvDesc->Flags = 0;
    dsvDesc->Format = static_cast<DXGI_FORMAT>(dsvParams->format);
    dsvDesc->Texture2D.MipSlice = 0;
    dsvDesc->ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc->Flags = 0;
  }

  SPtr<GDX11DepthStencilView> pDepthStencilView = std::make_shared<GDX11DepthStencilView>();

  // TODO: Make some standard values for a Depth Stencil texture
  SPtr<GTextureElement> pTextureElement = std::make_shared<GTextureElement>();
  if (dsvParams != nullptr) {
    pTextureElement->width = dsvParams->width > 0 ? dsvParams->width : 128;
    pTextureElement->height = dsvParams->height > 0 ? dsvParams->height : 128;
    pTextureElement->mipLevels = dsvParams->mipLevels > 0 ? dsvParams->mipLevels : 1;
    pTextureElement->format = dsvParams->format > 0 ? dsvParams->format : DXGI_FORMAT_D24_UNORM_S8_UINT;
    pTextureElement->bindFlags = dsvParams->flags > 0 ? dsvParams->flags : D3D11_BIND_DEPTH_STENCIL;
  }
  else {
    pTextureElement->width = 1;
    pTextureElement->height = 1;
    pTextureElement->mipLevels = 1;
    pTextureElement->format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    pTextureElement->bindFlags = D3D11_BIND_DEPTH_STENCIL;
  }

  pTextureElement->cpuAccessFlags = 0;
  pTextureElement->usage = D3D11_USAGE_DEFAULT;

  pDepthStencilView->m_pTexture = (texture == nullptr) ?
    std::static_pointer_cast<GDX11Texture>(createTexture2D(pTextureElement)) :
    std::static_pointer_cast<GDX11Texture>(texture);

  // SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(depthStencilView);

  if (FAILED(m_pd3d11Device->CreateDepthStencilView(pDepthStencilView->m_pTexture->m_texture, dsvDesc, &pDepthStencilView->m_pDSV))) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Depth Stencil view");
    return nullptr;
  }
  return std::static_pointer_cast<GDepthStencilView>(pDepthStencilView);
}

SPtr<GRenderTargetView>
GDX11Device::createRenderTargetView(SPtr<GRenderTargetViewElement> rtvParams,
                                    SPtr<GTexture> pTexture) {
  D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc = rtvParams ? new CD3D11_RENDER_TARGET_VIEW_DESC() : nullptr;
  
  bool requieresDepthStencil = false;
  if (rtvParams != nullptr) {
    if (rtvParams->flags & D3D11_BIND_DEPTH_STENCIL) {
      requieresDepthStencil = true;

      rtvParams->flags = (rtvParams->flags & ~D3D11_BIND_DEPTH_STENCIL);
    }
  }
  
  if (rtvDesc != nullptr) {
    // D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
    rtvDesc->Format = static_cast<DXGI_FORMAT>(rtvParams->format);
    rtvDesc->Texture2D.MipSlice = 0;
    rtvDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  }

  SPtr<GDX11RenderTargetView> pRenderTargetView = std::make_shared<GDX11RenderTargetView>();
  
  // TODO: Make some standard values for a Render Target texture
  SPtr<GTextureElement> pTextureElement = std::make_shared<GTextureElement>();
  
  if (rtvParams != nullptr) {
    pTextureElement->width = rtvParams->width > 0 ? rtvParams->width : 128;
    pTextureElement->height = rtvParams->height > 0 ? rtvParams->height : 128;
    pTextureElement->cpuAccessFlags = 0;
    pTextureElement->mipLevels = rtvParams->mipLevels > 0 ? rtvParams->mipLevels : 1;
    pTextureElement->format = rtvParams->format > 0 ? static_cast<DXGI_FORMAT>(rtvParams->format) : DXGI_FORMAT_R8G8B8A8_UNORM;
    pTextureElement->usage = D3D11_USAGE_DEFAULT;
    pTextureElement->bindFlags = rtvParams->flags > 0 ? rtvParams->flags : D3D11_BIND_RENDER_TARGET;
  }

  pRenderTargetView->m_pTexture = (pTexture == nullptr) ? 
                                  std::static_pointer_cast<GDX11Texture>(createTexture2D(pTextureElement)) : 
                                  std::static_pointer_cast<GDX11Texture>(pTexture);
  
  SPtr<GDepthStencilViewElement> dsvparams = std::make_shared<GDepthStencilViewElement>();
  if (requieresDepthStencil) {
    dsvparams->width = rtvParams->width > 0 ? rtvParams->width : 128;
    dsvparams->height = rtvParams->height > 0 ? rtvParams->height : 128;
    dsvparams->format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvparams->viewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvparams->mipLevels = rtvParams->mipLevels > 0 ? rtvParams->mipLevels : 1;
    dsvparams->flags = 0;
  }
  pRenderTargetView->m_pDSV = std::static_pointer_cast<GDX11DepthStencilView>(createDepthStencilView(dsvparams));

  // SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(renderTargetView);
  if (FAILED(m_pd3d11Device->CreateRenderTargetView(pRenderTargetView->m_pTexture->m_texture, rtvDesc, &pRenderTargetView->m_pRTV))) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Render target view");
    return nullptr;
  }

  return std::static_pointer_cast<GRenderTargetView>(pRenderTargetView);
}

SPtr<GTexture>
GDX11Device::createTexture2D(SPtr<GTextureElement> textureParams) {

  SPtr<GDX11Texture> pTexture = std::make_shared<GDX11Texture>();
  memset(&pTexture->m_texture, 0, sizeof(pTexture->m_texture));

  D3D11_TEXTURE2D_DESC* desc = textureParams != nullptr ? new CD3D11_TEXTURE2D_DESC : nullptr;

  if (desc != nullptr) {
    desc->ArraySize = 1;
    desc->BindFlags = textureParams->bindFlags;
    desc->CPUAccessFlags = textureParams->cpuAccessFlags;
    desc->Format = static_cast<DXGI_FORMAT>(textureParams->format);
    desc->Height = textureParams->height;
    desc->Width = textureParams->width;
    desc->MipLevels = textureParams->mipLevels;
    desc->MiscFlags = 0;
    desc->SampleDesc.Count = 1; //MSAA
    desc->SampleDesc.Quality = 0;
    desc->Usage = static_cast<D3D11_USAGE>(textureParams->usage);
  }
  if (FAILED(m_pd3d11Device->CreateTexture2D(desc, nullptr, &pTexture->m_texture))) {
    return nullptr;
  }

  // TODO: Create shader resource view in case of flag on
  if (textureParams->bindFlags & D3D11_BIND_SHADER_RESOURCE) {
    SPtr<GShaderResourceViewElement> pSRVParams = std::make_shared<GShaderResourceViewElement>();

    pSRVParams->format = textureParams->format;
    pSRVParams->mipLevels = textureParams->mipLevels == 1 ? 1 : -1;

    createShaderResourceView(pTexture, pSRVParams);
  }

  return std::static_pointer_cast<GTexture>(pTexture);
}

SPtr<GVertexShader>
GDX11Device::createVertexShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);
  
  SPtr<GDX11VertexShader> sPtrShader = std::make_shared<GDX11VertexShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;
  
  HRESULT hr = m_pd3d11Device->CreateVertexShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                             sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                             nullptr,
                                             &sPtrShader->m_pVertexShader);


  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Vertex Shader");
    return nullptr;
  }

  return std::static_pointer_cast<GVertexShader>(sPtrShader);
}

SPtr<GPixelShader>
GDX11Device::createPixelShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);

  SPtr<GDX11PixelShader> sPtrShader = std::make_shared<GDX11PixelShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;

  HRESULT hr = m_pd3d11Device->CreatePixelShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                                 sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                                 nullptr,
                                                 &sPtrShader->m_pPixelShader);


  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Pixel Shader");
    return nullptr;
  }

  return std::static_pointer_cast<GPixelShader>(sPtrShader);
}

SPtr<GGeometryShader>
GDX11Device::createGeometryShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);

  SPtr<GDX11GeometryShader> sPtrShader = std::make_shared<GDX11GeometryShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;

  HRESULT hr = m_pd3d11Device->CreateGeometryShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                                    sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                                    nullptr,
                                                    &sPtrShader->m_pGeometryShader);


  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Geometry Shader");
    return nullptr;
  }

  return std::static_pointer_cast<GGeometryShader>(sPtrShader);
}

SPtr<GComputeShader>
GDX11Device::createComputeShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);

  SPtr<GDX11ComputeShader> sPtrShader = std::make_shared<GDX11ComputeShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;

  HRESULT hr = m_pd3d11Device->CreateComputeShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                                   sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                                   nullptr,
                                                   &sPtrShader->m_pComputeShader);


  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Compute Shader");
    return nullptr;
  }

  return std::static_pointer_cast<GComputeShader>(sPtrShader);
}

SPtr<GShader>
GDX11Device::createShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);

  return nullptr;

  // TODO: Implement this so this function is more agnostic-like
  // The idea is that this function may take information from the blob to know what
  // type of shader to create

//   SPtr<GDX11ComputeShader> sPtrShader = std::make_shared<GDX11ComputeShader>();
//   sPtrShader->m_pBlob = sPtrShaderBlob;
// 
//   HRESULT hr = m_pd3d11Device->CreateComputeShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
//                                                    sPtrShaderBlob->m_pBlob->GetBufferSize(),
//                                                    nullptr,
//                                                    &sPtrShader->m_pComputeShader);
// 
// 
//   if (FAILED(hr)) {
//     WindowManager::instance().ShowErrorMessage("Error", "Error creating Compute Shader");
//     return nullptr;
//   }
// 
//   return std::static_pointer_cast<GComputeShader>(sPtrShader);
}


void// SPtr<GShaderResourceView>
GDX11Device::createShaderResourceView(SPtr<GTexture> shaderResourceView,
                                      SPtr<GShaderResourceViewElement> srvParams) {
  
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
  if (srvParams->format == DXGI_FORMAT_R32_TYPELESS) {
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
  }
  else {
    srvDesc.Format = static_cast<DXGI_FORMAT>(srvParams->format);
  }
  srvDesc.Texture2D.MipLevels = srvParams->mipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(shaderResourceView);
  pTexture->m_pSRV = std::make_shared<GDX11ShaderResourceView>();


  HRESULT hr = m_pd3d11Device->CreateShaderResourceView(pTexture->m_texture, &srvDesc, &pTexture->m_pSRV->m_pd3d11SRV);
  
  if (FAILED(hr)) {
    // TODO: Show an error here
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Shader resource view");
    pTexture->m_pSRV = nullptr; // pShaderResourceView->m_pSRV;
  }
}


SPtr<GInputLayout>
GDX11Device::createInputLayout(const Vector<GInputLayoutElement>& descriptor,
                               SPtr<GVertexShader> desc) {

  SPtr<GDX11InputLayout> sPtrInputLayout = std::make_shared<GDX11InputLayout>();

  Vector<D3D11_INPUT_ELEMENT_DESC> d3d11Descriptor;

  for (const auto& element : descriptor) {
    D3D11_INPUT_ELEMENT_DESC desc;
    desc.SemanticName = element.semanticName.c_str();
    desc.SemanticIndex = element.semanticIndex;
    desc.Format = static_cast<DXGI_FORMAT>(element.format);
    desc.InputSlot = element.inputSlot;
    desc.AlignedByteOffset = element.alignedByteOffset;
    desc.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(element.inputSlotClass);
    desc.InstanceDataStepRate = element.instanceDataStepRate;
    d3d11Descriptor.push_back(desc);
  }

  SPtr<GDX11VertexShader> sPtrVertexShader = std::static_pointer_cast<GDX11VertexShader>(desc);

  HRESULT hr = m_pd3d11Device->CreateInputLayout(d3d11Descriptor.data(),
                                            d3d11Descriptor.size(),
                                /*pShader*/ sPtrVertexShader->m_pBlob->m_pBlob->GetBufferPointer(),
                                /*pShader*/ sPtrVertexShader->m_pBlob->m_pBlob->GetBufferSize(),
                                            &sPtrInputLayout->m_pInputLayout);
  
  
  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Input layout");
    return nullptr;
  }
  return std::static_pointer_cast<GInputLayout>(sPtrInputLayout);
}


SPtr<GraphicsBuffer>
GDX11Device::createGraphicsBuffer(SPtr<GBufferElement> bufferParams) {
  
  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));

  desc.Usage = static_cast<D3D11_USAGE>(bufferParams->usage);
  desc.ByteWidth = bufferParams->byteSize;
  desc.BindFlags = bufferParams->bindFlags;
  desc.CPUAccessFlags = bufferParams->cpuAccessFlags;
  desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA initData;
  initData.pSysMem = bufferParams->data.data();
  initData.SysMemPitch = 0;
  initData.SysMemSlicePitch = 0;
  
  SPtr<DX11GraphicsBuffer> pBuffer = std::make_shared<DX11GraphicsBuffer>();

  HRESULT hr = m_pd3d11Device->CreateBuffer(&desc, &initData, &pBuffer->m_pBuffer);


  if (FAILED(hr)) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating graphics buffer");
    return nullptr;
  }

  return std::static_pointer_cast<GraphicsBuffer>(pBuffer);
}

SPtr<GSamplerState>
GDX11Device::createSamplerState(SPtr<GSamplerStateElement> samplerParams) {
  CD3D11_SAMPLER_DESC descSS(D3D11_DEFAULT);

  SPtr<GDX11SamplerState> pSamplerState = std::make_shared<GDX11SamplerState>();

  if (samplerParams != nullptr) {
    descSS.Filter = samplerParams->filter == SAMPLERFILTER::E::ePOINT ? D3D11_FILTER_MIN_MAG_MIP_POINT :
                    samplerParams->filter == SAMPLERFILTER::E::eLINEAR ? D3D11_FILTER_MIN_MAG_MIP_LINEAR :
                    D3D11_FILTER_ANISOTROPIC;
    descSS.AddressU = samplerParams->addressU == TEXTUREMODE::E::eWRAP ? D3D11_TEXTURE_ADDRESS_WRAP :
                      samplerParams->addressU == TEXTUREMODE::E::eMIRROR ? D3D11_TEXTURE_ADDRESS_MIRROR :
                      samplerParams->addressU == TEXTUREMODE::E::eCLAMP ? D3D11_TEXTURE_ADDRESS_CLAMP :
                      D3D11_TEXTURE_ADDRESS_BORDER;

    descSS.AddressV = samplerParams->addressV == TEXTUREMODE::E::eWRAP ? D3D11_TEXTURE_ADDRESS_WRAP :
                      samplerParams->addressV == TEXTUREMODE::E::eMIRROR ? D3D11_TEXTURE_ADDRESS_MIRROR :
                      samplerParams->addressV == TEXTUREMODE::E::eCLAMP ? D3D11_TEXTURE_ADDRESS_CLAMP :
                      D3D11_TEXTURE_ADDRESS_BORDER;
    
    descSS.AddressW = samplerParams->addressW == TEXTUREMODE::E::eWRAP ? D3D11_TEXTURE_ADDRESS_WRAP :
                      samplerParams->addressW == TEXTUREMODE::E::eMIRROR ? D3D11_TEXTURE_ADDRESS_MIRROR :
                      samplerParams->addressW == TEXTUREMODE::E::eCLAMP ? D3D11_TEXTURE_ADDRESS_CLAMP :
                      D3D11_TEXTURE_ADDRESS_BORDER;

    descSS.MipLODBias = samplerParams->mipLODBias;
    descSS.MaxAnisotropy = samplerParams->maxAnisotropy;
    descSS.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(samplerParams->comparisonFunc);
    descSS.BorderColor[0] = samplerParams->borderColor[0];
    descSS.BorderColor[1] = samplerParams->borderColor[1];
    descSS.BorderColor[2] = samplerParams->borderColor[2];
    descSS.BorderColor[3] = samplerParams->borderColor[3];
    descSS.MinLOD = samplerParams->minLOD;
    descSS.MaxLOD = samplerParams->maxLOD;
  }

  
  if (FAILED(m_pd3d11Device->CreateSamplerState(&descSS, &pSamplerState->m_pSamplerState))) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Sampler State");
    return nullptr;
  }
  return std::static_pointer_cast<GSamplerState>(pSamplerState);

}

SPtr<GRasterizerState>
GDX11Device::createRasterizerState(SPtr<GRasterizerElement> rasterizerParams) {
  CD3D11_RASTERIZER_DESC1 descRD(D3D11_DEFAULT);

  if (rasterizerParams != nullptr) {
    descRD.FillMode = static_cast<D3D11_FILL_MODE>(rasterizerParams->fillMode);
    descRD.CullMode = static_cast<D3D11_CULL_MODE>(rasterizerParams->cullMode);
    descRD.FrontCounterClockwise = rasterizerParams->counterWise;
    descRD.DepthBias = rasterizerParams->depthBias;
    descRD.DepthBiasClamp = rasterizerParams->depthBiasClamp;
    descRD.SlopeScaledDepthBias = rasterizerParams->slopScaledDepthBias;
    descRD.DepthClipEnable = rasterizerParams->clipEnabled;
    descRD.ScissorEnable = rasterizerParams->scissorEnable;
    descRD.MultisampleEnable = rasterizerParams->multisampleEnable;
    descRD.AntialiasedLineEnable = rasterizerParams->antialiasedLineEnable;
    descRD.ForcedSampleCount = rasterizerParams->forcedSampleCount;
  }


  SPtr<GDX11RasterizerState> pRasterizerState = std::make_shared<GDX11RasterizerState>();

  // m_pDevice->CreateRasterizerState1(&descRD, &pRasterizerState->m_pRasterizerState);
  if (FAILED(m_pd3d11Device->CreateRasterizerState1(&descRD, &pRasterizerState->m_pRasterizerState))) {
    WindowManager::instance().ShowErrorMessage("Error", "Error creating Rasterizer State");
    return nullptr;
  }
  return std::static_pointer_cast<GRasterizerState>(pRasterizerState);
}

}

