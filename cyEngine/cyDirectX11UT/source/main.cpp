#include <cyUnitTesting.h>

#include <cyGraphicsDX11API.h>
#include <cyGDX11Device.h>
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

ShadowConstantBuffer shadowConstants;

/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
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

  GraphicsDX11API::startUp<GraphicsDX11API>();
  GraphicsDX11API::instance().initialize(hwnd);

  SPtr<GDevice> device = GraphicsDX11API::instance().getDevice();
  SPtr<GDeviceContext> context = GraphicsDX11API::instance().getDeviceContext();

  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";

  auto compileVertexShader = [&](const String fileName, SPtr<RShader>& shaderResource, SPtr<GVertexShader>& vertexShader) {
    File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + fileName);
    shaderResource = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
    vertexShader = GraphicsDX11API::instance().createVertexShader(shaderResource, "vertex_main");
    };

  auto compilePixelShader = [&](const String fileName, SPtr<RShader>& shaderResource, SPtr<GPixelShader>& pixelShader) {
    File shaderFile = FileSystem::open(resourceDir.fullPath() + "/" + fileName);
    shaderResource = ResourceManager::instance().loadFromPath<RShader>(shaderFile.path());
    pixelShader = GraphicsDX11API::instance().createPixelShader(shaderResource, "pixel_main");
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
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,     0, 36,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,        0, 64,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BONES",    0, DXGI_FORMAT_R32G32B32A32_SINT,   0, 72,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 88,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "METADATA", 0, DXGI_FORMAT_R32G32B32A32_SINT,   0, 104, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  SPtr<GInputLayout> pInputLayout = GraphicsDX11API::instance().createInputLayout(inputDescs, vShader);

  if (!pInputLayout) {
    WindowManager::ShowErrorMessage("Error", "Error creating Input Layout");
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
  SPtr<GraphicsBuffer> shaderConstantsBuffer = GraphicsDX11API::instance().createConstantBuffer(shaderConstantsData);
  if (!shaderConstantsBuffer) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return -1;
  }

  Vector<char> constantBufferData;

  constantBufferData.resize(sizeof(PerObjectConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(PerObjectConstantBuffer));
  SPtr<GraphicsBuffer> perObjectCB = GraphicsDX11API::instance().createConstantBuffer(constantBufferData);
  if (!perObjectCB) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return -1;
  }

  constantBufferData.resize(sizeof(PerPassConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(PerPassConstantBuffer));
  SPtr<GraphicsBuffer> perPassCB = GraphicsDX11API::instance().createConstantBuffer(constantBufferData);
  if (!perPassCB) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return -1;
  }

  constantBufferData.resize(sizeof(ShadowConstantBuffer));
  memset(constantBufferData.data(), 0, sizeof(ShadowConstantBuffer));
  SPtr<GraphicsBuffer> shadowCB = GraphicsDX11API::instance().createConstantBuffer(constantBufferData);
  if (!shadowCB) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return -1;
  }

  SPtr<RImage> checkerImage;
  SPtr<RTexture> checkerTexture;
  SPtr<GTexture> checkerGTexture;
  {
    checkerImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/cube_base.png");

    checkerTexture = ResourceManager::instance().create<RTexture>("checker");
    checkerTexture->setImage(checkerImage);

    checkerGTexture = GraphicsDX11API::instance().createTexture2D(checkerTexture);
  }

  SPtr<RImage> sampleImage;
  SPtr<RTexture> sampleTexture;
  SPtr<GTexture> sampleGTexture;
  {
    sampleImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/beto.png");

    sampleTexture = ResourceManager::instance().create<RTexture>("sample_texture");
    sampleTexture->setImage(sampleImage);

    sampleGTexture = GraphicsDX11API::instance().createTexture2D(sampleTexture);
  }

  SPtr<RImage> particleImage;
  SPtr<RTexture> particleRTexture;
  SPtr<GTexture> particleGTexture;
  {
    particleImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/particle.png");

    particleRTexture = ResourceManager::instance().create<RTexture>("particle_texture");
    particleRTexture->setImage(particleImage);

    particleGTexture = GraphicsDX11API::instance().createTexture2D(particleRTexture);
  }

  WPtr<BBeing> cubeObject = SceneManager::instance().createBeing<BBeing>("Cube");
  File cubeModelF;
  SPtr<RModel> cubeModelR;
  SPtr<GMesh> cubeMesh;
  {
    cubeModelF = FileSystem::open(resourceDir.fullPath() + "/cube.fbx");
    cubeModelR = ResourceManager::instance().loadFromPath<RModel>(cubeModelF.path());

    cubeMesh = GraphicsDX11API::instance().createMesh(cubeModelR->m_meshes[0]);

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

    floorMesh = GraphicsDX11API::instance().createMesh(floorModelR->m_meshes[0]);

    floorObject.lock()->createComponent<CMeshRenderer>(floorModelR->m_meshes[0]);

    floorObject.lock()->getTransform().lock()->setWorldPosition(Vector3f(0, 0, 0));
    floorObject.lock()->getTransform().lock()->setWorldScale(Vector3f(20, 20, 1));
    floorObject.lock()->getTransform().lock()->setWorldRotation(Euler(270.0f * Math::DEG2RAD, 0.0f, 0.0f));
  }

  WPtr<BBeing> saqObject = SceneManager::instance().createBeing<BBeing>("SAQ");
  SPtr<GMesh> saqMesh;
  {
    saqMesh = GraphicsDX11API::instance().createMesh(floorModelR->m_meshes[0]);

    saqObject.lock()->createComponent<CMeshRenderer>(floorModelR->m_meshes[0]);
  }


  // TODO: Create Render Target and Depth stencil for mirror effect
  SPtr<GTextureElement> shadowDepthStencilTextureDesc = std::make_shared<GTextureElement>();
  shadowDepthStencilTextureDesc->width          = 1024;
  shadowDepthStencilTextureDesc->height         = 1024;
  shadowDepthStencilTextureDesc->mipLevels      = 1;
  shadowDepthStencilTextureDesc->arraySize      = 1;
  shadowDepthStencilTextureDesc->format         = DXGI_FORMAT_R32_TYPELESS;
  shadowDepthStencilTextureDesc->sampleCount    = 1;
  shadowDepthStencilTextureDesc->sampleQuality  = 0;
  shadowDepthStencilTextureDesc->usage          = D3D11_USAGE_DEFAULT;
  shadowDepthStencilTextureDesc->bindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  shadowDepthStencilTextureDesc->cpuAccessFlags = 0;
  shadowDepthStencilTextureDesc->miscFlags      = 0;
  SPtr<GTexture> shadowDepthStencilTexture = GraphicsDX11API::instance().createTexture2D(shadowDepthStencilTextureDesc);

  SPtr<GDepthStencilViewElement> shadowDepthStencilDesc = std::make_shared<GDepthStencilViewElement>();
  shadowDepthStencilDesc->format  = DXGI_FORMAT_D32_FLOAT;
  shadowDepthStencilDesc->width   = 1024;
  shadowDepthStencilDesc->height  = 1024;
  shadowDepthStencilDesc->flags   = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  shadowDepthStencilDesc->viewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  SPtr<GDepthStencilView> shadowDepthStencil = GraphicsDX11API::instance().getDevice()->createDepthStencilView(shadowDepthStencilDesc, shadowDepthStencilTexture);

  SPtr<GRenderTargetViewElement> colorRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  colorRenderTargetDesc->format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  colorRenderTargetDesc->width = 1280;
  colorRenderTargetDesc->height = 720;
  colorRenderTargetDesc->flags   = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_UNORDERED_ACCESS;
  SPtr<GRenderTargetView> colorRenderTarget = GraphicsDX11API::instance().getDevice()->createRenderTargetView(colorRenderTargetDesc);
  
  SPtr<GRenderTargetViewElement> positionRenderTargetDesc = std::make_shared<GRenderTargetViewElement>();
  positionRenderTargetDesc->format  = DXGI_FORMAT_R32G32B32A32_FLOAT;
  positionRenderTargetDesc->width   = 1280;
  positionRenderTargetDesc->height  = 720;
  positionRenderTargetDesc->flags   = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
  SPtr<GRenderTargetView> positionRenderTarget = GraphicsDX11API::instance().getDevice()->createRenderTargetView(positionRenderTargetDesc);
  
  SPtr<GRasterizerElement> defaultRasDesc = std::make_shared<GRasterizerElement>();
  SPtr<GRasterizerState> defaultRasterizer = GraphicsDX11API::instance().getDevice()->createRasterizerState(defaultRasDesc);

  SPtr<GBlendElement> defaultBlendDesc = std::make_shared<GBlendElement>();
  defaultBlendDesc->enabled = false;
  defaultBlendDesc->writeMask = BLEND_MASK::E::ALL;
  SPtr<GBlendState> defaultBlend = GraphicsDX11API::instance().getDevice()->createBlendState(defaultBlendDesc);

  SPtr<GBlendElement> alphaBlendDesc = std::make_shared<GBlendElement>();
  alphaBlendDesc->enabled = true;
  alphaBlendDesc->writeMask = BLEND_MASK::E::ALL;
  SPtr<GBlendState> alphaBlend = GraphicsDX11API::instance().getDevice()->createBlendState(alphaBlendDesc);
  
  // TODO: Create other rasterizers
  
  // TODO: Do sampler states
  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;

  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  SPtr<GSamplerState> pointSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  SPtr<GSamplerState> linearSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);

  samplerDesc->filter = SAMPLERFILTER::E::eTRILINEAL;
  SPtr<GSamplerState> anisoSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);

  SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  bool running = true;
  float time = 0.0f;
  float deltaTime;

  WindowManager::instance().m_windowEvent.addListener([&](SPtr<WindowEvent> event) {
    if (+EVENTTYPE::E::eCLOSE == event->type) {
      std::cout << "Window closed" << std::endl;
      running = false;
    }
  });
  while (running) {
    eventQueue->update();
    Time::instance().update();

    DELTA_TYPE::E deltaType = DELTA_TYPE::E::eMILLISECOND;
    deltaTime = Time::instance().deltaTime(deltaType);
    time += deltaTime * 0.001f;
    WindowManager::instance().update();
    const uint32 vertexStride = sizeof(Vertex);
    const uint32 indexStride = sizeof(unsigned short);
    const uint32 vertexOffset = 0;

    SPtr<GRenderTargetView> backBufferRT = GraphicsDX11API::instance().m_pRenderTargetView;
    SPtr<GDepthStencilView> backBufferDS = GraphicsDX11API::instance().m_pDepthStencilView;
    
    auto draw = [&](BBeing& refObject, const SPtr<GMesh>& refMesh, const SPtr<GTexture>* refTexture) {
      Vector<SPtr<GraphicsBuffer>> vertexBuffer;
      vertexBuffer.push_back(refMesh->m_pVertexBuffer);

      Vector<uint32> vertexStrides;
      vertexStrides.push_back(vertexStride);

      Vector<uint32> vertexOffsets;
      vertexOffsets.push_back(vertexOffset);

      GraphicsDX11API::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

      GraphicsDX11API::instance().getDeviceContext()->setIndexBuffer(refMesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

      if (refTexture != nullptr) {
        Vector<SPtr<GShaderResourceView>> srvVector;
        srvVector.push_back(refTexture->get()->getResource());

        GraphicsDX11API::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);
      }

      perObjectConstants.world = refObject.getTransform().lock()->m_worldMatrix;
      perObjectConstants.world.transpose();

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perObjectConstants));
      memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));

      GraphicsDX11API::instance().writeToBuffer(perObjectCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perObjectCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);

      GraphicsDX11API::instance().getDeviceContext()->drawIndexed(refMesh);
    };

    shaderConstants.time = time;
    memcpy(shaderConstantsData.data(), &shaderConstants, sizeof(shaderConstants));
    GraphicsDX11API::instance().writeToBuffer(shaderConstantsBuffer, shaderConstantsData);

    // Clear
    {
      GraphicsDX11API::instance().getDeviceContext()->clearDepthStencilView(shadowDepthStencil,
                                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

      GraphicsDX11API::instance().getDeviceContext()->clearRenderTargetView(colorRenderTarget, Color::BLACK);
      GraphicsDX11API::instance().getDeviceContext()->clearDepthStencilView(colorRenderTarget.get()->getDepthStencil(),
                                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
      
      GraphicsDX11API::instance().getDeviceContext()->clearRenderTargetView(positionRenderTarget, Color::BLACK);

      GraphicsDX11API::instance().getDeviceContext()->clearRenderTargetView(backBufferRT, Color::MISSING);
      GraphicsDX11API::instance().getDeviceContext()->clearDepthStencilView(backBufferDS,
                                                                            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    // Shadow pass
    {
      GraphicsDX11API::instance().setViewport(0, 0, 1024, 1024);

      GraphicsDX11API::instance().getDeviceContext()->setVertexShader(vShadowShader);
      GraphicsDX11API::instance().getDeviceContext()->setPixelShader(nullptr);

      GraphicsDX11API::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsDX11API::instance().getDeviceContext()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> nullRTs;
      nullRTs.push_back(nullptr);
      GraphicsDX11API::instance().getDeviceContext()->setRenderTargets(1,
                                                                       nullRTs,
                                                                       shadowDepthStencil);
      
      perPassConstants.cameraForward  = shadowCamera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight    = shadowCamera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp       = shadowCamera.lock()->m_view.getUpVector();
      perPassConstants.view           = shadowCamera.lock()->m_view;
      perPassConstants.projection     = shadowCamera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsDX11API::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      draw(*floorObject.lock().get(), floorMesh, nullptr);
      draw(*cubeObject.lock().get(), cubeMesh, nullptr);
    }

    // Color pass
    {
      GraphicsDX11API::instance().setViewport(0, 0, 1280, 720);

      GraphicsDX11API::instance().getDeviceContext()->setVertexShader(vShader);
      GraphicsDX11API::instance().getDeviceContext()->setPixelShader(pShader);

      GraphicsDX11API::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsDX11API::instance().getDeviceContext()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(colorRenderTarget);
      targets.push_back(positionRenderTarget);
      GraphicsDX11API::instance().getDeviceContext()->setRenderTargets(2,
                                                                       targets, 
                                                                       colorRenderTarget.get()->getDepthStencil());

      Vector<SPtr<GraphicsBuffer>> scVector;
      scVector.push_back(shaderConstantsBuffer);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

      perPassConstants.cameraForward  = camera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight    = camera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp       = camera.lock()->m_view.getUpVector();
      perPassConstants.view           = camera.lock()->m_view;
      perPassConstants.projection     = camera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsDX11API::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

      draw(*floorObject.lock().get(), floorMesh, &checkerGTexture);

      cubeObject.lock()->getTransform().lock()->setWorldPosition(Vector3f(0.0f, 3.0f + Math::cos(time), 0.0f));
      cubeObject.lock()->getTransform().lock()->setWorldRotation(Euler(0.2f * time * 3.0f,
                                             0.2f * time * 1.0f,
                                             0.2f * time * 9.0f));
      draw(*cubeObject.lock().get(), cubeMesh, &sampleGTexture);
    }

    // Particles pass
    {
      GraphicsDX11API::instance().setViewport(0, 0, 1280, 720);

      GraphicsDX11API::instance().getDeviceContext()->setVertexShader(vParticleShader);
      GraphicsDX11API::instance().getDeviceContext()->setPixelShader(pParticleShader);

      GraphicsDX11API::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsDX11API::instance().getDeviceContext()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(colorRenderTarget);
      GraphicsDX11API::instance().getDeviceContext()->setRenderTargets(1,
                                                                       targets,
                                                                       colorRenderTarget.get()->getDepthStencil());

      GraphicsDX11API::instance().getDeviceContext()->setBlendState(alphaBlend);
      
      Vector<SPtr<GraphicsBuffer>> scVector;
      scVector.push_back(shaderConstantsBuffer);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(0, 1, scVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(0, 1, scVector);

      perPassConstants.cameraForward  = camera.lock()->m_view.getForwardVector();
      perPassConstants.cameraRight    = camera.lock()->m_view.getRightVector();
      perPassConstants.cameraUp       = camera.lock()->m_view.getUpVector();
      perPassConstants.view           = camera.lock()->m_view;
      perPassConstants.projection     = camera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perPassConstants));
      memcpy(constantBufferData.data(), &perPassConstants, sizeof(perPassConstants));

      GraphicsDX11API::instance().writeToBuffer(perPassCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(perPassCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(2, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(2, 1, gbVector);

      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

      //draw(camera, saqObject, saqMesh, &particleGTexture);

      Vector<SPtr<GraphicsBuffer>> vertexBuffer;
      vertexBuffer.push_back(saqMesh->m_pVertexBuffer);

      Vector<uint32> vertexStrides;
      vertexStrides.push_back(vertexStride);

      Vector<uint32> vertexOffsets;
      vertexOffsets.push_back(vertexOffset);

      GraphicsDX11API::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

      GraphicsDX11API::instance().getDeviceContext()->setIndexBuffer(saqMesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

      Vector<SPtr<GShaderResourceView>> srvVector;
      srvVector.push_back(particleGTexture->getResource());

      GraphicsDX11API::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);

      perObjectConstants.world = saqObject.lock()->getTransform().lock()->m_worldMatrix;
      perObjectConstants.world.transpose();

      constantBufferData.clear();
      constantBufferData.resize(sizeof(perObjectConstants));
      memcpy(constantBufferData.data(), &perObjectConstants, sizeof(perObjectConstants));

      GraphicsDX11API::instance().writeToBuffer(perObjectCB, constantBufferData);
      gbVector.clear();
      gbVector.push_back(perObjectCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(1, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(1, 1, gbVector);

      //GraphicsDX11API::instance().getDeviceContext()->drawIndexed(saqMesh);
      GraphicsDX11API::instance().getDeviceContext()->drawIndexedInstanced(saqMesh, 100);

      GraphicsDX11API::instance().getDeviceContext()->setBlendState(defaultBlend);
    }

    // Composition
    {
      GraphicsDX11API::instance().setViewport(0, 0, 1280, 720);

      GraphicsDX11API::instance().getDeviceContext()->setVertexShader(vSAQShader);
      GraphicsDX11API::instance().getDeviceContext()->setPixelShader(pSAQShader);

      GraphicsDX11API::instance().getDeviceContext()->setInputLayout(pInputLayout);
      GraphicsDX11API::instance().getDeviceContext()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      Vector<SPtr<GRenderTargetView>> targets;
      targets.push_back(backBufferRT);
      GraphicsDX11API::instance().getDeviceContext()->setRenderTargets(1, 
                                                                       targets, 
                                                                       backBufferDS);

      shadowConstants.shadowView        = shadowCamera.lock()->m_view;
      shadowConstants.shadowProjection  = shadowCamera.lock()->m_projection;

      constantBufferData.clear();
      constantBufferData.resize(sizeof(shadowConstants));
      memcpy(constantBufferData.data(), &shadowConstants, sizeof(shadowConstants));

      GraphicsDX11API::instance().writeToBuffer(shadowCB, constantBufferData);
      Vector<SPtr<GraphicsBuffer>> gbVector;
      gbVector.push_back(shadowCB);
      GraphicsDX11API::instance().getDeviceContext()->setVSConstantBuffer(3, 1, gbVector);
      GraphicsDX11API::instance().getDeviceContext()->setPSConstantBuffer(3, 1, gbVector);
      
      Vector<SPtr<GSamplerState>> ssVec1;
      Vector<SPtr<GSamplerState>> ssVec2;
      ssVec1.push_back(pointSampler);
      ssVec2.push_back(linearSampler);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
      GraphicsDX11API::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

      Vector<SPtr<GShaderResourceView>> srvVector;
      srvVector.push_back(colorRenderTarget->getTexture()->getResource());
      srvVector.push_back(positionRenderTarget->getTexture()->getResource());
      srvVector.push_back(shadowDepthStencil->getTexture()->getResource());

      GraphicsDX11API::instance().getDeviceContext()->setShaderResources(srvVector, 0, 3);

      draw(*saqObject.lock().get(), saqMesh, nullptr);

      GraphicsDX11API::instance().getDeviceContext()->unbindShaderResource(0);
      GraphicsDX11API::instance().getDeviceContext()->unbindShaderResource(1);
      GraphicsDX11API::instance().getDeviceContext()->unbindShaderResource(2);
    }
    
    GraphicsDX11API::instance().present();
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


