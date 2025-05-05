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

SPtr<Window> 
WindowManager::createWindow(const String& title,
                            const int32& width, 
                            const int32& height, 
                            const int32& flags) {
  // SPtr<Window> newWindow(SDL_CreateWindow(title.c_str(), width, height, flags),
  //   [](Window* w) { if (w) SDL_DestroyWindow(w); });
  // 
  // if (!newWindow) {
  //   CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
  // }
  // else {
  //   m_windows.push_back(newWindow);
  // }
  return nullptr;
}

SPtr<Window> 
WindowManager::createWindow(const String& title,
                            const Vector2i& size, 
                            const int32& flags) {
  return createWindow(title, size.x, size.y, flags);
}

SPtr<Window>
WindowManager::createWindow(const WindowSettings& settings) {
  return this->createWindow(settings.title, 
                            settings.size, 
                            settings.flags);
}
  
SPtr<Window>
WindowManager::getWindow(const int32& window) {
  return m_windows[window];
}

void*
WindowManager::getWindowHandle(const int32& window) {
  auto wndow = m_windows[window];
  // auto hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(wndow.get()),
  //                                                             SDL_PROP_WINDOW_WIN32_HWND_POINTER,
  //                                                             nullptr);
  return nullptr;
}

void*
WindowManager::getWindowProperty(const int32& window, const String& property) {
  auto wndow = m_windows[window];
  // auto hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(wndow.get()),
  //                                                             property.c_str(),
  //                                                             nullptr);
  return nullptr;
}

const int32
WindowManager::getWindowID(SPtr<Window> wndw) {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    if (m_windows[i] == wndw) { return i; }
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
    m_windows[i].reset();
  }
  // SDL_Quit();
}
}