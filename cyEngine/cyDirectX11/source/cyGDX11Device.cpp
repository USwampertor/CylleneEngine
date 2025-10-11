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
#include "cyGDX11BlendState.h"
#include "cyDirectXHelpers.h"

#include <cyWindow.h>
#include <cyRTexture.h>

namespace CYLLENE_SDK {

namespace {
  static uint32 sanitizeDepthFormat(uint32 format) {
    if (!COLORFORMAT::E::_is_valid(format)) {
      return COLORFORMAT::E::D_24_UNORM_S8_UINT;
    }
    return format;
  }

  static bool isDepthStencilFormat(uint32 format) {
    if (!COLORFORMAT::E::_is_valid(format)) {
      return false;
    }

    switch (COLORFORMAT::E::_from_integral_unchecked(format)) {
    case COLORFORMAT::E::D_32_FLOAT:
    case COLORFORMAT::E::D_32_UINT:
    case COLORFORMAT::E::D_32_SINT:
    case COLORFORMAT::E::D_32_TYPELESS:
    case COLORFORMAT::E::R_32_TYPELESS:
    case COLORFORMAT::E::D_24_UNORM_S8_UINT:
    case COLORFORMAT::E::D_8_FLOAT:
    case COLORFORMAT::E::D_8_UINT:
    case COLORFORMAT::E::D_8_SINT:
    case COLORFORMAT::E::D_8_TYPELESS:
    case COLORFORMAT::E::D_8_UNORM:
      return true;
    default:
      return false;
    }
  }

  static bool isRenderableColorFormat(uint32 format) {
    if (!COLORFORMAT::E::_is_valid(format)) {
      return false;
    }
    return !isDepthStencilFormat(format);
  }

  static uint32 resolveDepthTextureFormat(uint32 format, uint32 bindFlags) {
    const uint32 sanitized = sanitizeDepthFormat(format);
    const bool wantsSRV = (bindFlags & GRESOURCE_BINDINGS::E::SHADERRESOURCE) != 0;

    switch (COLORFORMAT::E::_from_integral_unchecked(sanitized)) {
    case COLORFORMAT::E::D_32_FLOAT:
      return wantsSRV ? COLORFORMAT::E::R_32_TYPELESS : sanitized;
    case COLORFORMAT::E::D_32_UINT:
    case COLORFORMAT::E::D_32_SINT:
    case COLORFORMAT::E::D_32_TYPELESS:
    case COLORFORMAT::E::R_32_TYPELESS:
      return wantsSRV ? COLORFORMAT::E::R_32_TYPELESS : COLORFORMAT::E::D_32_FLOAT;
    case COLORFORMAT::E::D_8_FLOAT:
    case COLORFORMAT::E::D_8_UINT:
    case COLORFORMAT::E::D_8_SINT:
    case COLORFORMAT::E::D_8_TYPELESS:
    case COLORFORMAT::E::D_8_UNORM:
      return COLORFORMAT::E::D_24_UNORM_S8_UINT;
    default:
      return sanitized;
    }
  }

  static uint32 resolveDepthViewFormat(uint32 format) {
    const uint32 sanitized = sanitizeDepthFormat(format);

    switch (COLORFORMAT::E::_from_integral_unchecked(sanitized)) {
    case COLORFORMAT::E::R_32_TYPELESS:
    case COLORFORMAT::E::D_32_TYPELESS:
    case COLORFORMAT::E::D_32_UINT:
    case COLORFORMAT::E::D_32_SINT:
      return COLORFORMAT::E::D_32_FLOAT;
    case COLORFORMAT::E::D_8_FLOAT:
    case COLORFORMAT::E::D_8_UINT:
    case COLORFORMAT::E::D_8_SINT:
    case COLORFORMAT::E::D_8_TYPELESS:
    case COLORFORMAT::E::D_8_UNORM:
      return COLORFORMAT::E::D_24_UNORM_S8_UINT;
    default:
      return sanitized;
    }
  }

  static uint32 resolveColorFormat(uint32 format) {
    return isRenderableColorFormat(format) ? format : COLORFORMAT::E::RGBA_8_UNORM;
  }
}


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

  const uint32 requestedFormat = dsvParams ? dsvParams->format : COLORFORMAT::E::D_24_UNORM_S8_UINT;
  const uint32 bindFlags = (dsvParams && dsvParams->flags > 0) ? dsvParams->flags : GRESOURCE_BINDINGS::E::DEPTHSTENCIL;
  const uint32 viewFormat = resolveDepthViewFormat(requestedFormat);
  const uint32 textureFormat = resolveDepthTextureFormat(texture ? requestedFormat : viewFormat, bindFlags);

  if (dsvParams != nullptr) {
    dsvParams->format = viewFormat;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = dsvParams != nullptr ? new CD3D11_DEPTH_STENCIL_VIEW_DESC() : nullptr;

  if (dsvDesc != nullptr) {
    dsvDesc->Format = colorFormatToDXGI(viewFormat);
    dsvDesc->Texture2D.MipSlice = 0;
        dsvDesc->ViewDimension = textureDimensionToD3D11DSVDIMENSION(GDSV_DIMENSION::E::_from_integral_unchecked(dsvParams && dsvParams->viewDimension ? dsvParams->viewDimension : GDSV_DIMENSION::E::TEXTURE2D));
    dsvDesc->Flags = 0;
  }

  SPtr<GDX11DepthStencilView> pDepthStencilView = std::make_shared<GDX11DepthStencilView>();

  SPtr<GTextureElement> pTextureElement = std::make_shared<GTextureElement>();
  const uint32 defaultDSWidth = (dsvParams == nullptr) ? 1u : 128u;
  const uint32 defaultDSHeight = (dsvParams == nullptr) ? 1u : 128u;
  pTextureElement->width = (dsvParams && dsvParams->width > 0) ? dsvParams->width : defaultDSWidth;
  pTextureElement->height = (dsvParams && dsvParams->height > 0) ? dsvParams->height : defaultDSHeight;
  pTextureElement->mipLevels = (dsvParams && dsvParams->mipLevels > 0) ? dsvParams->mipLevels : 1;
  pTextureElement->format = textureFormat;
  pTextureElement->bindFlags = bindFlags;
  pTextureElement->cpuAccessFlags = 0;
  pTextureElement->usage = GRESOURCE_USAGE::E::eDEFAULT;

  SPtr<GTexture> baseTexture = texture ? texture : createTexture2D(pTextureElement);
  if (!baseTexture) {
    WindowManager::instance().showErrorMessage("Error", "Error creating Depth Stencil texture", 0);
    delete dsvDesc;
    return nullptr;
  }

  pDepthStencilView->m_pTexture = std::static_pointer_cast<GDX11Texture>(baseTexture);

  if (FAILED(m_pd3d11Device->CreateDepthStencilView(pDepthStencilView->m_pTexture->m_texture, dsvDesc, &pDepthStencilView->m_pDSV))) {
    WindowManager::instance().showErrorMessage("Error", "Error creating Depth Stencil view", 0);
    delete dsvDesc;
    return nullptr;
  }

  delete dsvDesc;
  return std::static_pointer_cast<GDepthStencilView>(pDepthStencilView);
}

SPtr<GRenderTargetView>
GDX11Device::createRenderTargetView(SPtr<GRenderTargetViewElement> rtvParams,
                                    SPtr<GTexture> pTexture) {
  D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc = rtvParams ? new CD3D11_RENDER_TARGET_VIEW_DESC() : nullptr;
  
  bool requieresDepthStencil = false;
  if (rtvParams != nullptr) {
    if (rtvParams->flags & GRESOURCE_BINDINGS::E::DEPTHSTENCIL) {
      requieresDepthStencil = true;
      rtvParams->flags = (rtvParams->flags & ~GRESOURCE_BINDINGS::E::DEPTHSTENCIL);
    }
  }
  
  const uint32 resolvedRTVFormat = resolveColorFormat(rtvParams ? rtvParams->format : COLORFORMAT::E::RGBA_8_UNORM);
  if (rtvParams != nullptr) {
    rtvParams->format = resolvedRTVFormat;
  }

  if (rtvDesc != nullptr) {
    rtvDesc->Format = colorFormatToDXGI(resolvedRTVFormat);
    rtvDesc->Texture2D.MipSlice = 0;
    rtvDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  }

  SPtr<GDX11RenderTargetView> pRenderTargetView = std::make_shared<GDX11RenderTargetView>();
  
  SPtr<GTextureElement> pTextureElement = std::make_shared<GTextureElement>();
  pTextureElement->width = (rtvParams && rtvParams->width > 0) ? rtvParams->width : 128;
  pTextureElement->height = (rtvParams && rtvParams->height > 0) ? rtvParams->height : 128;
  pTextureElement->cpuAccessFlags = 0;
  pTextureElement->mipLevels = (rtvParams && rtvParams->mipLevels > 0) ? rtvParams->mipLevels : 1;
  pTextureElement->format = resolvedRTVFormat;
  pTextureElement->usage = GRESOURCE_USAGE::E::eDEFAULT;
  pTextureElement->bindFlags = (rtvParams && rtvParams->flags > 0) ? rtvParams->flags : GRESOURCE_BINDINGS::E::RENDERTARGET;

  SPtr<GTexture> renderTargetTexture = pTexture ? pTexture : createTexture2D(pTextureElement);
  if (!renderTargetTexture) {
    WindowManager::instance().showErrorMessage("Error", "Error creating Render Target texture", 0);
    delete rtvDesc;
    return nullptr;
  }

  pRenderTargetView->m_pTexture = std::static_pointer_cast<GDX11Texture>(renderTargetTexture);
  
  SPtr<GDepthStencilViewElement> dsvparams;
  if (requieresDepthStencil) {
    dsvparams = std::make_shared<GDepthStencilViewElement>();
    dsvparams->width = pTextureElement->width;
    dsvparams->height = pTextureElement->height;
    dsvparams->format = COLORFORMAT::E::D_24_UNORM_S8_UINT;
        dsvparams->viewDimension = GDSV_DIMENSION::E::TEXTURE2D;
    dsvparams->mipLevels = pTextureElement->mipLevels;
    dsvparams->flags = 0;
  }
  pRenderTargetView->m_pDSV = requieresDepthStencil
    ? std::static_pointer_cast<GDX11DepthStencilView>(createDepthStencilView(dsvparams))
    : nullptr;

  if (FAILED(m_pd3d11Device->CreateRenderTargetView(pRenderTargetView->m_pTexture->m_texture, rtvDesc, &pRenderTargetView->m_pRTV))) {
    WindowManager::instance().showErrorMessage("Error", "Error creating Render target view", 0);
    delete rtvDesc;
    return nullptr;
  }

  delete rtvDesc;
  return std::static_pointer_cast<GRenderTargetView>(pRenderTargetView);
}

SPtr<GTexture>
GDX11Device::createTexture2D(SPtr<GTextureElement> textureParams) {

  SPtr<GDX11Texture> pTexture = std::make_shared<GDX11Texture>();
  memset(&pTexture->m_texture, 0, sizeof(pTexture->m_texture));

  D3D11_TEXTURE2D_DESC* desc = textureParams != nullptr ? new CD3D11_TEXTURE2D_DESC : nullptr;

  if (desc != nullptr) {
    desc->ArraySize = 1;
        desc->BindFlags = resourceBindingsToD3D11BINDINGS(textureParams->bindFlags);
    desc->CPUAccessFlags = textureParams->cpuAccessFlags;
    const bool requestsDepth = (textureParams->bindFlags & GRESOURCE_BINDINGS::E::DEPTHSTENCIL) != 0;
    const uint32 resolvedTextureFormat = requestsDepth
      ? resolveDepthTextureFormat(textureParams->format, textureParams->bindFlags)
      : resolveColorFormat(textureParams->format);
    textureParams->format = resolvedTextureFormat;
    desc->Format = colorFormatToDXGI(resolvedTextureFormat);
    desc->Height = textureParams->height;
    desc->Width = textureParams->width;
    desc->MipLevels = textureParams->mipLevels;
    desc->MiscFlags = 0;
    desc->SampleDesc.Count = 1; //MSAA
    desc->SampleDesc.Quality = 0;
    desc->Usage = usageToD3D11USAGE(GRESOURCE_USAGE::E::_from_integral_unchecked(textureParams->usage));
  }
  if (FAILED(m_pd3d11Device->CreateTexture2D(desc, nullptr, &pTexture->m_texture))) {
    delete desc;
    return nullptr;
  }

  // TODO: Create shader resource view in case of flag on
  if (textureParams->bindFlags & GRESOURCE_BINDINGS::E::SHADERRESOURCE) {
    SPtr<GShaderResourceViewElement> pSRVParams = std::make_shared<GShaderResourceViewElement>();

    pSRVParams->format = textureParams->format;
    pSRVParams->mipLevels = textureParams->mipLevels == 1 ? 1 : -1;

    createShaderResourceView(pTexture, pSRVParams);
  }

  delete desc;
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Vertex Shader", 0);
    return nullptr;
  }

  // Reflect to populate parameter metadata
  sPtrShader->reflect();
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Pixel Shader", 0);
    return nullptr;
  }

  sPtrShader->reflect();
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Geometry Shader", 0);
    return nullptr;
  }

  sPtrShader->reflect();
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Compute Shader", 0);
    return nullptr;
  }

  sPtrShader->reflect();
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
  uint32 srvFormat = COLORFORMAT::E::_is_valid(srvParams->format) ? srvParams->format : COLORFORMAT::E::RGBA_8_UNORM;
  srvParams->format = srvFormat;
  if (srvFormat == COLORFORMAT::E::R_32_TYPELESS) {
    srvDesc.Format = colorFormatToDXGI(COLORFORMAT::E::R_32_FLOAT);
  }
  else {
    srvDesc.Format = colorFormatToDXGI(srvFormat);
  }
  srvDesc.Texture2D.MipLevels = srvParams->mipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(shaderResourceView);
  pTexture->m_pSRV = std::make_shared<GDX11ShaderResourceView>();


  HRESULT hr = m_pd3d11Device->CreateShaderResourceView(pTexture->m_texture, &srvDesc, &pTexture->m_pSRV->m_pd3d11SRV);
  
  if (FAILED(hr)) {
    // TODO: Show an error here
    WindowManager::instance().showErrorMessage("Error", "Error creating Shader resource view", 0);
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
    const uint32 inputFormat = resolveColorFormat(element.format);
    desc.Format = colorFormatToDXGI(inputFormat);
    desc.InputSlot = element.inputSlot;
    desc.AlignedByteOffset = element.alignedByteOffset;
    desc.InputSlotClass = inputClassificationToD3D11INPUT(INPUTCLASSIFICATION::E::_from_integral_unchecked(element.inputSlotClass));
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Input layout", 0);
    return nullptr;
  }
  return std::static_pointer_cast<GInputLayout>(sPtrInputLayout);
}


SPtr<GraphicsBuffer>
GDX11Device::createGraphicsBuffer(SPtr<GBufferElement> bufferParams) {
  
  D3D11_BUFFER_DESC desc;
  memset(&desc, 0, sizeof(desc));

  desc.Usage = usageToD3D11USAGE(GRESOURCE_USAGE::E::_from_integral_unchecked(bufferParams->usage));
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
    WindowManager::instance().showErrorMessage("Error", "Error creating graphics buffer", 0);
    return nullptr;
  }

  return std::static_pointer_cast<GraphicsBuffer>(pBuffer);
}
SPtr<GBlendState>
GDX11Device::createBlendState(SPtr<GBlendElement> blendParams) {
  D3D11_BLEND_DESC1 blendState;

  blendState.AlphaToCoverageEnable = false;
  blendState.IndependentBlendEnable = false;

  blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

  blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

  blendState.RenderTarget[0].LogicOpEnable = false;
  blendState.RenderTarget[0].LogicOp = D3D11_LOGIC_OP_NOOP;

  if (blendParams != nullptr) {
    blendState.RenderTarget[0].BlendEnable = blendParams->enabled;
    blendState.RenderTarget[0].RenderTargetWriteMask = blendParams->writeMask;
  }
  else {
    blendState.RenderTarget[0].BlendEnable = false;
    blendState.RenderTarget[0].RenderTargetWriteMask = BLEND_MASK::E::ALL;
  }

  SPtr<GDX11BlendState> pBlendState = std::make_shared<GDX11BlendState>();

  if (FAILED(m_pd3d11Device->CreateBlendState1(&blendState, &pBlendState->m_pBlendState))) {
    WindowManager::instance().showErrorMessage("Error", "Error creating Blend State", 0);
    return nullptr;
  }

  return std::static_pointer_cast<GBlendState>(pBlendState);
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Sampler State", 0);
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
    WindowManager::instance().showErrorMessage("Error", "Error creating Rasterizer State", 0);
    return nullptr;
  }
  return std::static_pointer_cast<GRasterizerState>(pRasterizerState);
}

}






