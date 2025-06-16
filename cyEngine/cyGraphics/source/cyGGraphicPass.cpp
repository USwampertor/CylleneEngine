#include "cyGGraphicPass.h"

#include <cyResourceManager.h>
#include <cyRShader.h>
#include <cyGraphicsAPI.h>
#include <cyLogger.h>
#include <cyWindow.h>

namespace CYLLENE_SDK {

GShadowPass::~GShadowPass() {

}

void
GShadowPass::initialize() {
  SPtr<RShader> vsShadowR = ResourceManager::instance().get<RShader>("shadowVertex");
  if (!vsShadowR) {
    WindowManager::ShowErrorMessage("Error", "There is no default shadow vertex shader loaded");
    return;
  }
  Vector<char> constantBufferData;
  GraphicsAPI::instance().createVertexShader(vsShadowR, "vertex_main");
  m_shadowCB = GraphicsAPI::instance().createConstantBuffer(constantBufferData);
  if (!m_shadowCB) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return;
  }

  SPtr<GTextureElement> shadowDepthStencilTextureDesc = std::make_shared<GTextureElement>();
  shadowDepthStencilTextureDesc->width = 1024;
  shadowDepthStencilTextureDesc->height = 1024;
  shadowDepthStencilTextureDesc->mipLevels = 1;
  shadowDepthStencilTextureDesc->arraySize = 1;
  shadowDepthStencilTextureDesc->format = COLORFORMAT::E::R_32_TYPELESS;  // DXGI_FORMAT_R32_TYPELESS;
  shadowDepthStencilTextureDesc->sampleCount = 1;
  shadowDepthStencilTextureDesc->sampleQuality = 0;
  shadowDepthStencilTextureDesc->usage = GRESOURCE_USAGE::E::eDEFAULT; // D3D11_USAGE_DEFAULT;
  shadowDepthStencilTextureDesc->bindFlags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE; // D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  shadowDepthStencilTextureDesc->cpuAccessFlags = 0;
  shadowDepthStencilTextureDesc->miscFlags = 0;
  m_shadowDST = GraphicsAPI::instance().createTexture2D(shadowDepthStencilTextureDesc);

  SPtr<GDepthStencilViewElement> shadowDepthStencilDesc = std::make_shared<GDepthStencilViewElement>();
  shadowDepthStencilDesc->format = COLORFORMAT::E::D_32_FLOAT; // DXGI_FORMAT_D32_FLOAT;
  shadowDepthStencilDesc->width = 1024;
  shadowDepthStencilDesc->height = 1024;
  shadowDepthStencilDesc->flags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE; // D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  shadowDepthStencilDesc->viewDimension = GDSV_DIMENSION::E::TEXTURE2D; // D3D11_DSV_DIMENSION_TEXTURE2D;
  SPtr<GDepthStencilView> m_shadowDSV = GraphicsAPI::instance().getDevice()->createDepthStencilView(shadowDepthStencilDesc, m_shadowDST);

}

void
GShadowPass::clear() {
  GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(m_shadowDSV,
    GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL /*D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, 1.0f, 0);
}

void
GShadowPass::execute() {
//   GraphicsAPI::instance().setViewport(0, 0, 1024, 1024);
//   
//   GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVShadowShader);
//   GraphicsAPI::instance().getDeviceContext()->setPixelShader(nullptr);
//   
//   GraphicsAPI::instance().getDeviceContext()->setInputLayout();
//   GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);
//   
//   Vector<SPtr<GRenderTargetView>> nullRTs;
//   nullRTs.push_back(nullptr);
//   GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1, nullRTs, m_shadowDSV);
//   
//   perPassConstants.cameraForward = shadowCamera->m_view.getForwardVector();
//   perPassConstants.cameraRight = shadowCamera->m_view.getRightVector();
//   perPassConstants.cameraUp = shadowCamera->m_view.getUpVector();
//   perPassConstants.view = shadowCamera->m_view;
//   perPassConstants.projection = shadowCamera->m_projection;
//   
//   constantBufferData.clear();
//   constantBufferData.resize(sizeof(perPassConstants));
//   memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));
//   
//   GraphicsDX11API::instance().writeToBuffer(perPassCB, constantBufferData);
//   Vector<SPtr<GraphicsBuffer>> gbVector;
//   gbVector.push_back(perPassCB);
//   GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
//   GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);
//   
//   draw(floorObject, floorMesh, nullptr);
//   draw(cubeObject, cubeMesh, nullptr);
}

void
GShadowPass::cleanup() {

}

void
GShadowPass::shutdown() {

}


}