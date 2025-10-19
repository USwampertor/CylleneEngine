#include "cyGGraphicPass.h"

#include <cyResourceManager.h>
#include <cyCMeshRenderer.h>
#include <cySceneManager.h>
#include <cyRShader.h>
#include <cyGraphicsAPI.h>
#include <cyLogger.h>
#include <cyWindow.h>
#include <cyCCamera.h>
#include <cyTime.h>
#include <cyCLight.h>



namespace CYLLENE_SDK {

GDefaultShadowPass::~GDefaultShadowPass() {
  shutdown();
}

void
GDefaultShadowPass::initialize(WPtr<CCamera> newParentCamera, 
                               WPtr<GraphicsPipeline> newParentPipeline) {
  GGraphicPass::initialize(newParentCamera, newParentPipeline);
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

  // Per-pass constant buffer
  m_bufferData.clear();
  m_bufferData.resize(sizeof(DefaultPerPassConstantBuffer));
  memset(m_bufferData.data(), 0, sizeof(DefaultPerPassConstantBuffer));
  m_perPassCB = GraphicsAPI::instance().createConstantBuffer(m_bufferData);
  if (!m_perPassCB) {
    WindowManager::showErrorMessage("Error", "Error creating Per-Pass Constant Buffer", 0);
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
GDefaultShadowPass::clear() {
  GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(m_shadowDSV,
    GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL /*D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, 1.0f, 0);
}

void
GDefaultShadowPass::execute() {
  GraphicsAPI::instance().setViewport(0, 0, 1024, 1024);
  GraphicsAPI::instance().getDeviceContext()->setVertexShader(m_pVShadowShader);
  GraphicsAPI::instance().getDeviceContext()->setPixelShader(nullptr);
  GraphicsAPI::instance().getDeviceContext()->setInputLayout(GraphicsAPI::instance().m_defaultLayout);
  GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);
  Vector<SPtr<GRenderTargetView>> nullRTs;
  nullRTs.push_back(nullptr);
  // We'll bind per-light DSVs below

  // TODO: Check if we should be doing this for each camera that is attached to a light 
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

  // Build per-light shadow maps and render from each light's view
  m_shadowDSTs.clear();
  m_shadowDSVs.clear();
  auto lights = SceneManager::instance().findBeingsWithComponent<CLight>();
  auto renderables = SceneManager::instance().findBeingsWithComponent<CMeshRenderer>();
  for (auto& l : lights) {
    auto lightBeing = l.lock();
    if (!lightBeing) continue;
    auto lightComp = lightBeing->getComponent<CLight>().lock();
    if (!lightComp || !lightComp->m_castsShadows) continue;

    // Create per-light shadow texture and DSV
    SPtr<GTextureElement> texDesc = makeSharedPtr<GTextureElement>();
    texDesc->width = 1024;
    texDesc->height = 1024;
    texDesc->mipLevels = 1;
    texDesc->arraySize = 1;
    texDesc->format = COLORFORMAT::E::R_32_TYPELESS;
    texDesc->sampleCount = 1;
    texDesc->sampleQuality = 0;
    texDesc->usage = GRESOURCE_USAGE::E::eDEFAULT;
    texDesc->bindFlags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE;
    texDesc->cpuAccessFlags = 0;
    texDesc->miscFlags = 0;

    SPtr<GTexture> shadowTex = GraphicsAPI::instance().createTexture2D(texDesc);
    if (!shadowTex) continue;

    SPtr<GDepthStencilViewElement> dsvDesc = makeSharedPtr<GDepthStencilViewElement>();
    dsvDesc->format = COLORFORMAT::E::D_32_FLOAT;
    dsvDesc->width = 1024;
    dsvDesc->height = 1024;
    dsvDesc->flags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE;
    dsvDesc->viewDimension = GDSV_DIMENSION::E::TEXTURE2D;
    SPtr<GDepthStencilView> shadowDSV = GraphicsAPI::instance().getDevice()->createDepthStencilView(dsvDesc, shadowTex);
    if (!shadowDSV) continue;

    m_shadowDSTs.push_back(shadowTex);
    m_shadowDSVs.push_back(shadowDSV);

    // Bind this light's DSV and clear
    GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1, nullRTs, shadowDSV);
    GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(shadowDSV,
      GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);

    // Select light-space camera (if present) or fallback
    SPtr<CCamera> lightCam = lightBeing->getComponent<CCamera>().lock();
    if (!lightCam) lightCam = m_camera.lock();
    if (!lightCam) continue;

    // Update per-pass constants from light camera
    m_perPassConstants.cameraForward = lightCam->m_view.getForwardVector();
    m_perPassConstants.cameraRight = lightCam->m_view.getRightVector();
    m_perPassConstants.cameraUp = lightCam->m_view.getUpVector();
    m_perPassConstants.view = lightCam->m_view;
    m_perPassConstants.projection = lightCam->m_projection;

    m_bufferData.clear();
    m_bufferData.resize(sizeof(m_perPassConstants));
    memcpy(m_bufferData.data(), &m_perPassConstants, sizeof(m_perPassConstants));
    GraphicsAPI::instance().writeToBuffer(m_perPassCB, m_bufferData);
    Vector<SPtr<GraphicsBuffer>> gb;
    gb.push_back(m_perPassCB);
    GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gb);
    GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gb);

    // Render scene geometry to this light's shadow map
    for (auto& obj : renderables) {
      if (auto sp = obj.lock()) {
        GraphicsAPI::instance().draw(sp);
      }
    }
  }
}

void
GDefaultShadowPass::cleanup() {

}

void
GDefaultShadowPass::shutdown() {

}

GDefaultGeometryPass::~GDefaultGeometryPass() {
  shutdown();
}

void
GDefaultGeometryPass::initialize(WPtr<CCamera> newParentCamera, 
                                 WPtr<GraphicsPipeline> newParentPipeline) {
  GGraphicPass::initialize(newParentCamera, newParentPipeline);

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
  m_shaderConstantsBuffer = GraphicsAPI::instance().createConstantBuffer(shaderConstantsData);
  if (!m_shaderConstantsBuffer) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return;
  }

  // Per-pass constant buffer
  Vector<char> perPassInit;
  perPassInit.resize(sizeof(DefaultPerPassConstantBuffer));
  memset(perPassInit.data(), 0, sizeof(DefaultPerPassConstantBuffer));
  m_perPassCB = GraphicsAPI::instance().createConstantBuffer(perPassInit);
  if (!m_perPassCB) {
    WindowManager::showErrorMessage("Error", "Error creating Per-Pass Constant Buffer", 0);
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
  
  // Blend states
  SPtr<GBlendElement> defaultBlendDesc = std::make_shared<GBlendElement>();
  defaultBlendDesc->enabled = false;
  defaultBlendDesc->writeMask = BLEND_MASK::E::ALL;
  m_defaultBlend = GraphicsAPI::instance().getDevice()->createBlendState(defaultBlendDesc);

}

void
GDefaultGeometryPass::clear() {
  GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(m_colorRenderTarget, Color::BLACK);
  GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(m_colorRenderTarget.get()->getDepthStencil(),
    GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);

  GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(m_positionRenderTarget, Color::BLACK);
}

void
GDefaultGeometryPass::execute() {

  // TODO: MOVE THIS OUT;
  DELTA_TYPE::E deltaType = DELTA_TYPE::E::eMILLISECOND;
  float deltaTime;
  deltaTime = Time::instance().deltaTime(deltaType);
  m_time += deltaTime * 0.001f;
  Vector<char> shaderConstantsData;
  shaderConstantsData.resize(sizeof(m_shaderConstants));
  m_shaderConstants.time = m_time;
  memcpy(shaderConstantsData.data(), &m_shaderConstants, sizeof(m_shaderConstants));
  GraphicsAPI::instance().writeToBuffer(m_shaderConstantsBuffer, shaderConstantsData);


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
  
  GraphicsAPI::instance().getDeviceContext()->setBlendState(m_defaultBlend);

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


  // Draw all renderables
  auto renderables = SceneManager::instance().findBeingsWithComponent<CMeshRenderer>();
  for (auto& obj : renderables) {
    if (auto sp = obj.lock()) {
      GraphicsAPI::instance().draw(sp);
    }
  }
}

void
GDefaultGeometryPass::cleanup() {

}


void
GDefaultGeometryPass::shutdown() {

}

void
GDefaultParticlesPass::initialize(WPtr<CCamera> newParentCamera, 
                                  WPtr<GraphicsPipeline> newParentPipeline) {
  GGraphicPass::initialize(newParentCamera, newParentPipeline);

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

  // Color/Geometry pass dependency
  WPtr<GGraphicPass> geometryPass = STATICPOINTER(GGraphicPass, m_parentPipeline.lock()->getAt(1));
  m_dependencies.try_emplace(1, geometryPass);

  // Shader constants buffer
  Vector<char> shaderConstantsData;
  shaderConstantsData.resize(sizeof(DefaultShaderConstants));
  memset(shaderConstantsData.data(), 0, sizeof(DefaultShaderConstants));
  m_shaderConstantsBuffer = GraphicsAPI::instance().createConstantBuffer(shaderConstantsData);

  // Per-pass constant buffer
  Vector<char> perPassInit;
  perPassInit.resize(sizeof(DefaultPerPassConstantBuffer));
  memset(perPassInit.data(), 0, sizeof(DefaultPerPassConstantBuffer));
  m_perPassCB = GraphicsAPI::instance().createConstantBuffer(perPassInit);

  // Blend states
  SPtr<GBlendElement> defaultBlendDesc = std::make_shared<GBlendElement>();
  defaultBlendDesc->enabled = false;
  defaultBlendDesc->writeMask = BLEND_MASK::E::ALL;
  m_defaultBlend = GraphicsAPI::instance().getDevice()->createBlendState(defaultBlendDesc);

  SPtr<GBlendElement> alphaBlendDesc = std::make_shared<GBlendElement>();
  alphaBlendDesc->enabled = true;
  alphaBlendDesc->writeMask = BLEND_MASK::E::ALL;
  m_alphaBlend = GraphicsAPI::instance().getDevice()->createBlendState(alphaBlendDesc);

  // Prepare SAQ object once (optional reuse)
  SPtr<BBeing> saqObject = SceneManager::instance().createBeing<BBeing>("SAQ_Particles").lock();
  saqObject->getTransform().lock()->setLocalTransform(Vector3f::ZERO, Vector3f::ONE, Quaternion::IDENTITY);
  SPtr<RModel> saqMeshRes = ResourceManager::instance().get<RModel>("saq");
  saqObject->createComponent<CMeshRenderer>(saqMeshRes->m_meshes[0]);
  m_cachedSAQObject = saqObject;

}


void
GDefaultParticlesPass::clear() {

}

void
GDefaultParticlesPass::execute() {

  // TODO: MOVE THIS OUT;
  DELTA_TYPE::E deltaType = DELTA_TYPE::E::eMILLISECOND;
  float deltaTime;
  deltaTime = Time::instance().deltaTime(deltaType);
  m_time += deltaTime * 0.001f;
  Vector<char> shaderConstantsData;
  shaderConstantsData.resize(sizeof(m_shaderConstants));
  m_shaderConstants.time = m_time;
  memcpy(shaderConstantsData.data(), &m_shaderConstants, sizeof(m_shaderConstants));
  GraphicsAPI::instance().writeToBuffer(m_shaderConstantsBuffer, shaderConstantsData);

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

  SPtr<GDefaultGeometryPass> geometryPass = STATICPOINTER(GDefaultGeometryPass, m_dependencies[1].lock());

  targets.push_back( geometryPass->getOutputRenderTarget());
  GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
                                                               targets,
                                                               geometryPass->getOutputRenderTarget().get()->getDepthStencil());

  GraphicsAPI::instance().getDeviceContext()->setBlendState(m_alphaBlend);

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

  // Draw instanced SAQ if available
  if (auto so = m_cachedSAQObject.lock()) {
    GraphicsAPI::instance().drawInstanced(so, 100);
  }

  GraphicsAPI::instance().getDeviceContext()->setBlendState(m_defaultBlend);
}

void
GDefaultParticlesPass::cleanup() {

}

void
GDefaultParticlesPass::shutdown() {
  
}


void
GDefaultPPPass::initialize(WPtr<CCamera> newParentCamera, 
                           WPtr<GraphicsPipeline> newParentPipeline) {
  GGraphicPass::initialize(newParentCamera, newParentPipeline);

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
      m_pVSAQShader = GraphicsAPI::instance().createVertexShader("saqVertexShader.hlsl");
      if (!m_pVSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ vertex shader loaded", 0);
        return;
      }
    }
  }

  if (!m_pPSAQShader) {
    SPtr<RShader> psSAQR = ResourceManager::instance().get<RShader>("saqPixelShader");
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
      m_pPSAQShader = GraphicsAPI::instance().createPixelShader("saqPixelShader.hlsl");
      if (!m_pPSAQShader) {
        WindowManager::showErrorMessage("Error",
          "There is no default SAQ pixel shader loaded", 0);
        return;
      }
    }
  }

  SPtr<GRasterizerElement> defaultRasDesc = makeSharedPtr<GRasterizerElement>();
  SPtr<GRasterizerState> m_rasterState = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);
  
  // Create samplers
  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;
  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  m_pointSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  m_linearSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  WPtr<GGraphicPass> shadowPass = STATICPOINTER(GGraphicPass, m_parentPipeline.lock()->getAt(0));
  m_dependencies.try_emplace(0, shadowPass);

  WPtr<GGraphicPass> geometryPass = STATICPOINTER(GGraphicPass, m_parentPipeline.lock()->getAt(1));
  m_dependencies.try_emplace(1, geometryPass);
}


void
GDefaultPPPass::clear() {
  SPtr<GRenderTargetView> backBufferRT = GraphicsAPI::instance().m_pRenderTargetView;
  SPtr<GDepthStencilView> backBufferDS = GraphicsAPI::instance().m_pDepthStencilView;

  GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(backBufferRT, Color::MISSING);
  GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(backBufferDS,
    GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);
}

void
GDefaultPPPass::execute() {
  
  // TODO: Check if this should be done for each camera that is attached to a light
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

  SPtr<GDefaultShadowPass> shadowPass = STATICPOINTER(GDefaultShadowPass, m_dependencies[0].lock());
  SPtr<GDefaultGeometryPass> geometryPass = STATICPOINTER(GDefaultGeometryPass, m_dependencies[1].lock());

  // Build shadow constants locally (avoid taking address of rvalues)
  DefaultShadowConstantBuffer shadowConsts;
  shadowConsts.shadowView = m_camera.lock()->m_view;
  shadowConsts.shadowProjection = m_camera.lock()->m_projection;
  // Fill number of shadows from shadow pass
  shadowConsts.shadowCount = (uint32)shadowPass->getShadowTextures().size();

  m_bufferData.clear();

  m_bufferData.resize(sizeof(shadowConsts));
  memcpy(m_bufferData.data(), 
         &shadowConsts, 
         sizeof(shadowConsts));

  GraphicsAPI::instance().writeToBuffer(shadowPass->getOutputBuffer(), m_bufferData);
  Vector<SPtr<GraphicsBuffer>> gbVector;
  gbVector.push_back(shadowPass->getOutputBuffer());
  GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(3, 1, gbVector);
  GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(3, 1, gbVector);

  Vector<SPtr<GSamplerState>> ssVec1;
  Vector<SPtr<GSamplerState>> ssVec2;
  ssVec1.push_back(m_pointSampler);
  ssVec2.push_back(m_linearSampler);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
  GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

  Vector<SPtr<GShaderResourceView>> srvVector;
  srvVector.push_back(geometryPass->getOutputRenderTarget()->getTexture()->getResource());
  srvVector.push_back(geometryPass->getPositionRenderTarget()->getTexture()->getResource());
  // Append all shadow SRVs from helper (per-light)
  Vector<SPtr<GShaderResourceView>> shadowSrvs = shadowPass->getShadowSRVs();
  for (auto& srv : shadowSrvs) {
    if (srv) srvVector.push_back(srv);
  }

  if (!srvVector.empty()) {
    GraphicsAPI::instance().getDeviceContext()->setShaderResources(srvVector, 0, (uint32)srvVector.size());
  }
  //SPtr<BBeing> saqObject = SceneManager::instance().createBeing<BBeing>("SAQ_Particles").lock();
  //saqObject->getTransform().lock()->setLocalTransform(Vector3f::ZERO, Vector3f::ONE, Quaternion::IDENTITY);
  //SPtr<RModel> saqMeshRes = ResourceManager::instance().get<RModel>("saq");
  //saqObject->createComponent<CMeshRenderer>(saqMeshRes->m_meshes[0]);
  
  // Full-screen draw using SAQ mesh only (no material rebinding)
  SPtr<GMesh> saqMesh = STATICPOINTER(GMesh, GraphicsAPI::instance().getGGraphic<RMesh>("saq_sub0"));
  if (saqMesh) {
    GraphicsAPI::instance().draw(saqMesh);
  }

  for (uint32 i = 0; i < (uint32)srvVector.size(); ++i) {
    GraphicsAPI::instance().getDeviceContext()->unbindShaderResource(i);
  }
}

void
GDefaultPPPass::cleanup() {

}

void
GDefaultPPPass::shutdown() {

}



}
