#include <cyUnitTesting.h>

#include <cyGraphicsDX11API.h>
#include <cyDLLLoader.h>
#include <cyGraphicsAPI.h>
#include <cyWindow.h> 
#include <cyLogger.h> 

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


  WindowManager::startUp();
  WindowManager::instance().init();
  SPtr<Window> window = WindowManager::instance().createWindow("Test", Vector2i(1280, 720), SDL_WINDOW_RESIZABLE);

  void* hwnd = WindowManager::instance().getWindowHandle(0);

  GraphicsDX11API::startUp<GraphicsDX11API>();
  GraphicsDX11API::instance().initialize(hwnd);






  WindowManager::shutDown();

  return 0;

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}


