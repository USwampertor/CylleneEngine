#include "cyGDX11DeviceContext.h"

#include "cyGDX11BlendState.h"
#include "cyGDX11DepthStencilView.h"
#include "cyGDX11RenderTargetView.h"
#include "cyGDX11Shader.h"
#include "cyGDX11InputLayout.h"
#include "cyDX11GraphicsBuffer.h"
#include "cyGDX11ShaderResourceView.h"
#include "cyGDX11SamplerState.h"
#include "cyGDX11Mesh.h"
#include "cyGDX11RasterizerState.h"
#include "cyDirectXHelpers.h"

namespace CYLLENE_SDK
{

GDX11DeviceContext::~GDX11DeviceContext() {
  DX11_SAFE_RELEASE(m_pDeviceContext);
}

void* GDX11DeviceContext::get() {
  return m_pDeviceContext;
}

void GDX11DeviceContext::set(void* pHandle) {
  m_pDeviceContext = static_cast<ID3D11DeviceContext1*>(pHandle);
}

void
GDX11DeviceContext::queryInterface() {

}

void
GDX11DeviceContext::setViewPort(uint32 numViewports, const GViewport& viewport) {
  D3D11_VIEWPORT vp;
  vp.TopLeftX = viewport.m_rect.x;
  vp.TopLeftY = viewport.m_rect.y;
  vp.Width = viewport.m_rect.width;
  vp.Height = viewport.m_rect.height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  m_pDeviceContext->RSSetViewports(numViewports, &vp);
}

void
GDX11DeviceContext::updateSubresource(SPtr<GGraphic> resource, const GSubResourceElement& data) {
  ID3D11Resource* pResource = static_cast<ID3D11Resource*>(resource->get());
  m_pDeviceContext->UpdateSubresource1(pResource, 
                                       data.index, 
                                       nullptr, 
                                       data.data, 
                                       data.pitch, 
                                       data.depth, 
                                       0);
  // m_pDeviceContext->UpdateSubresource1(pResource, index, nullptr, &data, 0, 0);
}

void
GDX11DeviceContext::setRenderTargets(uint32 numRenderTargets,
                                     Vector<SPtr<GRenderTargetView>> renderTargets,
                                     SPtr<GDepthStencilView> depthStencil) {

  ID3D11DepthStencilView* pDSV = nullptr;
  if (depthStencil != nullptr) {
    SPtr<GDX11DepthStencilView> pDepthStencilView = std::static_pointer_cast<GDX11DepthStencilView>(depthStencil);
    pDSV = pDepthStencilView->m_pDSV;
  }
  Vector<ID3D11RenderTargetView*> pRTVs;
  
  for (uint32 i = 0; i < numRenderTargets; ++i) {
    if (renderTargets[i] != nullptr) {
      SPtr<GDX11RenderTargetView> pRenderTargetView = std::static_pointer_cast<GDX11RenderTargetView>(renderTargets[i]);
      pRTVs.push_back(pRenderTargetView->m_pRTV);
    }
    else {
      pRTVs.push_back(nullptr);
    }
  }

  m_pDeviceContext->OMSetRenderTargets(numRenderTargets, pRTVs.data(), pDSV);


}

void
GDX11DeviceContext::clearRenderTargetView(SPtr<GRenderTargetView> renderTarget,
                                          const Color& color) {
  SPtr<GDX11RenderTargetView> pRenderTargetView = 
    std::static_pointer_cast<GDX11RenderTargetView>(renderTarget);
  
  float toDXColor[] = { color.r, color.g, color.b, color.a };
  
  m_pDeviceContext->ClearRenderTargetView(pRenderTargetView->m_pRTV, toDXColor);
}

void
GDX11DeviceContext::clearDepthStencilView(SPtr<GDepthStencilView> depthStencil,
                                          uint32 clearFlags,
                                          float depth, 
                                          uint8 stencil) {
  SPtr<GDX11DepthStencilView> pDepthStencilView = 
    std::static_pointer_cast<GDX11DepthStencilView>(depthStencil);
  
  m_pDeviceContext->ClearDepthStencilView(pDepthStencilView->m_pDSV, clearFlags, depth, stencil);
}

void
GDX11DeviceContext::setVertexShader(SPtr<GVertexShader> shader, 
                                    void* classInstance /* = nullptr */, 
                                    uint32 numClasses /* = 0 */) {

  if (shader == nullptr) {
    m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
    return;
  }

  SPtr<GDX11VertexShader> pVertexShader = std::static_pointer_cast<GDX11VertexShader>(shader);
  m_pDeviceContext->VSSetShader(pVertexShader->m_pVertexShader, 
                                static_cast<ID3D11ClassInstance* const*>(classInstance), 
                                numClasses);
}

void
GDX11DeviceContext::setPixelShader(SPtr<GPixelShader> shader,
                                   void* classInstance /* = nullptr */,
                                   uint32 numClasses /* = 0 */) {
  if (shader == nullptr) {
    m_pDeviceContext->PSSetShader(nullptr, nullptr, 0); 
    return;
  }

  SPtr<GDX11PixelShader> pPixelShader = std::static_pointer_cast<GDX11PixelShader>(shader);
  m_pDeviceContext->PSSetShader(pPixelShader->m_pPixelShader, 
                                static_cast<ID3D11ClassInstance* const*>(classInstance), 
                                numClasses);
}

void
GDX11DeviceContext::setGeometryShader(SPtr<GGeometryShader> shader,
                                      void* classInstance /* = nullptr */,
                                      uint32 numClasses /* = 0 */) {
  if (shader == nullptr) {
    m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
    return;
  }

  SPtr<GDX11GeometryShader> pGeometryShader = std::static_pointer_cast<GDX11GeometryShader>(shader);
  m_pDeviceContext->GSGetShader(&pGeometryShader->m_pGeometryShader,
                                static_cast<ID3D11ClassInstance**>(classInstance), 
                                &numClasses);
}

void
GDX11DeviceContext::setComputeShader(SPtr<GComputeShader> shader,
                                      void* classInstance /* = nullptr */,
                                      uint32 numClasses /* = 0 */) {
  if (shader == nullptr) {
    m_pDeviceContext->CSSetShader(nullptr, nullptr, 0);
    return;
  }

  SPtr<GDX11ComputeShader> pComputeShader = std::static_pointer_cast<GDX11ComputeShader>(shader);
  m_pDeviceContext->CSSetShader(pComputeShader->m_pComputeShader,
                                static_cast<ID3D11ClassInstance* const*>(classInstance),
                                numClasses);
}


void
GDX11DeviceContext::setShader(SPtr<GShader> shader,
                              void* classInstance /* = nullptr */,
                              uint32 numClasses /* = 0 */) {
  if (shader == nullptr) {
    m_pDeviceContext->VSSetShader(nullptr, nullptr, 0);
    return;
  }

  // TODO: Implement this
//   SPtr<GDX11ComputeShader> pComputeShader = std::static_pointer_cast<GDX11ComputeShader>(shader);
//   m_pDeviceContext->CSSetShader(pComputeShader->m_pComputeShader,
//                                 static_cast<ID3D11ClassInstance* const*>(classInstance),
//                                 numClasses);
}



void
GDX11DeviceContext::setInputLayout(SPtr<GInputLayout> layout) {
  SPtr<GDX11InputLayout> pInputLayout = std::static_pointer_cast<GDX11InputLayout>(layout);
  m_pDeviceContext->IASetInputLayout(pInputLayout->m_pInputLayout);
}

void
GDX11DeviceContext::setPrimitiveTopology(uint32 topology) {
  m_pDeviceContext->IASetPrimitiveTopology(primitiveTopologyToD3D3TOPOLOGY(GPRIMITIVE_TOPOLOGY::E::_from_integral_unchecked(topology)));
}

void
GDX11DeviceContext::setVertexBuffers(uint32 startSlot, 
                                     uint32 numBuffers, 
                                     Vector<SPtr<GraphicsBuffer>> buffers, 
                                     Vector<uint32> strides, 
                                     Vector<uint32> offsets) {
  Vector<ID3D11Buffer*> pBuffers;
  for (uint32 i = 0; i < numBuffers; ++i) {
    SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffers[i]);
    pBuffers.push_back(pBuffer->m_pBuffer);
  }

  m_pDeviceContext->IASetVertexBuffers(startSlot, 
                                       numBuffers, 
                                       pBuffers.data(), 
                                       strides.data(), 
                                       offsets.data());
}


void
GDX11DeviceContext::setIndexBuffer(SPtr<GraphicsBuffer> buffer,
                                   uint32 format,
                                   uint32 offset) {
  SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffer);
  m_pDeviceContext->IASetIndexBuffer(pBuffer->m_pBuffer, colorFormatToDXGI(format), offset);
}

void
GDX11DeviceContext::setVSConstantBuffer(uint32 slot,
                                        uint32 numBuffers,
                                        Vector<SPtr<GraphicsBuffer>> buffers) {
  Vector<ID3D11Buffer*> pBuffers;
  for (uint32 i = 0; i < numBuffers; ++i) {
    SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffers[i]);
    pBuffers.push_back(pBuffer->m_pBuffer);
  }
  m_pDeviceContext->VSSetConstantBuffers(slot, numBuffers, pBuffers.data());
}

void
GDX11DeviceContext::setPSConstantBuffer(uint32 slot,
                                        uint32 numBuffers,
                                        Vector<SPtr<GraphicsBuffer>> buffers) {
  Vector<ID3D11Buffer*> pBuffers;
  for (uint32 i = 0; i < numBuffers; ++i) {
    SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffers[i]);
    pBuffers.push_back(pBuffer->m_pBuffer);
  }
  m_pDeviceContext->PSSetConstantBuffers(slot, numBuffers, pBuffers.data());
}

void
GDX11DeviceContext::setCSConstantBuffer(uint32 slot,
                                        uint32 numBuffers,
                                        Vector<SPtr<GraphicsBuffer>> buffers) {
  Vector<ID3D11Buffer*> pBuffers;
  for (uint32 i = 0; i < numBuffers; ++i) {
    SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffers[i]);
    pBuffers.push_back(pBuffer->m_pBuffer);
  }
  m_pDeviceContext->CSSetConstantBuffers(slot, numBuffers, pBuffers.data());
}


void
GDX11DeviceContext::setShaderResources(Vector<SPtr<GShaderResourceView>> resource,
                                      uint32 slot,
                                      uint32 numViews) {
  Vector<ID3D11ShaderResourceView*> pBuffers;
  for (uint32 i = 0; i < numViews; ++i) {
    SPtr<GDX11ShaderResourceView> pBuffer = std::static_pointer_cast<GDX11ShaderResourceView>(resource[i]);
    pBuffers.push_back(pBuffer->m_pd3d11SRV);
  }
  m_pDeviceContext->PSSetShaderResources(slot, numViews, pBuffers.data());
//   SPtr<GDX11ShaderResourceView> pShaderResourceView = 
//     std::static_pointer_cast<GDX11ShaderResourceView>(resource);
//   m_pDeviceContext->PSSetShaderResources(slot, numViews, &pShaderResourceView->m_pSRV);
}

void 
GDX11DeviceContext::unbindShaderResource(uint32 slot) {
  Vector<ID3D11ShaderResourceView*> nullBuffer;
  nullBuffer.push_back(nullptr);
  m_pDeviceContext->PSSetShaderResources(slot, 1, nullBuffer.data());
}

void
GDX11DeviceContext::setBlendState(SPtr<GBlendState> blendState) {
  if (blendState == nullptr) {
    m_pDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    return;
  }

  SPtr<GDX11BlendState> pBlendState = std::static_pointer_cast<GDX11BlendState>(blendState);
  m_pDeviceContext->OMSetBlendState(pBlendState->m_pBlendState, nullptr, 0xffffffff);
}


void
GDX11DeviceContext::setSamplers(uint32 slot,
                                uint32 numSamplers,
                                Vector<SPtr<GSamplerState>> samplers) {
                                Vector<ID3D11SamplerState*> pSamplers;
  for (uint32 i = 0; i < numSamplers; ++i) {
    SPtr<GDX11SamplerState> pSamplerState = std::static_pointer_cast<GDX11SamplerState>(samplers[i]);
    pSamplers.push_back(pSamplerState->m_pSamplerState);
  }
  m_pDeviceContext->PSSetSamplers(slot, numSamplers, pSamplers.data());
}

void
GDX11DeviceContext::drawIndexed(SPtr<GMesh> mesh) {
  SPtr<GDX11Mesh> pMesh = std::static_pointer_cast<GDX11Mesh>(mesh);

  m_pDeviceContext->DrawIndexed(pMesh->m_numIndices, pMesh->m_baseIndex, pMesh->m_baseVertex);

  // m_pDeviceContext->DrawIndexed(pMesh->m_indexCount,
  //                               pMesh->m_startIndexLocation,
  //                               pMesh->m_baseVertexLocation);
}

void
GDX11DeviceContext::drawIndexedInstanced(SPtr<GMesh> mesh, uint32 instances) {
  SPtr<GDX11Mesh> pMesh = std::static_pointer_cast<GDX11Mesh>(mesh);
  m_pDeviceContext->DrawIndexedInstanced(pMesh->m_numIndices, instances, pMesh->m_baseIndex, pMesh->m_baseVertex, 0);
}

void
GDX11DeviceContext::drawIndexedInstancedIndirect(Vector<SPtr<GraphicsBuffer>> buffer, uint32 instances) {

  Vector<ID3D11Buffer*> pBuffers;
  for (uint32 i = 0; i < buffer.size(); ++i) {
    SPtr<DX11GraphicsBuffer> pBuffer = std::static_pointer_cast<DX11GraphicsBuffer>(buffer[i]);
    pBuffers.push_back(pBuffer->m_pBuffer);
  }
  m_pDeviceContext->DrawIndexedInstancedIndirect(pBuffers[0], 0);
}


void
GDX11DeviceContext::dispatch(Vector3f threadGroupCount) {
  m_pDeviceContext->Dispatch(static_cast<uint32>(threadGroupCount.x),
                             static_cast<uint32>(threadGroupCount.y),
                             static_cast<uint32>(threadGroupCount.z));
}

void
GDX11DeviceContext::drawInstanced(SPtr<GMesh> mesh, uint32 instances) {
  SPtr<GDX11Mesh> pMesh = std::static_pointer_cast<GDX11Mesh>(mesh);
  m_pDeviceContext->DrawInstanced(pMesh->m_numVertices, instances, pMesh->m_baseVertex, 0);
}

// 
// void
// GDX11DeviceContext::setShaderResources(Vector<SPtr<GShaderResourceView>> resource,
//                                        uint32 slot,
//                                        uint32 numViews) {
//   Vector<ID3D11ShaderResourceView*> pViews;
//   for (uint32 i = 0; i < numViews; ++i) {
//     SPtr<GDX11ShaderResourceView> pShaderResourceView =
//       std::static_pointer_cast<GDX11ShaderResourceView>(pViews[i]);
//     pViews.push_back(pShaderResourceView->m_pSRV);
//   }
//   m_pDeviceContext->PSSetShaderResources(slot, numViews, pViews.data());
// }


}


