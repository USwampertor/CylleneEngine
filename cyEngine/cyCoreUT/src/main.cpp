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

#include <cyBeing.h>
#include <cyCoreUTPrerequisites.h>
#include <cyCrashHandler.h>
#include <cyFileSystem.h>
#include <cyGameMode.h>
#include <cyImage.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMesh.h>
#include <cyModel.h>
#include <cyResourceManager.h>
#include <cyTexture.h>
#include <cyTime.h>
#include <cyTriangle.h>
#include <cyTransform.h>
#include <cyUnitTesting.h>
#include <cyVector2f.h>
#include <cyVertex.h>
#include <cyWindow.h>


using namespace CYLLENE_SDK;


using ManualShader = std::function<Color(float, float)>;



int32
main(int32 argc, char* argv[]) {


  std::cout << argv[0] << std::endl;

  CrashHandler::startUp();
  SmartPointers::startUp();
  Logger::startUp();
  Time::startUp();
  ResourceManager::startUp();
  WindowManager::startUp();

  WindowManager::instance().init();

  WPtr<Window*> window = WindowManager::instance().createWindow("SDL3 Window", Vector2i(1280, 720), SDL_WINDOW_RESIZABLE);

  if (window.expired()) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  // if (!SDL_Init(SDL_INIT_VIDEO)) {
  //   std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
  //   return -1;
  // }

  // Create an SDL3 window with OpenGL support
  // SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1280, 720, SDL_WINDOW_RESIZABLE);
  // if (!window) {
  //   std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
  //   SDL_Quit();
  //   return -1;
  // }

  // Create an SDL3 renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(*window.lock().get(), nullptr);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(*window.lock().get());
    SDL_Quit();
    return -1;
  }

  // Main loop flag
  int quit = 0;
  Color clearColor = Color::MISSING;
  // Main loop
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255 * clearColor.r, 255 * clearColor.g, 255 * clearColor.b, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(*window.lock().get());
  SDL_Quit();

  // std::cout << "Starting Core Unit Test" << std::endl;
  // 
  // doctest::Context context;
  // 
  // context.applyCommandLine(argc, argv);
  // 
  // int32 res = context.run();
  // 
  // if (context.shouldExit()) {
  //   return res;
  // }
  // 
  // context.clearFilters();
  // 
  // return res + EXIT_SUCCESS;

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
  CHECK(Being::getClassName() == CLASSNAME(Being));
  CHECK(GameMode::getClassName() == CLASSNAME(GameMode));

  SPtr<Being> b1 = ClassRegister::createBeing("Being");
  SPtr<Being> b2 = ClassRegister::createBeing<Being>();
  b1->setName("b1");
  b1->createComponent<TransformComponent>();
}

TEST_CASE("[resource] Creation of textures") {

  Path workingPath = FileSystem::getWorkingDirectory();
  File testImage = FileSystem::open(workingPath.fullPath() + "/../resources/gizmo.png");
  if (testImage.isFile() && testImage.exists()) {
    std::cout << testImage.path() << std::endl;
    SPtr<ImageResource> r = ResourceManager::instance().loadFromPath<ImageResource>(testImage.path());
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
    SPtr<ModelResource> r = ResourceManager::instance().loadFromPath<ModelResource>(testModel.path());
    CHECK(r->m_meshes.size() == 1);
    CHECK((r->m_meshes[0])->m_vertexBuffer.size() == 24);
    CHECK((r->m_meshes[0])->m_indexBuffer.size() == 36);
  }
}

