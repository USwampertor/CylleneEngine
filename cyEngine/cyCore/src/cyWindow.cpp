#include "cyWindow.h"
#include "cyLogger.h"

namespace CYLLENE_SDK {
  
bool 
WindowManager::init() {

  // if (!SDL_Init(WINDOW_INIT::E::eVIDEO)) {
  //   String errorStr = Utils::format("Error initializing SDL: %s", SDL_GetError());
  //   Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
  //   return false;
  // }
  return true;
}

SPtr<WEventQueue>
WindowManager::createWindow(const String& title,
                            const int32& width, 
                            const int32& height, 
                            const int32& flags) {
  
  SPtr<WEventQueue> newWindowEvent = std::make_shared<WEventQueue>();
  SPtr<Window> newWindow = std::make_shared<Window>();
  
  WindowDesc settings;

  settings.title = title;
  settings.width = static_cast<uint32>(width);
  settings.height = static_cast<uint32>(height);
  settings.centered = flags & WINDOW_FLAGS::E::CENTERED;
  settings.resizable = flags & WINDOW_FLAGS::E::RESIZABLE;
  settings.movable = flags & WINDOW_FLAGS::E::MOVABLE;
  settings.closable = flags & WINDOW_FLAGS::E::CLOSABLE;
  settings.minimizable = flags & WINDOW_FLAGS::E::MINIMIZABLE;
  settings.maximizable = flags & WINDOW_FLAGS::E::MAXIMIZABLE;
  settings.canFullscreen = flags & WINDOW_FLAGS::E::CAN_FULLSCREEN;
  settings.transparent = flags & WINDOW_FLAGS::E::TRANSPARENT;
  settings.frame = flags & WINDOW_FLAGS::E::FRAME;
  settings.hasShadow = flags & WINDOW_FLAGS::E::SHADOW;
  settings.fullscreen = flags & WINDOW_FLAGS::E::FULLSCREEN;
  settings.modal = flags & WINDOW_FLAGS::E::MODAL;

  if (!newWindow->create(settings, *newWindowEvent)) {
    CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
  }
  else {
    m_windows.push_back(std::make_pair(newWindow,newWindowEvent));
  }

  return newWindowEvent;
}

SPtr<WEventQueue>
WindowManager::createWindow(const String& title,
                            const Vector2i& size, 
                            const int32& flags) {
  return createWindow(title, size.x, size.y, flags);
}

SPtr<WEventQueue>
WindowManager::createWindow(const WindowSettings& settings) {
  return this->createWindow(settings.title, 
                            settings.size, 
                            settings.flags);
}
  
SPtr<Window>
WindowManager::getWindow(const int32& window) {
  return std::get<0>(m_windows[window]);
}

SPtr<WEventQueue>
WindowManager::getWEventQueue(const int32& window) {
  return std::get<1>(m_windows[window]);
}

void
WindowManager::destroyWindow(const int32& window) {
  auto wndow = std::get<0>(m_windows[window]);
  wndow->close();
}

void*
WindowManager::getWindowHandle(const int32& window) {
  auto wndow = std::get<0>(m_windows[window]);
  
  return wndow->getHwnd();
}

const int32
WindowManager::getWindowID(SPtr<Window> wndw) {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    if (std::get<0>(m_windows[i]) == wndw) { return i; }
  }
  return -1;
}

WindowEvent
WindowManager::pollEvent() {
  return WindowEvent();
}

void
WindowManager::finish() {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    std::get<0>(m_windows[i]).reset();
  }
  // SDL_Quit();
}
}