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
#include <cyMath.h> 

// Using namespace for ease of use
using namespace CYLLENE_SDK;

struct MatrixCollection
{
  Matrix4 world;
  Matrix4 view;
  Matrix4 projection;
} matrices;

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


  WindowManager::startUp();
  WindowManager::instance().init();
  WindowManager::instance().createWindow("Test", Vector2i(1280, 720));

  void* hwnd = WindowManager::instance().getWindowHandle(0);

  GraphicsDX11API::startUp<GraphicsDX11API>();
  GraphicsDX11API::instance().initialize(hwnd);

  SPtr<GDevice> device = GraphicsDX11API::instance().getDevice();
  SPtr<GDeviceContext> context = GraphicsDX11API::instance().getDeviceContext();

  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  File shaderVSF = FileSystem::open(resourceDir.fullPath() + "/vertexShader.hlsl");
  SPtr<RShader> vsShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderVSF.path());
  File shaderPSF = FileSystem::open(resourceDir.fullPath() + "/pixelShader.hlsl");
  SPtr<RShader> psShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderPSF.path());

  SPtr<GVertexShader> vShader = GraphicsDX11API::instance().createVertexShader(vsShaderR,
    "vertex_main");

  SPtr<GPixelShader> pShader = GraphicsDX11API::instance().createPixelShader(psShaderR,
    "pixel_main");


  Vector<GInputLayoutElement> inputDescs = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 64,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BONES", 0, DXGI_FORMAT_R32G32B32A32_SINT,    0, 72,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    0, 88,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "METADATA", 0, DXGI_FORMAT_R32G32B32A32_SINT,    0, 104,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  SPtr<GInputLayout> pInputLayout = GraphicsDX11API::instance().createInputLayout(inputDescs, vShader);

  if (!pInputLayout) {
    WindowManager::ShowErrorMessage("Error", "Error creating Input Layout");
    return -1;
  }

  // camera shit
  BBeing cameraEntity("Camera");
  cameraEntity.createComponent<CTransform>();
  SPtr<CCamera> camera = cameraEntity.createComponent<CCamera>();
  camera->setLookAt(Vector3f(-1, 1, -1), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
  camera->setPerspective(1280, 720, 0.1f, 200.0f, 70.0f);


  matrices.world = Matrix4::IDENTITY;
  matrices.view = camera->m_view;
  matrices.projection = camera->m_projection;

  matrices.world.transpose();
  matrices.view.transpose();
  matrices.projection.transpose();

  Vector<char> data;
  data.resize(sizeof(MatrixCollection));
  memcpy(data.data(), &matrices, sizeof(MatrixCollection));
  SPtr<GraphicsBuffer> constantBuffer = GraphicsDX11API::instance().createConstantBuffer(data);
  if (!constantBuffer) {
    WindowManager::ShowErrorMessage("Error", "Error creating Constant Buffer");
    return -1;
  }

  File modelF = FileSystem::open(resourceDir.fullPath() + "/cube.fbx");
  SPtr<RModel> modelR = ResourceManager::instance().loadFromPath<RModel>(modelF.path());

  SPtr<RImage> newImage = ResourceManager::instance().loadFromPath<RImage>(resourceDir.fullPath() + "/roger2.png");
  SPtr<RTexture> newTexture = ResourceManager::instance().create<RTexture>("cube_base");
  newTexture->setImage(newImage);

  // SPtr<GTexture> newGTexture = GraphicsDX11API::instance().createTexture2D(newTexture);
  String testName = GraphicsDX11API::instance().generateResourceID<RTexture>("cube_base");
  SPtr<GTexture> newGTexture = GraphicsDX11API::instance().m_textureRenderPool[Hash<String>()(testName)];// GraphicsDX11API::instance().createTexture2D(newTexture);


  BBeing cubeObject("saq");
  cubeObject.createComponent<CTransform>();
  cubeObject.createComponent<CMeshRenderer>(modelR->m_meshes[0]);
  cubeObject.getTransform()->setPosition(Vector3f(0, 0, 0));
  cubeObject.getTransform()->setScale(Vector3f(2, 2, 2));

  SPtr<GMesh> gMesh = GraphicsDX11API::instance().createMesh(modelR->m_meshes[0]);

  // TODO: Create Render Target and Depth stencil for mirror effect
  
  SPtr<GRasterizerElement> defaultRasDesc = std::make_shared<GRasterizerElement>();
  SPtr<GRasterizerState> defaultRasterizer = GraphicsDX11API::instance().getDevice()->createRasterizerState(defaultRasDesc);
  
  // TODO: Create other rasterizers
  
  // TODO: Do sampler states
  SPtr<GSamplerStateElement> samplerDesc = std::make_shared<GSamplerStateElement>();
  samplerDesc->filter = SAMPLERFILTER::E::ePOINT;
  samplerDesc->addressU = TEXTUREMODE::E::eCLAMP;
  samplerDesc->addressV = TEXTUREMODE::E::eCLAMP;
  SPtr<GSamplerState> pointSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);
  samplerDesc->filter = SAMPLERFILTER::E::eLINEAR;
  SPtr<GSamplerState> linearSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);
  samplerDesc->filter = SAMPLERFILTER::E::eTRILINEAL;
  SPtr<GSamplerState> anisoSampler = GraphicsDX11API::instance().getDevice()->createSamplerState(samplerDesc);


  SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  bool running = true;
  float timer = 0.0f;
  float deltaTime;
  while (running) {
    eventQueue->update();
    Time::instance().update();

    DELTA_TYPE::E deltaType = DELTA_TYPE::E::MILLISECOND;
    deltaTime = Time::instance().deltaTime(deltaType);
    timer += deltaTime * 0.001f;
    // cubeObject.getTransform()->rotate(Vector3f(0, deltaTime * Math::DEG2RAD * 0.1f,0));

    
    if (!eventQueue->empty()) {
      WindowEvent event = eventQueue->front();
      eventQueue->pop();

      switch (event.type)
      {
      case xwin::EventType::MouseMove:
        //mouse.x, mouse.y
        break;
      case xwin::EventType::Close:
        WindowManager::instance().destroyWindow(0);
        running = false;
        break;
      default:
        // Do nothing
        break;
      }
    }

    GraphicsDX11API::instance().setViewport(0, 0, 1280, 720);
    Vector<SPtr<GRenderTargetView>> targets;
    targets.push_back(GraphicsDX11API::instance().m_pRenderTargetView);
    GraphicsDX11API::instance().getDeviceContext()->setRenderTargets(1, 
                                                                     targets, 
                                                                     GraphicsDX11API::instance().m_pDepthStencilView);

    
    GraphicsDX11API::instance().getDeviceContext()->clearRenderTargetView(GraphicsDX11API::instance().m_pRenderTargetView, Color::MISSING);

    // TODO: Clear render target from reflection

    GraphicsDX11API::instance().getDeviceContext()->clearDepthStencilView(GraphicsDX11API::instance().m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // TODO: Clear depth stencil from reflection

    GraphicsDX11API::instance().getDeviceContext()->setVertexShader(vShader);
    GraphicsDX11API::instance().getDeviceContext()->setPixelShader(pShader);

    uint32 vertexStride = sizeof(Vertex);
    uint32 indexStride = sizeof(unsigned short);
    uint32 vertexOffset = 0;

    GraphicsDX11API::instance().getDeviceContext()->setInputLayout(pInputLayout);
    GraphicsDX11API::instance().getDeviceContext()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Vector<SPtr<GraphicsBuffer>> vertexBuffer;
    vertexBuffer.push_back(gMesh->m_pVertexBuffer);
    Vector<uint32> vertexStrides;
    vertexStrides.push_back(vertexStride);
    Vector<uint32> vertexOffsets;
    vertexOffsets.push_back(vertexOffset);
    GraphicsDX11API::instance().getDeviceContext()->setVertexBuffers(0, 1, vertexBuffer, vertexStrides, vertexOffsets);

    GraphicsDX11API::instance().getDeviceContext()->setIndexBuffer(gMesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    Vector<SPtr<GShaderResourceView>> srvVector;
    srvVector.push_back(newGTexture->getResource());
    GraphicsDX11API::instance().getDeviceContext()->setShaderResources(srvVector, 0, 1);
    
    Vector<SPtr<GSamplerState>> ssVec1;
    Vector<SPtr<GSamplerState>> ssVec2;
    ssVec1.push_back(pointSampler);
    ssVec2.push_back(linearSampler);
    GraphicsDX11API::instance().getDeviceContext()->setSamplers(0, 1, ssVec1);
    GraphicsDX11API::instance().getDeviceContext()->setSamplers(1, 1, ssVec2);

    float t = Math::sin(timer) * 0.5f + 0.5f;
    // camera->setPerspective(1280, 720, 0.1f, 1000.0f, 30 + (60.0f * t));

    // cubeObject.getTransform()->setScale(Vector3f(Math::sin()));

    Quaternion rotationQuat(Euler(timer * 0.5, timer * 0.5, timer * 0.5));
    Vector3f eyePosition = Vector3f(0.0f, 4.0f, -5.0f);
    eyePosition = rotationQuat.rotate(eyePosition);

    camera->setLookAt(eyePosition, Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    cubeObject.getTransform()->setPosition(Vector3f(0.0f, Math::sin(timer), 0.0f));
    matrices.world = cubeObject.getTransform()->m_tMatrix; // Matrix4::IDENTITY; // cubeObject.getTransform()->m_tMatrix;
    matrices.view = camera->m_view;
    matrices.projection = camera->m_projection;
    matrices.world.transpose();
    data.clear();
    data.resize(sizeof(matrices));
    memcpy(data.data(), &matrices, sizeof(matrices));

    GraphicsDX11API::instance().writeToBuffer(constantBuffer, data);
    Vector<SPtr<GraphicsBuffer>> gbVector;
    gbVector.push_back(constantBuffer);
    GraphicsDX11API::instance().getDeviceContext()->setConstantBuffer(0, 1, gbVector);

    GraphicsDX11API::instance().getDeviceContext()->drawIndexed(gMesh);

    // GraphicsDX11API::instance().clear(Color::MISSING);
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


