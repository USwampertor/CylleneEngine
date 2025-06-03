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
  
  GraphicsAPI::instance().createVertexShader(vsShadowR, "vertex_main");
  // m_shadowCB = GraphicsAPI::instance().createConstantBuffer();
  // if (!m_shadowCB) {
  //   WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
  //   return;
  // }

  // SPtr<GTextureElement> shadowDepthStencilTextureDesc = std::make_shared<GTextureElement>();
  // shadowDepthStencilTextureDesc->width = 1024;
  // shadowDepthStencilTextureDesc->height = 1024;
  // shadowDepthStencilTextureDesc->mipLevels = 1;
  // shadowDepthStencilTextureDesc->arraySize = 1;
  // shadowDepthStencilTextureDesc->format = DXGI_FORMAT_R32_TYPELESS;
  // shadowDepthStencilTextureDesc->sampleCount = 1;
  // shadowDepthStencilTextureDesc->sampleQuality = 0;
  // shadowDepthStencilTextureDesc->usage = D3D11_USAGE_DEFAULT;
  // shadowDepthStencilTextureDesc->bindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  // shadowDepthStencilTextureDesc->cpuAccessFlags = 0;
  // shadowDepthStencilTextureDesc->miscFlags = 0;

}

void
GShadowPass::execute() {

}

void
GShadowPass::cleanup() {

}

void
GShadowPass::shutdown() {

}


}