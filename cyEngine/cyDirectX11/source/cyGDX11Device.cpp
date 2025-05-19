#include "cyGDX11Device.h"
#include "cyGDX11Texture.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"
#include "cyGDX11InputLayout.h"
#include "cyGDX11Shader.h"
#include "cyDX11GraphicsBuffer.h"
#include "cyGDX11RasterizerState.h"
#include "cyGDX11ShaderResourceView.h"

namespace CYLLENE_SDK {

GDX11Device::~GDX11Device() {
  DX11_SAFE_RELEASE(m_pDevice);
}
void* GDX11Device::get() {
  return m_pDevice;
}

void GDX11Device::set(void* pHandle) {
  m_pDevice = static_cast<ID3D11Device1*>(pHandle);
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
GDX11Device::createDepthStencilView(SPtr<GTexture> depthStencilView,
                                    SPtr<GDepthStencilViewElement> dsvParams) {

  D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc = dsvParams != nullptr ? new CD3D11_DEPTH_STENCIL_VIEW_DESC() : nullptr;

  if (dsvDesc != nullptr) {
    // D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC();
    dsvDesc->Flags = 0;
    dsvDesc->Format; // = format;
    dsvDesc->Texture2D.MipSlice = 0;
    dsvDesc->ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  }
  
  SPtr<GDX11DepthStencilView> pDepthStencilView = std::make_shared<GDX11DepthStencilView>();
  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(depthStencilView);

  if (FAILED(m_pDevice->CreateDepthStencilView(pTexture->m_texture, dsvDesc, &pDepthStencilView->m_pDSV))) {
    return nullptr;
  }
  return std::static_pointer_cast<GDepthStencilView>(pDepthStencilView);
}

SPtr<GRenderTargetView>
GDX11Device::createRenderTargetView(SPtr<GTexture> renderTargetView, 
                                    SPtr<GRenderTargetViewElement> rtvParams) {
  
  D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc = rtvParams ? new CD3D11_RENDER_TARGET_VIEW_DESC() : nullptr;
  if (rtvDesc != nullptr) {
    // D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = CD3D11_RENDER_TARGET_VIEW_DESC();
    rtvDesc->Format; // = format;
    rtvDesc->Texture2D.MipSlice = 0;
    rtvDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  }
  SPtr<GDX11RenderTargetView> pRenderTargetView = std::make_shared<GDX11RenderTargetView>();
  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(renderTargetView);

  if (FAILED(m_pDevice->CreateRenderTargetView(pTexture->m_texture, rtvDesc, &pRenderTargetView->m_pRTV))) {
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
  if (FAILED(m_pDevice->CreateTexture2D(desc, nullptr, &pTexture->m_texture))) {
    return nullptr;
  }
  return std::static_pointer_cast<GTexture>(pTexture);
}

SPtr<GVertexShader>
GDX11Device::createVertexShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);
  
  SPtr<GDX11VertexShader> sPtrShader = std::make_shared<GDX11VertexShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;
  
  HRESULT hr = m_pDevice->CreateVertexShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                             sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                             nullptr,
                                             &sPtrShader->m_pVertexShader);


  if (FAILED(hr)) {
    MessageBox(nullptr, "Error creating Vertex Shader", "Error", MB_OK);
    return nullptr;
  }

  return std::static_pointer_cast<GVertexShader>(sPtrShader);
}

SPtr<GPixelShader>
GDX11Device::createPixelShader(SPtr<GShaderBlob> blob) {
  SPtr<GDX11ShaderBlob> sPtrShaderBlob = std::static_pointer_cast<GDX11ShaderBlob>(blob);

  SPtr<GDX11PixelShader> sPtrShader = std::make_shared<GDX11PixelShader>();
  sPtrShader->m_pBlob = sPtrShaderBlob;

  HRESULT hr = m_pDevice->CreatePixelShader(sPtrShaderBlob->m_pBlob->GetBufferPointer(),
                                            sPtrShaderBlob->m_pBlob->GetBufferSize(),
                                            nullptr,
                                            &sPtrShader->m_pPixelShader);


  if (FAILED(hr)) {
    MessageBox(nullptr, "Error creating Vertex Shader", "Error", MB_OK);
    return nullptr;
  }

  return std::static_pointer_cast<GPixelShader>(sPtrShader);
}

void// SPtr<GShaderResourceView>
GDX11Device::createShaderResourceView(SPtr<GTexture> shaderResourceView,
                                      SPtr<GShaderResourceViewElement> srvParams) {
  
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();
  srvDesc.Format = static_cast<DXGI_FORMAT>(srvParams->format);
  srvDesc.Texture2D.MipLevels = srvParams->mipLevels;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

  SPtr<GDX11Texture> pTexture = std::static_pointer_cast<GDX11Texture>(shaderResourceView);
  SPtr<GDX11ShaderResourceView> pShaderResourceView = std::make_shared<GDX11ShaderResourceView>();

  HRESULT hr = m_pDevice->CreateShaderResourceView(pTexture->m_texture, &srvDesc, &pShaderResourceView->m_pSRV);
  
  if (!FAILED(hr)) {
    pTexture->m_pSRV = pShaderResourceView->m_pSRV;
  }
  else {
    pTexture->m_pSRV = pShaderResourceView->m_pSRV;
    // TODO: Show here a warning or error;
  }
  // return std::static_pointer_cast<GShaderResourceView>(pShaderResourceView);
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

  HRESULT hr = m_pDevice->CreateInputLayout(d3d11Descriptor.data(),
                                            d3d11Descriptor.size(),
                                /*pShader*/ sPtrVertexShader->m_pBlob->m_pBlob->GetBufferPointer(),
                                /*pShader*/ sPtrVertexShader->m_pBlob->m_pBlob->GetBufferSize(),
                                            &sPtrInputLayout->m_pInputLayout);
  
  
  if (FAILED(hr)) {
    MessageBox(nullptr, "Error creating Input layout", "Error", MB_OK);
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

  HRESULT hr = m_pDevice->CreateBuffer(&desc, &initData, &pBuffer->m_pBuffer);


  if (FAILED(hr)) {
    MessageBox(nullptr, "Error creating graphics buffer", "Error", MB_OK);
    return nullptr;
  }

  return std::static_pointer_cast<GraphicsBuffer>(pBuffer);
}

SPtr<GRasterizerState>
GDX11Device::createRasterizerState(SPtr<GRasterizerElement> rasterizerParams) {
  CD3D11_RASTERIZER_DESC1 descRD(D3D11_DEFAULT);

  SPtr<GDX11RasterizerState> pRasterizerState = std::make_shared<GDX11RasterizerState>();

  m_pDevice->CreateRasterizerState1(&descRD, &pRasterizerState->m_pRasterizerState);
  if (FAILED(m_pDevice->CreateRasterizerState1(&descRD, &pRasterizerState->m_pRasterizerState))) {
    MessageBox(nullptr, "Error creating Rasterizer State", "Error", MB_OK);
    return nullptr;
  }
  return std::static_pointer_cast<GRasterizerState>(pRasterizerState);
}

}

