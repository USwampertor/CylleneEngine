
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
  File cubeFile = FileSystem::open(resDir.fullPath() + "/cube.cym");
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
  auto mr = cubeBeing->createComponent<CMeshRenderer>();
  if (cubeMesh) { mr.lock()->setMesh(cubeMesh); }

  // Ensure SAQ model for post-process exists and register it
  File saqFile = FileSystem::open(resDir.fullPath() + "/saq.cym");
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


