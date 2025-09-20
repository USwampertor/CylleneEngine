#include "cyGraphicsAPI.h"
#include "cyWindow.h"

#include <cySceneManager.h>
#include <cyResourceManager.h>
#include <cyCCamera.h>
#include <cyCMeshRenderer.h>
#include <cyLogger.h>
#include <cyGBlendState.h>


namespace CYLLENE_SDK
{
  
void
GraphicsAPI::createDefaultObjects() {
  Vector<GInputLayoutElement> inputDescs = {
    { "POSITION", 0, COLORFORMAT::E::RGB_32_FLOAT,  0, 0,   INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "NORMAL",   0, COLORFORMAT::E::RGB_32_FLOAT,  0, 12,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "TANGENT",  0, COLORFORMAT::E::RGB_32_FLOAT,  0, 24,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "BINORMAL", 0, COLORFORMAT::E::RGB_32_FLOAT,  0, 36,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "COLOR",    0, COLORFORMAT::E::RGBA_32_FLOAT, 0, 48,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "TEXCOORD", 0, COLORFORMAT::E::RG_32_FLOAT,   0, 64,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "BONES",    0, COLORFORMAT::E::RGBA_32_SINT,  0, 72,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "WEIGHTS",  0, COLORFORMAT::E::RGBA_32_FLOAT, 0, 88,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "METADATA", 0, COLORFORMAT::E::RGBA_32_SINT,  0, 104, INPUTCLASSIFICATION::E::PERVERTEX, 0 }
  };

  SPtr<GVertexShader> vColorShader = createVertexShader("vertexShader.hlsl");
  // SPtr<GVertexShader> vShadowShader = createVertexShader("shadowVertex.hlsl");
  // SPtr<GPixelShader> pColorShader = createPixelShader("pixelShader.hlsl");
  // SPtr<GVertexShader> vSAQShader = createVertexShader("saqVertexShader.hlsl");
  // SPtr<GPixelShader> pSAQShader = createPixelShader("saqPixelShader.hlsl");
  // SPtr<GVertexShader> vParticleShader = createVertexShader("particleVertexShader.hlsl");
  // SPtr<GPixelShader> pParticleShader = createPixelShader("particlePixelShader.hlsl");

  WPtr<BBeing> mainCamera = SceneManager::instance().findBeing<BBeing>("DefaultCamera");
  WPtr<BBeing> directionalLight = SceneManager::instance().findBeing<BBeing>("DirectionalLight");

  m_defaultLayout = createInputLayout(inputDescs, vColorShader);

  if (!m_defaultLayout) {
    WindowManager::showErrorMessage("Error", "Error creating Input Layout", 0);
    return;
  }

  m_defaultPipeline = makeSharedPtr<GraphicsPipeline>();
  

  SPtr<GDefaultShadowPass> shadowPass = makeSharedPtr<GDefaultShadowPass>();
  m_defaultPipeline->addPass(shadowPass);
  shadowPass->initialize(directionalLight.lock()->getComponent<CCamera>(), m_defaultPipeline);
  SPtr<GDefaultGeometryPass> colorPass = makeSharedPtr<GDefaultGeometryPass>();
  m_defaultPipeline->addPass(colorPass);
  colorPass->initialize(mainCamera.lock()->getComponent<CCamera>(), m_defaultPipeline);
  SPtr<GDefaultParticlesPass> particlesPass = makeSharedPtr<GDefaultParticlesPass>();
  m_defaultPipeline->addPass(particlesPass);
  particlesPass->initialize(mainCamera.lock()->getComponent<CCamera>(), m_defaultPipeline);
  SPtr<GDefaultPPPass> postProcessPass = makeSharedPtr<GDefaultPPPass>();
  m_defaultPipeline->addPass(postProcessPass);
  postProcessPass->initialize(mainCamera.lock()->getComponent<CCamera>(), m_defaultPipeline);

}

SPtr<GVertexShader>
GraphicsAPI::createVertexShader(const String& resourceName) {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  SPtr<RShader> vsShaderR;
  SPtr<GVertexShader> vShader;
  File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + resourceName);
  vsShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
  vShader = createVertexShader(vsShaderR, "vertex_main");
  return vShader;
}

SPtr<GPixelShader>
GraphicsAPI::createPixelShader(const String& resourceName) {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  SPtr<RShader> psShaderR;
  SPtr<GPixelShader> pShader;
  File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + resourceName);
  psShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
  pShader = createPixelShader(psShaderR, "pixel_main");
  return pShader;
}

SPtr<GGeometryShader>
GraphicsAPI::createGeometryShader(const String& resourceName) {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  SPtr<RShader> gsShaderR;
  SPtr<GGeometryShader> gShader;
  File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + resourceName);
  gsShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
  gShader = createGeometryShader(gsShaderR, "geometry_main");
  return gShader;
}

SPtr<GComputeShader>
GraphicsAPI::createComputeShader(const String& resourceName) {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  SPtr<RShader> csShaderR;
  SPtr<GComputeShader> cShader;
  File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + resourceName);
  csShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
  cShader = createComputeShader(csShaderR, "compute_main");
  return cShader;
}

SPtr<GShader>
GraphicsAPI::createShader(const String& resourceName, const GSHADERTYPE::E& type) {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  SPtr<RShader> sShaderR;
  SPtr<GShader> sShader;
  File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + resourceName);
  sShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
  sShader = +GSHADERTYPE::E::eVERTEX == type ? REINTERPRETPOINTER(GShader, createVertexShader(sShaderR, "vertex_main")) :
            +GSHADERTYPE::E::ePIXEL == type ? REINTERPRETPOINTER(GShader, createPixelShader(sShaderR, "pixel_main")) :
            +GSHADERTYPE::E::eGEOMETRY == type ? REINTERPRETPOINTER(GShader, createGeometryShader(sShaderR, "geometry_main")) :
            +GSHADERTYPE::E::eCOMPUTE == type ? REINTERPRETPOINTER(GShader, createComputeShader(sShaderR, "compute_main")) : nullptr;
  return sShader;
}

void
GraphicsAPI::update() {
  clear(m_clearColor);
  executePipelines();
  present();
}

void
GraphicsAPI::addToRenderPool(uint32 index) {

}

void
GraphicsAPI::removeFromRenderPool(uint32 index) {

}

void
GraphicsAPI::registerToRenderPool(uint32 index) {

}

bool
GraphicsAPI::registerGraphicPass(SPtr<GGraphicPass> newPass) {
  return true;
}

void
GraphicsAPI::executePipelines() {
  auto cameras = SceneManager::instance().findBeingsWithComponent<CCamera>();
  if (cameras.empty()) {
    return;
  }

  for (auto& camera : cameras) {
    WPtr<CCamera> cameraPtr = camera.lock()->getComponent<CCamera>();
    if (cameraPtr.lock()) {
      WPtr<GraphicsPipeline> pipeline = cameraPtr.lock()->m_pipeline;

      for (auto& pass : pipeline.lock()->getPasses()) {
        WPtr<GGraphicPass> passPtr = REINTERPRETPOINTER(GGraphicPass, pass);
        passPtr.lock()->execute();
      }
    }

  }

}


void
GraphicsAPI::draw(SPtr<BBeing> refObject) {
  if (!refObject) {
    return;
  }
  
  SPtr<CMeshRenderer> meshRenderer = refObject->getComponent<CMeshRenderer>().lock();
  SPtr<GMesh> refMesh = REINTERPRETPOINTER(GMesh, getGGraphic<RMesh>(meshRenderer->m_mesh->getName()));
  if (!refMesh) {
    Logger::instance().logWarning("This mesh was never registered when created into the graphics API");
    return;
  }

  Vector<SPtr<GraphicsBuffer>> vertexBuffer;
  vertexBuffer.push_back(refMesh->m_pVertexBuffer);

  const uint32 vertexStride = sizeof(Vertex);
  const uint32 vertexOffset = 0;

  Vector<uint32> vertexStrides;
  vertexStrides.push_back(vertexStride);

  Vector<uint32> vertexOffsets;
  vertexOffsets.push_back(vertexOffset);

  // TODO: Check if this is a better way of doing so
  // Vector<SPtr<GraphicsBuffer>> vertexBuffer{ refMesh->m_pVertexBuffer };
  // Vector<uint32> strides{ sizeof(Vertex) };
  // Vector<uint32> offsets{ 0 };

  getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

  getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);


  SPtr<RMaterial> materialInstance = meshRenderer->m_mesh->m_material;
  const auto& matValues = materialInstance->getDefaultValues();
  SPtr<RShader> shaderRes = materialInstance->getBaseShader().lock(); 
  SPtr<GShader> gShader = REINTERPRETPOINTER(GShader, getGGraphic<RShader>(shaderRes->getName()));
  const Vector<GShaderValue>& shaderVals = gShader->getValues();
  // for each value set Shader Resource

   // Build constant-buffer data & texture array
  Vector<char> materialCBData;
  uint32 cbSize = 0;
  for (auto& v : shaderVals)
    if (v.type != +GSHADERPARAM::E::eTEXTURE2D &&
        v.type != +GSHADERPARAM::E::eTEXTURECUBE &&
        v.type != +GSHADERPARAM::E::eSAMPLERSTATE)
      cbSize = std::max(cbSize, v.offset +
        sizeOfShaderParam(v.type) * v.count);
  materialCBData.resize(cbSize);

  Vector<SPtr<GShaderResourceView>> srvs;
  for (auto& sVal : shaderVals) {
    auto it = matValues.find(sVal.name.c_str());
    if (it == matValues.end()) continue;

    if (sVal.type == +GSHADERPARAM::E::eTEXTURE2D ||
        sVal.type == +GSHADERPARAM::E::eTEXTURECUBE) {
      RTexture* rTex = reinterpret_cast<RTexture*>(it->second);
      if (rTex) {
        SPtr<GTexture> gTex = REINTERPRETPOINTER(GTexture, getGGraphic<RTexture>(rTex->getName()));
        if (gTex) srvs.push_back(gTex->getResource());
      }
    }
    else {
      uint32 size = sizeOfShaderParam(sVal.type) * sVal.count;
      memcpy(materialCBData.data() + sVal.offset, it->second, size);
    }
  }

  // Upload constant-buffer & textures
  if (!materialCBData.empty()) {
    // Ensure material constant buffer exists and is large enough for this shader
    if (!m_materialCB || m_materialCBSize != cbSize) {
      Vector<char> initData;
      initData.resize(cbSize);
      memset(initData.data(), 0, cbSize);
      m_materialCB = createConstantBuffer(initData);
      m_materialCBSize = cbSize;
    }
    writeToBuffer(m_materialCB, materialCBData);
    Vector<SPtr<GraphicsBuffer>> gb{ m_materialCB };
    getDeviceContext()->setVSConstantBuffer(4, 1, gb);
    getDeviceContext()->setPSConstantBuffer(4, 1, gb);
  }
  if (!srvs.empty()) {
    getDeviceContext()->setShaderResources(srvs, 0, srvs.size());
  }

  // --- Per-object constants -----------------------------------------------
  DefaultPerObjectConstantBuffer perObjectConstants;
  Vector<char> constantBufferData;

  constantBufferData.resize(sizeof(DefaultPerObjectConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(DefaultPerObjectConstantBuffer));
  SPtr<GraphicsBuffer> perObjectCB = createConstantBuffer(constantBufferData);

  perObjectConstants.world = refObject->getTransform().lock()->m_worldMatrix;
  perObjectConstants.world.transpose();
  constantBufferData.assign(sizeof(perObjectConstants), 0);
  memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));
  writeToBuffer(perObjectCB, constantBufferData);
  Vector<SPtr<GraphicsBuffer>> objCB{ perObjectCB };
  getDeviceContext()->setVSConstantBuffer(1, 1, objCB);
  getDeviceContext()->setPSConstantBuffer(1, 1, objCB);

  // Draw call ---------------------------------------------------------------
  getDeviceContext()->drawIndexed(refMesh);

  // if (refTexture != nullptr) {
  //   Vector<SPtr<GShaderResourceView>> srvVector;
  //   srvVector.push_back(refTexture->get()->getResource());
  // 
  //   getDeviceContext()->setShaderResources(srvVector, 0, 1);
  // }
  // 
  // perObjectConstants.world = refObject->getTransform().lock()->m_worldMatrix;
  // perObjectConstants.world.transpose();
  // 
  // constantBufferData.clear();
  // constantBufferData.resize(sizeof(perObjectConstants));
  // memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));
  // 
  // GraphicsAPI::instance().writeToBuffer(perObjectCB, constantBufferData);
  // Vector<SPtr<GraphicsBuffer>> gbVector;
  // gbVector.push_back(perObjectCB);
  // getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
  // getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);
  // 
  // getDeviceContext()->drawIndexed(refMesh);
}

void
GraphicsAPI::draw(SPtr<RMesh> rmesh) {
  if (!rmesh) return;

  SPtr<GMesh> refMesh = REINTERPRETPOINTER(GMesh, getGGraphic<RMesh>(rmesh->getName()));
  if (!refMesh) {
    Logger::instance().logWarning("This mesh was never registered when created into the graphics API");
    return;
  }

  // Bind vertex/index buffers
  Vector<SPtr<GraphicsBuffer>> vertexBuffer{ refMesh->m_pVertexBuffer };
  const uint32 vertexStride = sizeof(Vertex);
  const uint32 vertexOffset = 0;
  Vector<uint32> vertexStrides{ vertexStride };
  Vector<uint32> vertexOffsets{ vertexOffset };
  getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);
  getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);

  // Material constants and SRVs
  SPtr<RMaterial> materialInstance = rmesh->m_material;
  if (materialInstance) {
    const auto& matValues = materialInstance->getDefaultValues();
    SPtr<RShader> shaderRes = materialInstance->getBaseShader().lock();
    if (shaderRes) {
      SPtr<GShader> gShader = REINTERPRETPOINTER(GShader, getGGraphic<RShader>(shaderRes->getName()));
      if (gShader) {
        const Vector<GShaderValue>& shaderVals = gShader->getValues();

        uint32 cbSize = 0;
        for (auto& v : shaderVals)
          if (v.type != +GSHADERPARAM::E::eTEXTURE2D &&
              v.type != +GSHADERPARAM::E::eTEXTURECUBE &&
              v.type != +GSHADERPARAM::E::eSAMPLERSTATE)
            cbSize = std::max(cbSize, v.offset + sizeOfShaderParam(v.type) * v.count);

        Vector<char> materialCBData;
        materialCBData.resize(cbSize);

        Vector<SPtr<GShaderResourceView>> srvs;
        for (auto& sVal : shaderVals) {
          auto it = matValues.find(sVal.name.c_str());
          if (it == matValues.end()) continue;
          if (sVal.type == +GSHADERPARAM::E::eTEXTURE2D ||
              sVal.type == +GSHADERPARAM::E::eTEXTURECUBE) {
            RTexture* rTex = reinterpret_cast<RTexture*>(it->second);
            if (rTex) {
              SPtr<GTexture> gTex = REINTERPRETPOINTER(GTexture, getGGraphic<RTexture>(rTex->getName()));
              if (gTex) srvs.push_back(gTex->getResource());
            }
          } else {
            uint32 size = sizeOfShaderParam(sVal.type) * sVal.count;
            memcpy(materialCBData.data() + sVal.offset, it->second, size);
          }
        }

        if (!materialCBData.empty()) {
          if (!m_materialCB || m_materialCBSize != cbSize) {
            Vector<char> initData(cbSize, 0);
            m_materialCB = createConstantBuffer(initData);
            m_materialCBSize = cbSize;
          }
          writeToBuffer(m_materialCB, materialCBData);
          Vector<SPtr<GraphicsBuffer>> gb{ m_materialCB };
          getDeviceContext()->setVSConstantBuffer(4, 1, gb);
          getDeviceContext()->setPSConstantBuffer(4, 1, gb);
        }
        if (!srvs.empty()) {
          getDeviceContext()->setShaderResources(srvs, 0, srvs.size());
        }
      }
    }
  }

  // Per-object constants not available here (no transform). Use identity.
  DefaultPerObjectConstantBuffer perObjectConstants;
  Vector<char> constantBufferData;
  perObjectConstants.world = Matrix4::IDENTITY;
  perObjectConstants.world.transpose();
  constantBufferData.resize(sizeof(DefaultPerObjectConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(DefaultPerObjectConstantBuffer));
  memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));
  SPtr<GraphicsBuffer> perObjectCB = createConstantBuffer(constantBufferData);
  writeToBuffer(perObjectCB, constantBufferData);
  Vector<SPtr<GraphicsBuffer>> objCB{ perObjectCB };
  getDeviceContext()->setVSConstantBuffer(1, 1, objCB);
  getDeviceContext()->setPSConstantBuffer(1, 1, objCB);

  // Draw
  getDeviceContext()->drawIndexed(refMesh);
}

void
GraphicsAPI::draw(SPtr<GMesh> refMesh) {
  if (!refMesh) return;

  Vector<SPtr<GraphicsBuffer>> vertexBuffer{ refMesh->m_pVertexBuffer };
  const uint32 vertexStride = sizeof(Vertex);
  const uint32 vertexOffset = 0;
  Vector<uint32> vertexStrides{ vertexStride };
  Vector<uint32> vertexOffsets{ vertexOffset };
  getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);
  getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);

  getDeviceContext()->drawIndexed(refMesh);
}

void
GraphicsAPI::drawInstanced(SPtr<BBeing> refObject, uint32 instanceCount) {
//   if (!refObject) {
//     return;
//   }
// 
//   SPtr<CMeshRenderer> meshRenderer = refObject->getComponent<CMeshRenderer>().lock();
//   SPtr<GMesh> refMesh = REINTERPRETPOINTER(GMesh, getGGraphic<RMesh>(meshRenderer->m_mesh->getName()));
//   if (!refMesh) {
//     Logger::instance().logWarning("This mesh was never registered when created into the graphics API");
//     return;
//   }
// 
//   SPtr<BBeing> saqObject = SceneManager::instance().createBeing<BBeing>("SAQ_Particles").lock();
//   saqObject->getTransform().lock()->setLocalTransform(Vector3f::ZERO, Vector3f::ONE, Quaternion::IDENTITY);
//   saqObject->createComponent<CMeshRenderer>();
//   SPtr<RMesh> saqMeshRes = ResourceManager::instance().get<RMesh>("SAQ");
//   saqObject->getComponent<CMeshRenderer>().lock()->setMesh(saqMeshRes);
//   SPtr<GMesh> saqMesh = REINTERPRETPOINTER(GMesh, getGGraphic<RMesh>(saqMeshRes->getName()));
//   Vector<SPtr<GraphicsBuffer>> vertexBuffer;
//   vertexBuffer.push_back(saqMesh->m_pVertexBuffer);
// 
//   const uint32 vertexStride = sizeof(Vertex);
//   const uint32 vertexOffset = 0;
// 
//   Vector<uint32> vertexStrides;
//   vertexStrides.push_back(vertexStride);
// 
//   Vector<uint32> vertexOffsets;
//   vertexOffsets.push_back(vertexOffset);
// 
//   GraphicsAPI::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);
// 
//   GraphicsAPI::instance().getDeviceContext()->setIndexBuffer(saqMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);
// 
//   Vector<SPtr<GShaderResourceView>> srvVector;
//   srvVector.push_back(particleGTexture->getResource());
// 
//   GraphicsAPI::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);
// 
//   perObjectConstants.world = saqObject.lock()->getTransform().lock()->m_worldMatrix;
//   perObjectConstants.world.transpose();
// 
//   constantBufferData.clear();
//   constantBufferData.resize(sizeof(perObjectConstants));
//   memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));
// 
//   GraphicsAPI::instance().writeToBuffer(perObjectCB, constantBufferData);
//   gbVector.clear();
//   gbVector.push_back(perObjectCB);
//   GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
//   GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);
// 
//   //GraphicsDX11API::instance().getDeviceContext()->drawIndexed(saqMesh);
//   GraphicsAPI::instance().getDeviceContext()->drawIndexedInstanced(saqMesh, instanceCount);
// 
//   GraphicsAPI::instance().getDeviceContext()->setBlendState(defaultBlend);

  if (!refObject) {
    return;
  }

  SPtr<CMeshRenderer> meshRenderer = refObject->getComponent<CMeshRenderer>().lock();
  SPtr<GMesh> refMesh = REINTERPRETPOINTER(GMesh, getGGraphic<RMesh>(meshRenderer->m_mesh->getName()));
  if (!refMesh) {
    Logger::instance().logWarning("This mesh was never registered when created into the graphics API");
    return;
  }

  Vector<SPtr<GraphicsBuffer>> vertexBuffer;
  vertexBuffer.push_back(refMesh->m_pVertexBuffer);

  const uint32 vertexStride = sizeof(Vertex);
  const uint32 vertexOffset = 0;

  Vector<uint32> vertexStrides;
  vertexStrides.push_back(vertexStride);

  Vector<uint32> vertexOffsets;
  vertexOffsets.push_back(vertexOffset);

  // TODO: Check if this is a better way of doing so
  // Vector<SPtr<GraphicsBuffer>> vertexBuffer{ refMesh->m_pVertexBuffer };
  // Vector<uint32> strides{ sizeof(Vertex) };
  // Vector<uint32> offsets{ 0 };

  getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

  getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);


  SPtr<RMaterial> materialInstance = meshRenderer->m_mesh->m_material;
  const auto& matValues = materialInstance->getDefaultValues();
  SPtr<RShader> shaderRes = materialInstance->getBaseShader().lock();
  SPtr<GShader> gShader = REINTERPRETPOINTER(GShader, getGGraphic<RShader>(shaderRes->getName()));
  const Vector<GShaderValue>& shaderVals = gShader->getValues();
  // for each value set Shader Resource

   // Build constant-buffer data & texture array
  Vector<char> materialCBData;
  uint32 cbSize = 0;
  for (auto& v : shaderVals)
    if (v.type != +GSHADERPARAM::E::eTEXTURE2D &&
      v.type != +GSHADERPARAM::E::eTEXTURECUBE &&
      v.type != +GSHADERPARAM::E::eSAMPLERSTATE)
      cbSize = std::max(cbSize, v.offset +
        sizeOfShaderParam(v.type) * v.count);
  materialCBData.resize(cbSize);

  Vector<SPtr<GShaderResourceView>> srvs;
  for (auto& sVal : shaderVals) {
    auto it = matValues.find(sVal.name.c_str());
    if (it == matValues.end()) continue;

    if (sVal.type == +GSHADERPARAM::E::eTEXTURE2D ||
      sVal.type == +GSHADERPARAM::E::eTEXTURECUBE) {
      RTexture* rTex = reinterpret_cast<RTexture*>(it->second);
      if (rTex) {
        SPtr<GTexture> gTex = REINTERPRETPOINTER(GTexture, getGGraphic<RTexture>(rTex->getName()));
        if (gTex) srvs.push_back(gTex->getResource());
      }
    }
    else {
      uint32 size = sizeOfShaderParam(sVal.type) * sVal.count;
      memcpy(materialCBData.data() + sVal.offset, it->second, size);
    }
  }

  // Upload constant-buffer & textures
  if (!materialCBData.empty()) {
    // Ensure material constant buffer exists and is large enough for this shader
    if (!m_materialCB || m_materialCBSize != cbSize) {
      Vector<char> initData;
      initData.resize(cbSize);
      memset(initData.data(), 0, cbSize);
      m_materialCB = createConstantBuffer(initData);
      m_materialCBSize = cbSize;
    }
    writeToBuffer(m_materialCB, materialCBData);
    Vector<SPtr<GraphicsBuffer>> gb{ m_materialCB };
    getDeviceContext()->setVSConstantBuffer(4, 1, gb);
    getDeviceContext()->setPSConstantBuffer(4, 1, gb);
  }
  if (!srvs.empty()) {
    getDeviceContext()->setShaderResources(srvs, 0, srvs.size());
  }

  // --- Per-object constants -----------------------------------------------
  DefaultPerObjectConstantBuffer perObjectConstants;
  Vector<char> constantBufferData;

  constantBufferData.resize(sizeof(DefaultPerObjectConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(DefaultPerObjectConstantBuffer));
  SPtr<GraphicsBuffer> perObjectCB = createConstantBuffer(constantBufferData);

  perObjectConstants.world = refObject->getTransform().lock()->m_worldMatrix;
  perObjectConstants.world.transpose();
  constantBufferData.assign(sizeof(perObjectConstants), 0);
  memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));
  writeToBuffer(perObjectCB, constantBufferData);
  Vector<SPtr<GraphicsBuffer>> objCB{ perObjectCB };
  getDeviceContext()->setVSConstantBuffer(1, 1, objCB);
  getDeviceContext()->setPSConstantBuffer(1, 1, objCB);

  // Draw call ---------------------------------------------------------------
  getDeviceContext()->drawIndexedInstanced(refMesh, instanceCount);
  SPtr<GBlendElement> defaultBlendDesc = std::make_shared<GBlendElement>();
  defaultBlendDesc->enabled = false;
  defaultBlendDesc->writeMask = BLEND_MASK::E::ALL;
  SPtr<GBlendState> defaultBlend = getDevice()->createBlendState(defaultBlendDesc);
  GraphicsAPI::instance().getDeviceContext()->setBlendState(defaultBlend);

}

}
