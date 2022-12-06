#include <cyCoreUTPrerequisites.h>
#include <cyWindow.h>
#include <cyVector2f.h>
#include <cyLogger.h>
#include <cyCrashHandler.h>
#include <cyTime.h>

using namespace CYLLENE_SDK;

int32
main(int32 argc, char* argv[]) {

  CrashHandler::startUp();
  CrashHandler::instance().init();

  Logger::startUp();
  Logger::instance().init();

  Time::startUp();
  Time::instance().init();

  try {

    WindowManager::startUp();
    WindowManager::instance().init();
    WindowManager::instance().createWindow("Test Window", 
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           1280, 
                                           720, 
                                           SDL_WINDOW_OPENGL);
  
    SDL_Delay(3000);
    WindowManager::instance().finish();
    WindowManager::shutDown();
    CY_EXCEPT(UnitTestException, "This is part of the test");
  }
  catch (const Exception& e) {
    std::cout << e.what();
  }

  return 0;
}