#include <cyCoreUTPrerequisites.h>
#include <cyWindow.h>
#include <cyVector2f.h>

using namespace CYLLENE_SDK;

int32
main(int32 argc, char* argv[]) {
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

  return 0;
}