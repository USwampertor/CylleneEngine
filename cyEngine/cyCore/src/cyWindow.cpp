#include "cyWindow.h"

namespace CYLLENE_SDK {
  
  bool 
  WindowManager::init() {

    SDL_Init(SDL_INIT_VIDEO);
    return true;
  }

  SharedPointer<Window> 
  WindowManager::createWindow(const String& title,
                              const int32& posX, 
                              const int32& posY, 
                              const int32& width, 
                              const int32& height, 
                              const int32& flags) {
    SharedPointer<Window> newWindow = std::make_shared<Window>(SDL_CreateWindow(title.c_str(),
                                                     posX, 
                                                     posY, 
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

  SharedPointer<Window> 
  WindowManager::createWindow(const String& title,
                              const Vector2i& pos, 
                              const Vector2i& size, 
                              const int32& flags) {
    SharedPointer<Window> newWindow = std::make_shared<Window>(SDL_CreateWindow(title.c_str(),
                                                     pos.x, 
                                                     pos.y, 
                                                     size.x, 
                                                     size.y, 
                                                     flags));
    if (newWindow == nullptr) {
      CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
    }
    else {
      m_windows.push_back(newWindow);
    }
    return newWindow;
  }

  SharedPointer<Window>
  WindowManager::createWindow(const WindowSettings& settings) {
    SharedPointer<Window> newWindow = std::make_shared<Window>(SDL_CreateWindow(settings.title.c_str(), 
                                                     settings.position.x, 
                                                     settings.position.y, 
                                                     settings.size.x, 
                                                     settings.size.y, 
                                                     settings.flags));
    if (newWindow == nullptr) {
      CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
    }
    else {
      m_windows.push_back(newWindow);
    }
    return newWindow;
  }
  
  SharedPointer<Window>
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