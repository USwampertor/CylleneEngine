#include "cyWindow.h"
#include "cyLogger.h"

namespace CYLLENE_SDK {
  
  bool 
  WindowManager::init() {

    if (!SDL_Init(WINDOW_INIT::E::eVIDEO)) {
      String errorStr = Utils::format("Error initializing SDL: %s", SDL_GetError());
      Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
      return false;
    }
    return true;
  }

  SPtr<Window*> 
  WindowManager::createWindow(const String& title,
                              const int32& width, 
                              const int32& height, 
                              const int32& flags) {
    SPtr<Window*> newWindow = 
      std::make_shared<Window*>(SDL_CreateWindow(title.c_str(),
                                                 width, 
                                                 height, 
                                                 flags));
    if (newWindow == nullptr) {
      CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
    }
    else {
      m_windows.push_back(newWindow);
    }
    return newWindow;
  }

  SPtr<Window*> 
  WindowManager::createWindow(const String& title,
                              const Vector2i& size, 
                              const int32& flags) {
    return createWindow(title, size.x, size.y, flags);
  }

  SPtr<Window*>
  WindowManager::createWindow(const WindowSettings& settings) {
    return createWindow(settings.title, 
                        settings.size.x, 
                        settings.size.y, 
                        settings.flags);
  }
  
  SPtr<Window*>
  WindowManager::getWindow(const int32& window) {
    return m_windows[window];
  }


  void
  WindowManager::finish() {
    for (int32 i = 0; i < m_windows.size(); ++i) {
      m_windows[i].reset();
    }
    SDL_Quit();
  }
}