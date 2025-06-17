
#include <cyUnitTesting.h>

#include <cyGraphicsAPI.h>
#include <cyDLLLoader.h>
#include <cyLogger.h>
#include <cyResourceManager.h>
#include <cySceneManager.h>
#include <cyWindow.h>
#include <cyTime.h>

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
  void* createFunc = DLLLoader::load("cyDirectX11d.dll", "createPluginAPI", false);
  if (!createFunc) {
    // Handle error: DLL not found or function not exported
    return -1;
  }

  // Cast the function pointer to the correct type
  auto factory = reinterpret_cast<GraphicsAPI * (*)()>(createFunc);

  // 2. Create the DX11 API instance
  GraphicsAPI* dx11API = factory();
  if (!dx11API) {
    // Handle error: API creation failed
    return -1;
  }

  GraphicsAPI::setModule(dx11API);

  GraphicsAPI::instance().initialize(hwnd);

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

    DELTA_TYPE::E deltaType = DELTA_TYPE::E::MILLISECOND;
    deltaTime = Time::instance().deltaTime(deltaType);
    time += deltaTime * 0.001f;

    GraphicsAPI::instance().clear(Color::GREEN);
    GraphicsAPI::instance().present();
  }
  // ... rest of your rendering loop ...

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


