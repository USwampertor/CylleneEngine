#include "cyGGraphicPass.h"

#include <cyResourceManager.h>
#include <cyRShader.h>
#include <cyGraphicsAPI.h>
#include <cyLogger.h>
#include <cyWindow.h>
#include <cyCCamera.h>

namespace CYLLENE_SDK {

GShadowPass::~GShadowPass() {
  shutdown();
}

void
GShadowPass::initialize() {
  // First check if the shader is already loaded
  if (!m_pVShadowShader) {
    SPtr<RShader> vsShadowR = ResourceManager::instance().get<RShader>("shadowVertex");
    if (vsShadowR) {
      m_pVShadowShader = GraphicsAPI::instance().createVertexShader(vsShadowR, "vertex_main");
      if (!m_pVShadowShader) {
        WindowManager::showErrorMessage("Error", 
                                        "There is no default shadow vertex shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pVShadowShader = GraphicsAPI::instance().createVertexShader("shadowVertex.hlsl");
      if (!m_pVShadowShader) {
        WindowManager::showErrorMessage("Error", 
                                        "There is no default shadow vertex shader loaded", 0);
        return;
      }
    }
  }
  
  m_bufferData.clear();
  m_bufferData.resize(sizeof(DefaultShadowConstantBuffer));
  memset(m_bufferData.data(), 0, sizeof(DefaultShadowConstantBuffer));
  m_shadowCB = GraphicsAPI::instance().createConstantBuffer(m_bufferData);
  if (!m_shadowCB) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return;
  }

  SPtr<GTextureElement> shadowDepthStencilTextureDesc = makeSharedPtr<GTextureElement>();
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

  SPtr<GDepthStencilViewElement> shadowDepthStencilDesc = makeSharedPtr<GDepthStencilViewElement>();
  shadowDepthStencilDesc->format = COLORFORMAT::E::D_32_FLOAT; // DXGI_FORMAT_D32_FLOAT;
  shadowDepthStencilDesc->width = 1024;
  shadowDepthStencilDesc->height = 1024;
  shadowDepthStencilDesc->flags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE; // D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  shadowDepthStencilDesc->viewDimension = GDSV_DIMENSION::E::TEXTURE2D; // D3D11_DSV_DIMENSION_TEXTURE2D;
  m_shadowDSV = GraphicsAPI::instance().getDevice()->createDepthStencilView(shadowDepthStencilDesc, m_shadowDST);


  SPtr<GRasterizerElement> defaultRasDesc = makeSharedPtr<GRasterizerElement>();
  SPtr<GRasterizerState> m_rasterState = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);

}

void
GShadowPass::clear() {
  GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(m_shadowDSV,
    GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL /*D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, 1.0f, 0);
}

void
GShadowPass::execute() {
  GraphicsAPI::instance().setViewport(0, 0, 1024, 1024);
  GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVShadowShader);
  GraphicsAPI::instance().getDeviceContext()->setPixelShader(nullptr);
  GraphicsAPI::instance().getDeviceContext()->setInputLayout(GraphicsAPI::instance().m_defaultLayout);
  GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);
  Vector<SPtr<GRenderTargetView>> nullRTs;
  nullRTs.push_back(nullptr);
  GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1, nullRTs, m_shadowDSV);

  if (m_camera.expired()) {
    WindowManager::showErrorMessage("Error", "No camera assigned to shadow pass", 0);
    return;
  }

  SPtr<CCamera> shadowCamera = m_camera.lock();
  m_perPassConstants.cameraForward = shadowCamera->m_view.getForwardVector();
  m_perPassConstants.cameraRight = shadowCamera->m_view.getRightVector();
  m_perPassConstants.cameraUp = shadowCamera->m_view.getUpVector();
  m_perPassConstants.view = shadowCamera->m_view;
  m_perPassConstants.projection = shadowCamera->m_projection;
  
  m_bufferData.clear();
  m_bufferData.resize(sizeof(m_perPassConstants));
  memcpy(m_bufferData.data(), &m_perPassConstants, sizeof(m_perPassConstants));
  
  GraphicsAPI::instance().writeToBuffer(m_perPassCB, m_bufferData);
  Vector<SPtr<GraphicsBuffer>> gbVector;
  gbVector.push_back(m_perPassCB);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);
  
  // for (auto& obj : shadowCamera->getObjectsByDistance(1000)) {
  //   GraphicsAPI::instance().draw(obj);
  // }
}

void
GShadowPass::cleanup() {

}

void
GShadowPass::shutdown() {

}

GGeometryPass::~GGeometryPass() {
  shutdown();
}

void
GGeometryPass::initialize() {

  if (m_camera.expired()) {
    WindowManager::showErrorMessage("Error", "No camera assigned to shadow pass", 0);
    return;
  }

  SPtr<CCamera> camera = m_camera.lock();

  if (!m_pVGeometryShader) {
    SPtr<RShader> vsGeometryR = ResourceManager::instance().get<RShader>("vertexShader");
    if (vsGeometryR) {
      m_pVGeometryShader = GraphicsAPI::instance().createVertexShader(vsGeometryR, "vertex_main");
      if (!m_pVGeometryShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default vertex shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pVGeometryShader = GraphicsAPI::instance().createVertexShader("vertexShader.hlsl");
      if (!m_pVGeometryShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default vertex shader loaded", 0);
        return;
      }
    }
  }

  if (!m_pPGeometryShader) {
    SPtr<RShader> psGeometryR = ResourceManager::instance().get<RShader>("pixelShader");
    if (psGeometryR) {
      m_pPGeometryShader = GraphicsAPI::instance().createPixelShader(psGeometryR, "pixel_main");
      if (!m_pPGeometryShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default pixel shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pPGeometryShader = GraphicsAPI::instance().createPixelShader("pixelShader.hlsl");
      if (!m_pPGeometryShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default pixel shader loaded", 0);
        return;
      }
    }
  }

  Vector<char> shaderConstantsData;
  shaderConstantsData.resize(sizeof(DefaultShaderConstants));
  memset(shaderConstantsData.data(), 0, sizeof(DefaultShaderConstants));
  SPtr<GraphicsBuffer> shaderConstantsBuffer = GraphicsAPI::instance().createConstantBuffer(shaderConstantsData);
  if (!shaderConstantsBuffer) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return;
  }

  SPtr<GRenderTargetViewElement> colorRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  colorRenderTargetDesc->format = COLORFORMAT::E::RGBA_32_FLOAT;
  colorRenderTargetDesc->width = camera->m_width;
  colorRenderTargetDesc->height = camera->m_height;
  colorRenderTargetDesc->flags = GRESOURCE_BINDINGS::E::SHADERRESOURCE |
                                 GRESOURCE_BINDINGS::E::RENDERTARGET |
                                 GRESOURCE_BINDINGS::E::DEPTHSTENCIL |
                                 GRESOURCE_BINDINGS::E::UNORDEREDACCESS;
  m_colorRenderTarget = GraphicsAPI::instance().getDevice()->createRenderTargetView(colorRenderTargetDesc);

  SPtr<GRenderTargetViewElement> positionRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  positionRenderTargetDesc->format = COLORFORMAT::E::RGBA_32_FLOAT;
  positionRenderTargetDesc->width = camera->m_width;
  positionRenderTargetDesc->height = camera->m_height;
  positionRenderTargetDesc->flags = GRESOURCE_BINDINGS::E::SHADERRESOURCE |
                                    GRESOURCE_BINDINGS::E::RENDERTARGET |
                                    GRESOURCE_BINDINGS::E::UNORDEREDACCESS;
  m_positionRenderTarget = GraphicsAPI::instance().getDevice()->createRenderTargetView(positionRenderTargetDesc);

  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;

  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  m_pointSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  m_linearSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  SPtr<GRasterizerElement> defaultRasDesc = makeSharedPtr<GRasterizerElement>();
  SPtr<GRasterizerState> m_rasterState = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);

}

void
GGeometryPass::clear() {

}

void
GGeometryPass::execute() {

  if (m_camera.expired()) {
    WindowManager::showErrorMessage("Error", "No camera assigned to shadow pass", 0);
    return;
  }

  SPtr<CCamera> camera = m_camera.lock();

  GraphicsAPI::instance().setViewport(0, 0, camera->m_width , camera->m_height);

  GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVGeometryShader);
  GraphicsAPI::instance().getDeviceContext()->setPixelShader(m_pPGeometryShader);

  GraphicsAPI::instance().getDeviceContext()->setInputLayout(GraphicsAPI::instance().m_defaultLayout);
  GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

  Vector<SPtr<GRenderTargetView>> targets;
  targets.push_back(m_colorRenderTarget);
  targets.push_back(m_positionRenderTarget);
  GraphicsAPI::instance().getDeviceContext()->setRenderTargets(2,
                                                               targets,
                                                               m_colorRenderTarget.get()->getDepthStencil());

  Vector<SPtr<GraphicsBuffer>> scVector;
  scVector.push_back(m_shaderConstantsBuffer);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

  m_perPassConstants.cameraForward = camera->m_view.getForwardVector();
  m_perPassConstants.cameraRight = camera->m_view.getRightVector();
  m_perPassConstants.cameraUp = camera->m_view.getUpVector();
  m_perPassConstants.view = camera->m_view;
  m_perPassConstants.projection = camera->m_projection;

  m_bufferData.clear();
  m_bufferData.resize(sizeof(m_perPassConstants));
  memcpy(m_bufferData.data(), &m_perPassConstants, sizeof(m_perPassConstants));

  GraphicsAPI::instance().writeToBuffer(m_perPassCB, m_bufferData);
  Vector<SPtr<GraphicsBuffer>> gbVector;
  gbVector.push_back(m_perPassCB);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

  Vector<SPtr<GSamplerState>> ssVec1;
  Vector<SPtr<GSamplerState>> ssVec2;
  ssVec1.push_back(m_pointSampler);
  ssVec2.push_back(m_linearSampler);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);


  // for (auto& obj : shadowCamera->getObjectsByDistance(1000)) {
  //   GraphicsAPI::instance().draw(obj, material);
  // }
}

void
GGeometryPass::cleanup() {

}


void
GGeometryPass::shutdown() {

}

void
GParticlesPass::initialize() {
  if (!m_pVParticleShader) {
    SPtr<RShader> vsParticleR = ResourceManager::instance().get<RShader>("particleVertexShader");
    if (vsParticleR) {
      m_pVParticleShader = GraphicsAPI::instance().createVertexShader(vsParticleR, "vertex_main");
      if (!m_pVParticleShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default particle vertex shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pVParticleShader = GraphicsAPI::instance().createVertexShader("particleVertexShader.hlsl");
      if (!m_pVParticleShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default particle vertex shader loaded", 0);
        return;
      }
    }
  }

  if (!m_pPParticleShader) {
    SPtr<RShader> psParticleR = ResourceManager::instance().get<RShader>("particlePixelShader");
    if (psParticleR) {
      m_pPParticleShader = GraphicsAPI::instance().createPixelShader(psParticleR, "pixel_main");
      if (!m_pPParticleShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default particle pixel shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pPParticleShader = GraphicsAPI::instance().createPixelShader("particlePixelShader.hlsl");
      if (!m_pPParticleShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default particle pixel shader loaded", 0);
        return;
      }
    }
  }

  SPtr<GRasterizerElement> defaultRasDesc = makeSharedPtr<GRasterizerElement>();
  SPtr<GRasterizerState> m_rasterState = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);


  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;

  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  m_pointSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  m_linearSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

}


void
GParticlesPass::clear() {

}

void
GParticlesPass::execute() {

  if (m_camera.expired()) {
    WindowManager::showErrorMessage("Error", "No camera assigned to shadow pass", 0);
    return;
  }

  SPtr<CCamera> camera = m_camera.lock();

  GraphicsAPI::instance().setViewport(0, 0, camera->m_width, camera->m_height);

  GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVParticleShader);
  GraphicsAPI::instance().getDeviceContext()->setPixelShader(m_pPParticleShader);

  GraphicsAPI::instance().getDeviceContext()->setInputLayout(GraphicsAPI::instance().m_defaultLayout);
  GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

  Vector<SPtr<GRenderTargetView>> targets;
  targets.push_back(colorRenderTarget);
  GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
                                                               targets,
                                                               colorRenderTarget.get()->getDepthStencil());

  GraphicsAPI::instance().getDeviceContext()->setBlendState(m_alphaBlend);

  Vector<SPtr<GraphicsBuffer>> scVector;
  scVector.push_back(shaderConstantsBuffer);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

  m_perPassConstants.cameraForward = camera->m_view.getForwardVector();
  m_perPassConstants.cameraRight = camera->m_view.getRightVector();
  m_perPassConstants.cameraUp = camera->m_view.getUpVector();
  m_perPassConstants.view = camera->m_view;
  m_perPassConstants.projection = camera->m_projection;

  m_bufferData.clear();
  m_bufferData.resize(sizeof(m_perPassConstants));
  memcpy(m_bufferData.data(), &m_perPassConstants, sizeof(m_perPassConstants));

  GraphicsAPI::instance().writeToBuffer(m_perPassCB, m_bufferData);
  Vector<SPtr<GraphicsBuffer>> gbVector;
  gbVector.push_back(m_perPassCB);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

  Vector<SPtr<GSamplerState>> ssVec1;
  Vector<SPtr<GSamplerState>> ssVec2;
  ssVec1.push_back(m_pointSampler);
  ssVec2.push_back(m_linearSampler);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

  //draw(camera, saqObject, saqMesh, &particleGTexture);

  GraphicsAPI::instance().drawInstanced(, 100);
}

void
GParticlesPass::cleanup() {

}

void
GParticlesPass::shutdown() {
  
}


void
GPostProcessPass::initialize() {
  if (!m_pVSAQShader) {
    SPtr<RShader> vsSAQR = ResourceManager::instance().get<RShader>("saqVertexShader");
    if (vsSAQR) {
      m_pVSAQShader = GraphicsAPI::instance().createVertexShader(vsSAQR, "vertex_main");
      if (!m_pVSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ vertex shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pVSAQShader = GraphicsAPI::instance().createVertexShader("saqPixelShader.hlsl");
      if (!m_pVSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ vertex shader loaded", 0);
        return;
      }
    }
  }

  if (!m_pPSAQShader) {
    SPtr<RShader> psSAQR = ResourceManager::instance().get<RShader>("particlePixelShader");
    if (psSAQR) {
      m_pPSAQShader = GraphicsAPI::instance().createPixelShader(psSAQR, "pixel_main");
      if (!m_pPSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ pixel shader loaded", 0);
        return;
      }
    }
    else {
      // Try forcing it to load
      m_pPSAQShader = GraphicsAPI::instance().createPixelShader("particlePixelShader.hlsl");
      if (!m_pPSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ pixel shader loaded", 0);
        return;
      }
    }
  }

  SPtr<GRasterizerElement> defaultRasDesc = makeSharedPtr<GRasterizerElement>();
  SPtr<GRasterizerState> m_rasterState = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);
}


void
GPostProcessPass::clear() {

}

void
GPostProcessPass::execute() {
  if (m_camera.expired()) {
    WindowManager::showErrorMessage("Error", "No camera assigned to post process pass", 0);
    return;
  }

  SPtr<CCamera> camera = m_camera.lock();

  GraphicsAPI::instance().setViewport(0, 0, camera->m_width, camera->m_height);

  GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVSAQShader);
  GraphicsAPI::instance().getDeviceContext()->setPixelShader(m_pPSAQShader);

  GraphicsAPI::instance().getDeviceContext()->setInputLayout(GraphicsAPI::instance().m_defaultLayout);
  GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

  SPtr<GRenderTargetView> backBufferRT = GraphicsAPI::instance().m_pRenderTargetView;
  SPtr<GDepthStencilView> backBufferDS = GraphicsAPI::instance().m_pDepthStencilView;


  Vector<SPtr<GRenderTargetView>> targets;
  targets.push_back(backBufferRT);
  GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
                                                               targets,
                                                               backBufferDS);

  shadowConstants.shadowView = shadowCamera.lock()->m_view;
  shadowConstants.shadowProjection = shadowCamera.lock()->m_projection;

  m_bufferData.clear();
  m_bufferData.resize(sizeof(shadowConstants));
  memcpy(constantBufferData.data(), &shadowConstants, sizeof(shadowConstants));

  GraphicsAPI::instance().writeToBuffer(shadowCB, constantBufferData);
  Vector<SPtr<GraphicsBuffer>> gbVector;
  gbVector.push_back(shadowCB);
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(3, 1, gbVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(3, 1, gbVector);

  Vector<SPtr<GSamplerState>> ssVec1;
  Vector<SPtr<GSamplerState>> ssVec2;
  ssVec1.push_back(m_pointSampler);
  ssVec2.push_back(m_linearSampler);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

  Vector<SPtr<GShaderResourceView>> srvVector;
  srvVector.push_back(colorRenderTarget->getTexture()->getResource());
  srvVector.push_back(positionRenderTarget->getTexture()->getResource());
  srvVector.push_back(shadowDepthStencil->getTexture()->getResource());

  GraphicsAPI::instance().getDeviceContext()->setShaderResources(srvVector, 0, 3);

  draw(*saqObject.lock().get(), saqMesh, nullptr);

  GraphicsAPI::instance().getDeviceContext()->unbindShaderResource(0);
  GraphicsAPI::instance().getDeviceContext()->unbindShaderResource(1);
  GraphicsAPI::instance().getDeviceContext()->unbindShaderResource(2);
}

void
GPostProcessPass::cleanup() {

}

void
GPostProcessPass::shutdown() {

}



}