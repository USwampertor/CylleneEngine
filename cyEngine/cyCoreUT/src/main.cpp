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
#include <cyCTransform.h>
#include <cyCCamera.h> 
#include <cyCLight.h>
#include <cyCMeshRenderer.h>
#include <cyFileSystem.h>
#include <cyGameMode.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyResourceManager.h>
#include <cyRImage.h>
#include <cyRMesh.h>
#include <cyRModel.h>
#include <cyRTexture.h>
#include <cyRShader.h>
#include <cyTime.h>
#include <cyTriangle.h>
#include <cyUnitTesting.h>
#include <cyVector2f.h>
#include <cyVertex.h>
#include <cyWindow.h>
#include <cySceneManager.h>
#include <cyScene.h>
#include <cySNode.h>
#include <cyCParticleEmitter.h>


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
  SceneManager::startUp();
  CHECK(SceneManager::isStarted());
}

#define CLASSNAME(x) #x

TEST_CASE("[being] Creation of beings") {
  CHECK(BBeing::getClassName() == CLASSNAME(BBeing));
  CHECK(GameMode::getClassName() == CLASSNAME(GameMode));

  SPtr<BBeing> b1 = ClassRegister::createBeing("BBeing");
  SPtr<BBeing> b2 = ClassRegister::createBeing<BBeing>();
  b1->setName("b1");
  b1->createComponent<CTransform>();
  b1->createComponent<CCamera>();
  b1->createComponent<CMeshRenderer>();
  b1->createComponent<CLight>();
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
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  File testModel2 = FileSystem::open(resourceDir.fullPath() + "/Mabis.fbx");
  if (testModel2.isFile() && testModel2.exists()) {
    std::cout << testModel2.path() << std::endl;
    SPtr<RModel> r = ResourceManager::instance().loadFromPath<RModel>(testModel2.path());
    SPtr<BBeing> b = ClassRegister::createBeing<BBeing>();
    b->setName("b1");
    b->createComponent<CTransform>();
    WPtr<CMeshRenderer> model = b->createComponent<CMeshRenderer>();
    model.lock()->setModel(r);
    std::cout << r->getName() << std::endl;
    for (int i = 0; i < r->m_meshes.size(); ++i) {
      std::cout << r->m_meshes[i]->getName() << std::endl;

    }
    // CHECK(b->getTransform()->getChildren().size() == r->m_meshes.size());
  }

}

TEST_CASE("[resources] Creation of shaders") {
  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  File shaderVSF = FileSystem::open(resourceDir.fullPath() + "/vertexShader.hlsl");
  SPtr<RShader> vsShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderVSF.path());
  CHECK(String(vsShaderR->getShaderType()._to_string()) == "VERTEX");
  File shaderPSF = FileSystem::open(resourceDir.fullPath() + "/pixelShader.hlsl");
  SPtr<RShader> psShaderR = ResourceManager::instance().loadFromPath<RShader>(shaderPSF.path());
  CHECK(String(psShaderR->getShaderType()._to_string()) == "PIXEL");

  vsShaderR = ResourceManager::instance().loadFromPath<RShader>(Path(resourceDir.fullPath() + "/vShaderExt.vs_hlsl").path());
  CHECK(String(vsShaderR->getShaderType()._to_string()) == "VERTEX");

  CHECK(String(vsShaderR->getName()) == "vShaderExt");
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

TEST_SUITE("Scene System Tests") {

  TEST_CASE("Scene Management") {

    SUBCASE("Scene creation and activation") {
      auto scene = SceneManager::instance().createScene("TestScene");
      CHECK(scene != nullptr);
      CHECK(scene->getName() == "TestScene");

      CHECK(SceneManager::instance().changeScene("TestScene"));
      CHECK(SceneManager::instance().getActiveScene() == scene);
    }

    SUBCASE("Multiple scene handling") {
      SceneManager::instance().createScene("Scene1");
      SceneManager::instance().createScene("Scene2");

      CHECK(SceneManager::instance().changeScene("Scene1"));
      CHECK(SceneManager::instance().getActiveScene()->getName() == "Scene1");

      CHECK(SceneManager::instance().changeScene("Scene2"));
      CHECK(SceneManager::instance().getActiveScene()->getName() == "Scene2");

      CHECK(SceneManager::instance().findScene("Scene1") != nullptr);
      CHECK(SceneManager::instance().findScene("NonExistent") == nullptr);
    }
  }

  TEST_CASE("GameObject Management") {

    // SceneManager::instance().createScene("TestScene");
    // SceneManager::instance().changeScene("TestScene");
    // 
    // SUBCASE("Object creation") {
    //   auto obj = SceneManager::instance().createBeing<BBeing>("TestObject");
    //   CHECK(obj != nullptr);
    //   CHECK(obj->getName() == "TestObject");
    //   CHECK(SceneManager::instance().findBeing<BBeing>("TestObject") == obj);
    // }
    // 
    // SUBCASE("Object destruction") {
    //   auto obj = SceneManager::instance().createBeing<BBeing>("ToDelete");
    //   // CHECK_FALSE(obj->m_markedToDestroy);
    //   CHECK(SceneManager::instance().findBeing<BBeing>("ToDelete") != nullptr);
    // 
    //   SceneManager::instance().destroyBeing(obj);
    //   // CHECK(obj->m_markedToDestroy);
    // 
    //   // Should be removed in next update
    //   SceneManager::instance().update(0.0f);
    //   CHECK(SceneManager::instance().findBeing<BBeing>("ToDelete") == nullptr);
    // }
  }

  TEST_CASE("GameObject Parenting") {
    WPtr<BBeing> parent = SceneManager::instance().createBeing<BBeing>("Parent");
    parent.lock()->createComponent<CTransform>();

    WPtr<BBeing> child1 = parent.lock()->createChild("Child1");
    CHECK(parent.lock()->getChildCount() == 1);
    
    WPtr<BBeing> child2 = SceneManager::instance().createBeing<BBeing>("Child2");
    child2.lock()->createComponent<CTransform>(Vector3f::ZERO, Vector3f::ONE, Quaternion::IDENTITY);
    parent.lock()->addChild(child2);

    CHECK(parent.lock()->getChildCount() == 2);
    
    WPtr<BBeing> grandparent = SceneManager::instance().createBeing<BBeing>("Grandparent");
    grandparent.lock()->createComponent<CTransform>();
    
    parent.lock()->setParent(grandparent);
    CHECK(grandparent.lock()->getChildCount() == 1);
    CHECK(parent.lock()->isChildOf(grandparent));

    for (WPtr<BBeing>& being : SceneManager::instance().getActiveScene()->getAllBeings()) {
      std::cout << "Being: " << being.lock()->getName() << std::endl;
    }

    CHECK(SceneManager::instance().getActiveScene()->getAllBeings().size() == 4);

  }

}