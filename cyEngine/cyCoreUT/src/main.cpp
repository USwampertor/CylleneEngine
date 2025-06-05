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
#include <cySceneNode.h>
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
    SPtr<CMeshRenderer> model = b->createComponent<CMeshRenderer>();
    model->setModel(r);
    std::cout << r->getName() << std::endl;
    for (int i = 0; i < r->m_meshes.size(); ++i) {
      std::cout << r->m_meshes[i]->getName() << std::endl;

    }
    CHECK(b->getTransform()->getChildren().size() == r->m_meshes.size());
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

    SceneManager::instance().createScene("TestScene");
    SceneManager::instance().changeScene("TestScene");

    SUBCASE("Object creation") {
      auto obj = SceneManager::instance().createObject<BBeing>("TestObject");
      CHECK(obj != nullptr);
      CHECK(obj->getName() == "TestObject");
      CHECK(SceneManager::instance().findObject<BBeing>("TestObject") == obj);
    }

    SUBCASE("Object destruction") {
      auto obj = SceneManager::instance().createObject<BBeing>("ToDelete");
      // CHECK_FALSE(obj->m_markedToDestroy);
      CHECK(SceneManager::instance().findObject<BBeing>("ToDelete") != nullptr);

      SceneManager::instance().destroyObject(obj);
      // CHECK(obj->m_markedToDestroy);

      // Should be removed in next update
      SceneManager::instance().update(0.0f);
      CHECK(SceneManager::instance().findObject<BBeing>("ToDelete") == nullptr);
    }
  }

  TEST_CASE("Transform Component") {
    SceneManager::instance().createScene("TestScene2");
    SceneManager::instance().changeScene("TestScene2");

    auto parent = SceneManager::instance().createObject<BBeing>("Parent");
    auto child = SceneManager::instance().createObject<BBeing>("Child");

    auto parentTransform = parent->createComponent<CTransform>();
    auto childTransform = child->createComponent<CTransform>();

    SUBCASE("Basic Transform Operations") {
      parentTransform->setLocalPosition(Vector3f(1, 2, 3));
      Vector3f localPos = parentTransform->getLocalPosition();
      CHECK(localPos == Vector3f(1, 2, 3));

      parentTransform->setLocalScale(Vector3f(2, 2, 2));
      CHECK(parentTransform->getLocalScale() == Vector3f(2, 2, 2));

      Quaternion rot(Euler(0, 45, 0));
      parentTransform->setLocalRotation(rot);
      // CHECK(parentTransform->getLocalRotation() == rot);
    }

    SUBCASE("Parent-Child Relationships") {
      // Set up hierarchy
      parent->addChild(child);
      childTransform->setParent(parentTransform);

      CHECK(childTransform->getParent().lock() == parentTransform);
      CHECK(parentTransform->getChildCount() == 1);

      SUBCASE("World Position Inheritance") {
        parentTransform->setLocalPosition(Vector3f(10, 0, 0));
        childTransform->setLocalPosition(Vector3f(5, 0, 0));

        CHECK(childTransform->getWorldPosition() == Vector3f(15, 0, 0));

        // Move parent - child should move with it
        parentTransform->setLocalPosition(Vector3f(20, 0, 0));
        CHECK(childTransform->getWorldPosition() == Vector3f(25, 0, 0));
      }

      SUBCASE("World Scale Inheritance") {
        parentTransform->setLocalScale(Vector3f(2, 2, 2));
        childTransform->setLocalScale(Vector3f(0.5, 0.5, 0.5));

        CHECK(childTransform->getWorldScale() == Vector3f(1, 1, 1));
      }

      SUBCASE("World Rotation Inheritance") {
        Quaternion parentRot(Euler(0, 90, 0));
        Quaternion childRot(Euler(0, 45, 0));

        parentTransform->setLocalRotation(parentRot);
        childTransform->setLocalRotation(childRot);

        // Combined rotation should be ~135 degrees
        Quaternion expectedRot = parentRot * childRot;
        // CHECK(childTransform->getWorldRotation() == expectedRot);
      }
    }

    SUBCASE("Transform Hierarchy Operations") {
      auto grandchild = SceneManager::instance().createObject<BBeing>("Grandchild");
      auto grandchildTransform = grandchild->createComponent<CTransform>();

      // Build hierarchy
      parent->addChild(child);
      child->addChild(grandchild);
      childTransform->setParent(parentTransform);
      grandchildTransform->setParent(childTransform);

      CHECK(parentTransform->isParentOf(childTransform));
      CHECK(childTransform->isChildOf(parentTransform));
      CHECK(grandchildTransform->isChildOf(parentTransform));

      // Test find operations
      CHECK(parentTransform->findChild("Child") == childTransform);
      CHECK(parentTransform->findChild("Grandchild", true) == grandchildTransform);

      // Test removal
      parentTransform->removeChildren("Child");
      CHECK(parentTransform->getChildCount() == 0);
      CHECK(childTransform->getParent().expired());
    }

    SUBCASE("LookAt Functionality") {
      parentTransform->setLocalPosition(Vector3f(0, 0, 0));
      childTransform->setLocalPosition(Vector3f(0, 0, 5));

      parentTransform->setLookAt(childTransform->getWorldPosition());

      // Forward vector should point toward child
      Vector3f forward = parentTransform->getWorldTransform().getForwardVector();
      Vector3f expectedDirection = (childTransform->getWorldPosition() - parentTransform->getWorldPosition()).normalized();

      CHECK(Vector3f::dot(forward, expectedDirection) == doctest::Approx(1.0f).epsilon(0.001f));
    }
  }

  TEST_CASE("Component Querying") {
    SceneManager manager;
    manager.createScene("TestScene");
    manager.changeScene("TestScene");

    // Create test objects with different components
    auto cameraObj = manager.createObject<BBeing>("Camera");
    cameraObj->createComponent<CCamera>();

    auto lightObj = manager.createObject<BBeing>("Light");
    lightObj->createComponent<CLight>();

    auto emptyObj = manager.createObject<BBeing>("Empty");

    SUBCASE("Find by component type") {
      auto cameras = manager.findBBeingsWithComponent<CCamera>();
      CHECK(cameras.size() == 1);
      CHECK(cameras[0]->getName() == "Camera");

      auto lights = manager.findBBeingsWithComponent<CLight>();
      CHECK(lights.size() == 1);
      CHECK(lights[0]->getName() == "Light");

      CHECK(manager.findFirstBBeingWithComponent<CCamera>() == cameraObj);
      CHECK(manager.findFirstBBeingWithComponent<CParticleEmitter>() == nullptr);
    }
  }
}