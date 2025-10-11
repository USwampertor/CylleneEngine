#include <cyUnitTesting.h>

#include <cyDLLLoader.h>
#include <cyGraphicsAPI.h>
#include <cyWindow.h> 
#include <cyLogger.h> 
#include <cyResourceManager.h>
#include <cyFileSystem.h>
#include <cyGShader.h>
#include <cyGMesh.h>
#include <cyGTexture.h>
#include <cyTime.h>
#include <cyGInputLayout.h>
#include <cyMatrix4.h>
#include <cyRModel.h>
#include <cyRShader.h>
#include <cyBBeing.h>
#include <cyCCamera.h>
#include <cyCTransform.h>
#include <cyCMeshRenderer.h>
#include <cyScene.h>
#include <cySceneManager.h>
#include <cyMath.h> 
#include <cyWindowEvent.h>
/*

// Using namespace for ease of use
using namespace CYLLENE_SDK;

struct ShaderConstants
{
  float time;
  float align1;
  float align2;
  float align3;
} shaderConstants;

struct PerObjectConstantBuffer
{
  Matrix4 world;
} perObjectConstants;

struct PerPassConstantBuffer
{
  Vector4f cameraForward;
  Vector4f cameraRight;
  Vector4f cameraUp;
  Matrix4 view;
  Matrix4 projection;
} perPassConstants;

// struct ShadowConstantBuffer
// {
//   Matrix4 shadowView;
//   Matrix4 shadowProjection;
// } shadowConstants;

DefaultShadowConstantBuffer shadowConstants;

 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
int32
main(int argc, char* argv[])
{
  Logger::startUp();
  ResourceManager::startUp();
  Time::startUp();
  SceneManager::startUp();

  WindowManager::startUp();
  WindowManager::instance().init();
  WindowManager::instance().createWindow("Test", Vector2i(1280, 720));

  // void* hwnd = WindowManager::instance().getWindowHandle(0);


  void* hwnd = WindowManager::instance().getWindowHandle(0);
  loadGFXModule(GFXTYPE::E::eDX11);
  GraphicsAPI::instance().initialize(hwnd);


  SPtr<GDevice> device = GraphicsAPI::instance().getDevice();
  SPtr<GDeviceContext> context = GraphicsAPI::instance().getDeviceContext();

  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";

  auto compileVertexShader = [&](const String fileName, SPtr<RShader>& shaderResource, SPtr<GVertexShader>& vertexShader) {
    File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + fileName);
    shaderResource = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
    vertexShader = GraphicsAPI::instance().createVertexShader(shaderResource, "vertex_main");
    };

  auto compilePixelShader = [&](const String fileName, SPtr<RShader>& shaderResource, SPtr<GPixelShader>& pixelShader) {
    File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + fileName);
    shaderResource = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
    pixelShader = GraphicsAPI::instance().createPixelShader(shaderResource, "pixel_main");
    };

  SPtr<RShader> vsShadowR;
  SPtr<GVertexShader> vShadowShader;
  compileVertexShader("shadowVertex.hlsl", vsShadowR, vShadowShader);

  SPtr<RShader> vsShaderR;
  SPtr<GVertexShader> vShader;
  compileVertexShader("vertexShader.hlsl", vsShaderR, vShader);
  SPtr<RShader> psShaderR;
  SPtr<GPixelShader> pShader;
  compilePixelShader("pixelShader.hlsl", psShaderR, pShader);

  SPtr<RShader> vsSAQR;
  SPtr<GVertexShader> vSAQShader;
  compileVertexShader("saqVertexShader.hlsl", vsSAQR, vSAQShader);
  SPtr<RShader> psSAQR;
  SPtr<GPixelShader> pSAQShader;
  compilePixelShader("saqPixelShader.hlsl", psSAQR, pSAQShader);

  SPtr<RShader> vsParticleR;
  SPtr<GVertexShader> vParticleShader;
  compileVertexShader("particleVertexShader.hlsl", vsParticleR, vParticleShader);
  SPtr<RShader> psParticleR;
  SPtr<GPixelShader> pParticleShader;
  compilePixelShader("particlePixelShader.hlsl", psParticleR, pParticleShader);

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

  SPtr<GInputLayout> pInputLayout = GraphicsAPI::instance().createInputLayout(inputDescs, vShader);

  if (!pInputLayout) {
    WindowManager::showErrorMessage("Error", "Error creating Input Layout", 0);
    return -1;
  }

  SceneManager::instance().createScene("TestScene");
  SceneManager::instance().changeScene("TestScene");

  // shadow camera shit
  WPtr<BBeing> shadowCameraEntity = SceneManager::instance().createBeing<BBeing>("ShadowCamera");
  WPtr<CCamera> shadowCamera = shadowCameraEntity.lock()->createComponent<CCamera>();
  shadowCamera.lock()->setLookAt(Vector3f(25, 30, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
  shadowCamera.lock()->setPerspective(1024, 1024, 0.1f, 100.0f, 60.0f);
  //shadowCamera->setOrthogonal(30, 30, 0.1f, 100.0f);

  // camera shit
  WPtr<BBeing> cameraEntity = SceneManager::instance().createBeing<BBeing>("Camera");
  WPtr<CCamera> camera = cameraEntity.lock()->createComponent<CCamera>();
  camera.lock()->setLookAt(Vector3f(-6, 15, -20), Vector3f(0, 5, 0), Vector3f(0, 1, 0));
  camera.lock()->setPerspective(1280, 720, 0.1f, 200.0f, 70.0f);
  //camera->setOrthogonal(128.f * 0.5f, 72.f * 0.5f, 0.1f, 200.0f);

  Vector<char> shaderConstantsData;
  shaderConstantsData.resize(sizeof(ShaderConstants));
  memset(shaderConstantsData.data(), 0, sizeof(ShaderConstants));
  SPtr<GraphicsBuffer> shaderConstantsBuffer = GraphicsAPI::instance().createConstantBuffer(shaderConstantsData);
  if (!shaderConstantsBuffer) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return -1;
  }

  Vector<char> constantBufferData;

  constantBufferData.resize(sizeof(PerObjectConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(PerObjectConstantBuffer));
  SPtr<GraphicsBuffer> perObjectCB = GraphicsAPI::instance().createConstantBuffer(constantBufferData);
  if (!perObjectCB) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return -1;
  }

  constantBufferData.resize(sizeof(PerPassConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(PerPassConstantBuffer));
  SPtr<GraphicsBuffer> perPassCB = GraphicsAPI::instance().createConstantBuffer(constantBufferData);
  if (!perPassCB) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return -1;
  }

  constantBufferData.resize(sizeof(DefaultShadowConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(DefaultShadowConstantBuffer));
  SPtr<GraphicsBuffer> shadowCB = GraphicsAPI::instance().createConstantBuffer(constantBufferData);
  if (!shadowCB) {
    WindowManager::showErrorMessage("Error", "Error creating Constant Buffer", 0);
    return -1;
  }

  SPtr<RImage> checkerImage;
  SPtr<RTexture> checkerTexture;
  SPtr<GTexture> checkerGTexture;
  {
    checkerImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/cube_base.png");

    checkerTexture = ResourceManager::instance().create<RTexture>("checker");
    checkerTexture->setImage(checkerImage);

    checkerGTexture = GraphicsAPI::instance().createTexture2D(checkerTexture);
  }

  SPtr<RImage> sampleImage;
  SPtr<RTexture> sampleTexture;
  SPtr<GTexture> sampleGTexture;
  {
    sampleImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/beto.png");

    sampleTexture = ResourceManager::instance().create<RTexture>("sample_texture");
    sampleTexture->setImage(sampleImage);

    sampleGTexture = GraphicsAPI::instance().createTexture2D(sampleTexture);
  }

  SPtr<RImage> particleImage;
  SPtr<RTexture> particleRTexture;
  SPtr<GTexture> particleGTexture;
  {
    particleImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/particle.png");

    particleRTexture = ResourceManager::instance().create<RTexture>("particle_texture");
    particleRTexture->setImage(particleImage);

    particleGTexture = GraphicsAPI::instance().createTexture2D(particleRTexture);
  }

  WPtr<BBeing> cubeObject = SceneManager::instance().createBeing<BBeing>("Cube");
  File cubeModelF;
  SPtr<RModel> cubeModelR;
  SPtr<GMesh> cubeMesh;
  {
    cubeModelF = FileSystem::open(resourceDir.fullPath() + "/cube.fbx");
    cubeModelR = ResourceManager::instance().loadFromPath<RModel>(cubeModelF.path());

    cubeMesh = GraphicsAPI::instance().createMesh(cubeModelR->m_meshes[0]);

    cubeObject.lock()->createComponent<CMeshRenderer>(cubeModelR->m_meshes[0]);

    cubeObject.lock()->getTransform().lock()->setWorldPosition(Vector3f(0.0f, 50.0f, 0.0f));
    cubeObject.lock()->getTransform().lock()->setWorldScale(Vector3f(3.0f, 3.0f, 3.0f));
  }

  WPtr<BBeing> floorObject = SceneManager::instance().createBeing<BBeing>("floor");
  File floorModelF;
  SPtr<RModel> floorModelR;
  SPtr<GMesh> floorMesh;
  {
    floorModelF = FileSystem::open(resourceDir.fullPath() + "/saq.fbx");
    floorModelR = ResourceManager::instance().loadFromPath<RModel>(floorModelF.path());

    floorMesh = GraphicsAPI::instance().createMesh(floorModelR->m_meshes[0]);

    floorObject.lock()->createComponent<CMeshRenderer>(floorModelR->m_meshes[0]);

    floorObject.lock()->getTransform().lock()->setWorldPosition(Vector3f(0, 0, 0));
    floorObject.lock()->getTransform().lock()->setWorldScale(Vector3f(20, 20, 1));
    floorObject.lock()->getTransform().lock()->setWorldRotation(Euler(270.0f * Math::DEG2RAD, 0.0f, 0.0f));
  }

  WPtr<BBeing> saqObject = SceneManager::instance().createBeing<BBeing>("SAQ");
  SPtr<GMesh> saqMesh;
  {
    saqMesh = GraphicsAPI::instance().createMesh(floorModelR->m_meshes[0]);

    saqObject.lock()->createComponent<CMeshRenderer>(floorModelR->m_meshes[0]);
  }


  // TODO: Create Render Target and Depth stencil for mirror effect
  SPtr<GTextureElement> shadowDepthStencilTextureDesc = std::make_shared<GTextureElement>();
  shadowDepthStencilTextureDesc->width = 1024;
  shadowDepthStencilTextureDesc->height = 1024;
  shadowDepthStencilTextureDesc->mipLevels = 1;
  shadowDepthStencilTextureDesc->arraySize = 1;
  shadowDepthStencilTextureDesc->format = COLORFORMAT::E::R_32_TYPELESS;
  shadowDepthStencilTextureDesc->sampleCount = 1;
  shadowDepthStencilTextureDesc->sampleQuality = 0;
  shadowDepthStencilTextureDesc->usage = GRESOURCE_USAGE::E::eDEFAULT;
  shadowDepthStencilTextureDesc->bindFlags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE;
  shadowDepthStencilTextureDesc->cpuAccessFlags = 0;
  shadowDepthStencilTextureDesc->miscFlags = 0;
  SPtr<GTexture> shadowDepthStencilTexture = GraphicsAPI::instance().createTexture2D(shadowDepthStencilTextureDesc);

  SPtr<GDepthStencilViewElement> shadowDepthStencilDesc = std::make_shared<GDepthStencilViewElement>();
  shadowDepthStencilDesc->format = COLORFORMAT::E::D_32_FLOAT;
  shadowDepthStencilDesc->width = 1024;
  shadowDepthStencilDesc->height = 1024;
  shadowDepthStencilDesc->flags = GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::SHADERRESOURCE;
  shadowDepthStencilDesc->viewDimension = GDSV_DIMENSION::E::TEXTURE2D;
  SPtr<GDepthStencilView> shadowDepthStencil = GraphicsAPI::instance().getDevice()->createDepthStencilView(shadowDepthStencilDesc, shadowDepthStencilTexture);

  SPtr<GRenderTargetViewElement> colorRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  colorRenderTargetDesc->format = COLORFORMAT::E::RGBA_32_FLOAT;
  colorRenderTargetDesc->width = 1280;
  colorRenderTargetDesc->height = 720;
  colorRenderTargetDesc->flags = GRESOURCE_BINDINGS::E::SHADERRESOURCE | GRESOURCE_BINDINGS::E::RENDERTARGET | GRESOURCE_BINDINGS::E::DEPTHSTENCIL | GRESOURCE_BINDINGS::E::UNORDEREDACCESS;
  SPtr<GRenderTargetView> colorRenderTarget = GraphicsAPI::instance().getDevice()->createRenderTargetView(colorRenderTargetDesc);

  SPtr<GRenderTargetViewElement> positionRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  positionRenderTargetDesc->format = COLORFORMAT::E::RGBA_32_FLOAT;
  positionRenderTargetDesc->width = 1280;
  positionRenderTargetDesc->height = 720;
  positionRenderTargetDesc->flags = GRESOURCE_BINDINGS::E::SHADERRESOURCE | GRESOURCE_BINDINGS::E::RENDERTARGET | GRESOURCE_BINDINGS::E::UNORDEREDACCESS;
  SPtr<GRenderTargetView> positionRenderTarget = GraphicsAPI::instance().getDevice()->createRenderTargetView(positionRenderTargetDesc);

  SPtr<GRasterizerElement> defaultRasDesc = std::make_shared<GRasterizerElement>();
  SPtr<GRasterizerState> defaultRasterizer = GraphicsAPI::instance().getDevice()->createRasterizerState(defaultRasDesc);

  SPtr<GBlendElement> defaultBlendDesc = std::make_shared<GBlendElement>();
  defaultBlendDesc->enabled = false;
  defaultBlendDesc->writeMask = BLEND_MASK::E::ALL;
  SPtr<GBlendState> defaultBlend = GraphicsAPI::instance().getDevice()->createBlendState(defaultBlendDesc);

  SPtr<GBlendElement> alphaBlendDesc = std::make_shared<GBlendElement>();
  alphaBlendDesc->enabled = true;
  alphaBlendDesc->writeMask = BLEND_MASK::E::ALL;
  SPtr<GBlendState> alphaBlend = GraphicsAPI::instance().getDevice()->createBlendState(alphaBlendDesc);

  // TODO: Create other rasterizers

  // TODO: Do sampler states
  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;

  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  SPtr<GSamplerState> pointSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  SPtr<GSamplerState> linearSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eTRILINEAL;
  SPtr<GSamplerState> anisoSampler = GraphicsAPI::instance().getDevice()->createSamplerState(samplerDesc);

  // SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  bool running = true;
  float time = 0.0f;
  float deltaTime;

  WindowManager::instance().m_windowEvent.addListener([&](WPtr<WindowEvent> event) {
    if (+EVENTTYPE::E::eQUIT == event.lock()->type) {
      std::cout << "Window closed" << std::endl;
      running = false;
    }
    });
  while (running) {
    Time::instance().update();

    DELTA_TYPE::E deltaType = DELTA_TYPE::E::eMILLISECOND;
    deltaTime = Time::instance().deltaTime(deltaType);
    time += deltaTime * 0.001f;
    WindowManager::instance().update();
    const uint32 vertexStride = sizeof(Vertex);
    const uint32 indexStride = sizeof(unsigned short);
    const uint32 vertexOffset = 0;

    SPtr<GRenderTargetView> backBufferRT = GraphicsAPI::instance().m_pRenderTargetView;
    SPtr<GDepthStencilView> backBufferDS = GraphicsAPI::instance().m_pDepthStencilView;

    auto draw = [&](BBeing& refObject, const SPtr<GMesh>& refMesh, const SPtr<GTexture>* refTexture) {
      Vector<SPtr<GraphicsBuffer>> vertexBuffer;
      vertexBuffer.push_back(refMesh->m_pVertexBuffer);

      Vector<uint32> vertexStrides;
      vertexStrides.push_back(vertexStride);

      Vector<uint32> vertexOffsets;
      vertexOffsets.push_back(vertexOffset);

      GraphicsAPI::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

      GraphicsAPI::instance().getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);

      if (refTexture != nullptr) {
        Vector<SPtr<GShaderResourceView>> srvVector;
        srvVector.push_back(refTexture->get()->getResource());

        GraphicsAPI::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);
      }

      perObjectConstants.world = refObject.getTransform().lock()->m_worldMatrix;
      perObjectConstants.world.transpose();

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perObjectConstants));
      memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));

      GraphicsAPI::instance().writeToBuffer(perObjectCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perObjectCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);

      GraphicsAPI::instance().getDeviceContext()->drawIndexed(refMesh);
      };

    shaderConstants.time = time;
    memcpy(shaderConstantsData.data(), &shaderConstants, sizeof(shaderConstants));
    GraphicsAPI::instance().writeToBuffer(shaderConstantsBuffer, shaderConstantsData);

    // Clear
    {
      GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(shadowDepthStencil,
        GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);

      GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(colorRenderTarget, Color::BLACK);
      GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(colorRenderTarget.get()->getDepthStencil(),
        GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);

      GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(positionRenderTarget, Color::BLACK);

      GraphicsAPI::instance().getDeviceContext()->clearRenderTargetView(backBufferRT, Color::MISSING);
      GraphicsAPI::instance().getDeviceContext()->clearDepthStencilView(backBufferDS,
        GCLEAR_FLAGS::E::DEPTH | GCLEAR_FLAGS::E::STENCIL, 1.0f, 0);
    }

    // Shadow pass
    {
      GraphicsAPI::instance().setViewport(0, 0, 1024, 1024);

      GraphicsAPI::instance().getDeviceContext()->setVertexShader(vShadowShader);
      GraphicsAPI::instance().getDeviceContext()->setPixelShader(nullptr);

      GraphicsAPI::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> nullRTs;
      nullRTs.push_back(nullptr);
      GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
        nullRTs,
        shadowDepthStencil);

      perPassConstants.cameraForward = shadowCamera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight = shadowCamera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp = shadowCamera.lock()->m_view.getUpVector();
      perPassConstants.view = shadowCamera.lock()->m_view;
      perPassConstants.projection = shadowCamera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsAPI::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      draw(*floorObject.lock().get(), floorMesh, nullptr);
      draw(*cubeObject.lock().get(), cubeMesh, nullptr);
    }

    // Color pass
    {
      GraphicsAPI::instance().setViewport(0, 0, 1280, 720);

      GraphicsAPI::instance().getDeviceContext()->setVertexShader(vShader);
      GraphicsAPI::instance().getDeviceContext()->setPixelShader(pShader);

      GraphicsAPI::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(colorRenderTarget);
      targets.push_back(positionRenderTarget);
      GraphicsAPI::instance().getDeviceContext()->setRenderTargets(2,
        targets,
        colorRenderTarget.get()->getDepthStencil());

      Vector<SPtr<GraphicsBuffer>> scVector;
      scVector.push_back(shaderConstantsBuffer);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

      perPassConstants.cameraForward = camera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight = camera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp = camera.lock()->m_view.getUpVector();
      perPassConstants.view = camera.lock()->m_view;
      perPassConstants.projection = camera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsAPI::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
      GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
      GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

      draw(*floorObject.lock().get(), floorMesh, &checkerGTexture);

      cubeObject.lock()->getTransform().lock()->setWorldPosition(Vector3f(0.0f, 3.0f + Math::cos(time), 0.0f));
      cubeObject.lock()->getTransform().lock()->setWorldRotation(Euler(0.2f * time * 3.0f,
        0.2f * time * 1.0f,
        0.2f * time * 9.0f));
      draw(*cubeObject.lock().get(), cubeMesh, &sampleGTexture);
    }

    // Particles pass
    {
      GraphicsAPI::instance().setViewport(0, 0, 1280, 720);

      GraphicsAPI::instance().getDeviceContext()->setVertexShader(vParticleShader);
      GraphicsAPI::instance().getDeviceContext()->setPixelShader(pParticleShader);

      GraphicsAPI::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(colorRenderTarget);
      GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
        targets,
        colorRenderTarget.get()->getDepthStencil());

      GraphicsAPI::instance().getDeviceContext()->setBlendState(alphaBlend);

      Vector<SPtr<GraphicsBuffer>> scVector;
      scVector.push_back(shaderConstantsBuffer);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

      perPassConstants.cameraForward = camera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight = camera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp = camera.lock()->m_view.getUpVector();
      perPassConstants.view = camera.lock()->m_view;
      perPassConstants.projection = camera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsAPI::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
      GraphicsAPI::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
      GraphicsAPI::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

      //draw(camera, saqObject, saqMesh, &particleGTexture);

      Vector<SPtr<GraphicsBuffer>> vertexBuffer;
      vertexBuffer.push_back(saqMesh->m_pVertexBuffer);

      Vector<uint32> vertexStrides;
      vertexStrides.push_back(vertexStride);

      Vector<uint32> vertexOffsets;
      vertexOffsets.push_back(vertexOffset);

      GraphicsAPI::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

      GraphicsAPI::instance().getDeviceContext()->setIndexBuffer(saqMesh->m_pIndexBuffer, COLORFORMAT::E::R_32_UINT, 0);

      Vector<SPtr<GShaderResourceView>> srvVector;
      srvVector.push_back(particleGTexture->getResource());

      GraphicsAPI::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);

      perObjectConstants.world = saqObject.lock()->getTransform().lock()->m_worldMatrix;
      perObjectConstants.world.transpose();

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perObjectConstants));
      memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));

      GraphicsAPI::instance().writeToBuffer(perObjectCB, constantBufferData);
      gbVector.clear();
      gbVector.push_back(perObjectCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);

      //GraphicsDX11API::instance().getDeviceContext()->drawIndexed(saqMesh);
      GraphicsAPI::instance().getDeviceContext()->drawIndexedInstanced(saqMesh, 100);

      GraphicsAPI::instance().getDeviceContext()->setBlendState(defaultBlend);
    }

    // Composition
    {
      GraphicsAPI::instance().setViewport(0, 0, 1280, 720);

      GraphicsAPI::instance().getDeviceContext()->setVertexShader(vSAQShader);
      GraphicsAPI::instance().getDeviceContext()->setPixelShader(pSAQShader);

      GraphicsAPI::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsAPI::instance().getDeviceContext()->setPrimitiveTopology(GPRIMITIVE_TOPOLOGY::E::eTRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(backBufferRT);
      GraphicsAPI::instance().getDeviceContext()->setRenderTargets(1,
        targets,
        backBufferDS);

      shadowConstants.shadowView = shadowCamera.lock()->m_view;
      shadowConstants.shadowProjection = shadowCamera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(shadowConstants));
      memcpy(constantBufferData.data(), &shadowConstants, sizeof(shadowConstants));

      GraphicsAPI::instance().writeToBuffer(shadowCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(shadowCB);
      GraphicsAPI::instance().getDeviceContext()->setVSConstantBuffer(3, 1, gbVector);
      GraphicsAPI::instance().getDeviceContext()->setPSConstantBuffer(3, 1, gbVector);

      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
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

    GraphicsAPI::instance().present();
  }


  WindowManager::shutDown();

  return 0;

  //   doctest::Context dcontext;
  // 
  //   dcontext.applyCommandLine(argc, argv);
  // 
  //   int32 res = dcontext.run();
  // 
  //   if (dcontext.shouldExit()) {
  //     return res;
  //   }
  // 
  //   dcontext.clearFilters();
  // 
  //   return res + EXIT_SUCCESS;
}



 */





























//////////////////////////////////////////////////////////////////////////



#include <cyUnitTesting.h>

#include <cyGraphicsAPI.h>
#include <cyDLLLoader.h>
#include <cyLogger.h>
#include <cyResourceManager.h>
#include <cySceneManager.h>
#include <cyWindow.h>
#include <cyTime.h>
#include <cyCMeshRenderer.h>
#include <cyCCamera.h>
#include <cyBBeing.h>
#include <cyRModel.h>
#include <cyRShader.h>
#include <cyCLight.h>

// Using namespace for ease of use
using namespace CYLLENE_SDK;


/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 * 
 */
int32
main(int argc, char* argv[])
{
  Logger::startUp();
  ResourceManager::startUp();
  Time::startUp();
  SceneManager::startUp();

  WindowManager::startUp();
  WindowManager::instance().init();
  WindowManager::instance().createWindow("Test", Vector2i(1280, 720));

  void* hwnd = WindowManager::instance().getWindowHandle(0);
  loadGFXModule(GFXTYPE::E::eDX11);
  GraphicsAPI::instance().initialize(hwnd);

  // Build a simple scene: camera, light, and a cube with a material
  
  SceneManager::instance().createScene("TestScene");
  SceneManager::instance().changeScene("TestScene");


  // Create Default Camera
  SPtr<BBeing> camBeing = SceneManager::instance().createBeing<BBeing>("DefaultCamera").lock();
  auto camXform = camBeing->getTransform().lock();
  camXform->setLocalTransform(Vector3f(0.0f, 2.0f, -6.0f), Vector3f::ONE, Quaternion::IDENTITY);
  WPtr<CCamera> camera = camBeing->createComponent<CCamera>();
  camera.lock()->setPerspective(1280.0f, 720.0f, 0.1f, 100.0f, 60.0f);
  camera.lock()->setLookAt(Vector3f(0,0,0), Vector3f::UP);
  camera.lock()->m_isLightCamera = false;

  // Create Directional Light with a camera for shadows
  SPtr<BBeing> lightBeing = SceneManager::instance().createBeing<BBeing>("DirectionalLight").lock();
  auto ltx = lightBeing->getTransform().lock();
  ltx->setLocalTransform(Vector3f(5.0f, 6.0f, -5.0f), Vector3f::ONE, Quaternion::IDENTITY);
  WPtr<CCamera> lightCam = lightBeing->createComponent<CCamera>();
  lightCam.lock()->setPerspective(1024.0f, 1024.0f, 0.1f, 200.0f, 60.0f);
  lightCam.lock()->setLookAt(Vector3f(0,0,0), Vector3f::UP);
  lightCam.lock()->m_isLightCamera = true;
  // Add a light component and enable shadow casting
  auto lightComp = lightBeing->createComponent<CLight>();
  lightComp.lock()->m_castsShadows = true;

  // Load resources (model, texture, material) and register with GraphicsAPI
  Path resDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  // Preload shaders so materials can resolve baseShader and register GShaders
  {
    File psFile = FileSystem::open(resDir.fullPath() + "/pixelShader.hlsl");
    SPtr<RShader> ps = ResourceManager::instance().loadFromPath<RShader>(psFile.path());
    if (ps) { GraphicsAPI::instance().registerResource(ps); }
    File vsFile = FileSystem::open(resDir.fullPath() + "/vertexShader.hlsl");
    SPtr<RShader> vs = ResourceManager::instance().loadFromPath<RShader>(vsFile.path());
    if (vs) { GraphicsAPI::instance().registerResource(vs); }
  }
  // Load cube model and register into graphics
  File cubeFile = FileSystem::open(resDir.fullPath() + "/cube.fbx");
  SPtr<RModel> cubeModel = ResourceManager::instance().loadFromPath<RModel>(cubeFile.path());
  if (cubeModel) { GraphicsAPI::instance().registerResource(cubeModel); }
  // Load texture referenced by default material and register
  File baseTexFile = FileSystem::open(resDir.fullPath() + "/cube_base.png");
  SPtr<RImage> baseImage = ResourceManager::instance().loadFromPath<RImage>(baseTexFile.path());
  SPtr<RTexture> baseTex = ResourceManager::instance().create<RTexture>(baseImage->getName());
  baseTex->setImage(baseImage);
  if (baseTex) { GraphicsAPI::instance().registerResource(baseTex); }
  // Load a simple material
  File matFile = FileSystem::open(resDir.fullPath() + "/defaultPBR.mat");
  SPtr<RMaterial> mat = ResourceManager::instance().loadFromPath<RMaterial>(matFile.path());

  // Create a being with a cube mesh and assign the material
  SPtr<RMesh> cubeMesh = (cubeModel && !cubeModel->m_meshes.empty()) ? cubeModel->m_meshes[0] : nullptr;
  if (cubeMesh && mat) { cubeMesh->m_material = mat; }
  SPtr<BBeing> cubeBeing = SceneManager::instance().createBeing<BBeing>("Cube").lock();
  cubeBeing->getTransform().lock()->setLocalTransform(Vector3f(0,0,0), Vector3f::ONE, Quaternion::IDENTITY);
  auto mr = cubeBeing->createComponent<CMeshRenderer>(cubeModel->m_meshes[0]);
  // if (cubeMesh) { mr.lock()->setMesh(cubeMesh); }

  // Ensure SAQ model for post-process exists and register it
  File saqFile = FileSystem::open(resDir.fullPath() + "/saq.fbx");
  SPtr<RModel> saqModel = ResourceManager::instance().loadFromPath<RModel>(saqFile.path());
  if (saqModel) { GraphicsAPI::instance().registerResource(saqModel); }

  // Create default pipeline and passes bound to the camera/light beings
  GraphicsAPI::instance().createDefaultObjects();

  //  SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  bool running = true;
  float time = 0.0f;
  float deltaTime;
  WindowManager::instance().m_windowEvent.addListener([&](WPtr<WindowEvent> event) {
    if (+EVENTTYPE::E::eQUIT == event.lock()->type) {
      std::cout << "Window closed" << std::endl;
      running = false;
    }
    });
  while (running) {
    // Now use the API
    // eventQueue->update();
    Time::instance().update();
    WindowManager::instance().update();
    DELTA_TYPE::E deltaType = DELTA_TYPE::E::eMILLISECOND;
    deltaTime = Time::instance().deltaTime(deltaType);
    time += deltaTime * 0.001f;

    GraphicsAPI::instance().update();
    // if (time >= 5.0f) {
    //   running = false; // Stop after 5 seconds
    // }
  }

  // Cleanup
  GraphicsAPI::shutDown();
  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}


