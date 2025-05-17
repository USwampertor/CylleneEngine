/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	main.cpp
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/21
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

// #define SDL_MAIN_USE_CALLBACKS 1
#include <iostream>

#include <cyBBeing.h>
#include <cyCoreUTPrerequisites.h>
#include <cyCrashHandler.h>
#include <cyFileSystem.h>
#include <cyGameMode.h>
#include <cyRImage.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyRMesh.h>
#include <cyRModel.h>
#include <cyResourceManager.h>
#include <cyRTexture.h>
#include <cyTime.h>
#include <cyTriangle.h>
#include <cyCTransform.h>
#include <cyUnitTesting.h>
#include <cyVector2f.h>
#include <cyVertex.h>
#include <cyWindow.h>


using namespace CYLLENE_SDK;


using ManualShader = std::function<Color(float, float)>;



int32
main(int32 argc, char* argv[]) {


  std::cout << argv[0] << std::endl;

  std::cout << "Starting Core Unit Test" << std::endl;

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;

  return 0;
}

TEST_CASE("[module] testing module startup") {
  CrashHandler::startUp();
  CHECK(CrashHandler::isStarted());
  SmartPointers::startUp();
  CHECK(CrashHandler::isStarted());
  Logger::startUp();
  CHECK(Logger::isStarted());
  Time::startUp();
  CHECK(Time::isStarted());
  ResourceManager::startUp();
  CHECK(ResourceManager::isStarted());
}

#define CLASSNAME(x) #x

TEST_CASE("[being] Creation of beings") {
  CHECK(BBeing::getClassName() == CLASSNAME(BBeing));
  CHECK(GameMode::getClassName() == CLASSNAME(GameMode));

  SPtr<BBeing> b1 = ClassRegister::createBeing("BBeing");
  SPtr<BBeing> b2 = ClassRegister::createBeing<BBeing>();
  b1->setName("b1");
  b1->createComponent<CTransform>();
}

TEST_CASE("[resource] Creation of textures") {

  Path workingPath = FileSystem::getWorkingDirectory();
  File testImage = FileSystem::open(workingPath.fullPath() + "/../resources/gizmo.png");
  if (testImage.isFile() && testImage.exists()) {
    std::cout << testImage.path() << std::endl;
    SPtr<RImage> r = ResourceManager::instance().loadFromPath<RImage>(testImage.path());
    CHECK(r->m_metadata.m_width == 32);
    CHECK(r->m_metadata.m_height == 32);
    CHECK(r->m_metadata.m_format == (+IMGEXT::E::PNG)._to_integral());
  }

}

TEST_CASE("[resource] Creation of models") {
  Path workingPath = FileSystem::getWorkingDirectory();
  File testModel = FileSystem::open(workingPath.fullPath() + "/../resources/cube.fbx");
  if (testModel.isFile() && testModel.exists()) {
    std::cout << testModel.path() << std::endl;
    SPtr<RModel> r = ResourceManager::instance().loadFromPath<RModel>(testModel.path());
    CHECK(r->m_meshes.size() == 1);
    CHECK((r->m_meshes[0])->m_vertexBuffer.size() == 24);
    CHECK((r->m_meshes[0])->m_indexBuffer.size() == 36);
  }
}

TEST_CASE("[resources] Creation of shaders") {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  File shaderVSF = FileSystem::open(resourceDir.fullPath() + "/vertexShader.hlsl");
  SPtr<RShader> vsShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderVSF.path());
  CHECK(!vsShaderR->m_data.empty());
  File shaderPSF = FileSystem::open(resourceDir.fullPath() + "/pixelShader.hlsl");
  CHECK(!vsShaderR->m_data.empty());
  SPtr<RShader> psShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderPSF.path());

}



TEST_CASE("[window] Window creation") {
  WindowManager::startUp();

  WindowManager::instance().init();

  WindowManager::instance().createWindow("CrossWindow Window", Vector2i(1280, 720), 0);

  CHECK(WindowManager::instance().m_windows.size() > 0);

  int quit = 0;
  Color clearColor = Color::MISSING;
  // Main loop
  bool running = true;
  float timer = 0.0f;

  SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  while (running) {
    eventQueue->update();
    Time::instance().update();
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
        break;
      default:
        // Do nothing
        break;
      }
    }
    timer += Time::instance().deltaTime();
    if (timer > 30.0f) {
      running = false;
    }

  }
}


TEST_SUITE("[components] Components") {
  TEST_CASE("[components] Camera") {
    CTransform t;
    t.reset();
  }
}