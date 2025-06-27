#include "cyWindow.h"
#include "cyLogger.h"

namespace CYLLENE_SDK {
  
bool 
WindowManager::init() {
  if (!SDL_Init(SDLINITFLAGS::E::eVIDEO ||
                SDLINITFLAGS::E::eEVENTS ||
                SDLINITFLAGS::E::eGAMEPAD ||
                SDLINITFLAGS::E::eJOYSTICK)) {
    String errorStr = Utils::format("Error initializing SDL: %s", SDL_GetError());
    Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
    Utils::throwException(errorStr);
    return false;
  }
  return true;
  // return true;
}

void
WindowManager::update() {
  uint32 i = 0;

  // TODO: Maybe this should be managed by thread, and not in a single thread.
  for (i = 0; i < m_windows.size(); ++i) {
    SDL_Event e;


    // xwin::Event wEvent = eventqueue->front();
    // 
    // eventqueue->pop();
    // 
    // WindowEvent e(wEvent, i);
    // m_windowEvent.invoke(makeSharedPtr<WindowEvent>(e));
    // m_lastEvents[i] = e.type;
    // 
    // if (+EVENTTYPE::E::eCLOSE == e.type) {
    //   destroyWindow(i);
    //   break;
    // }

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_EVENT_QUIT:
        destroyWindow(i);
        break;
      case SDL_EVENT_KEY_DOWN:
        if (e.key.key == SDLK_ESCAPE) {
          destroyWindow(i);
        }
        break;
      }
    }

    // Clear screen (black)
    // SDL_SetRenderDrawColor(m_windows[i].get(), 0, 0, 0, 255);
    // SDL_RenderClear(m_windows[i].get());
    // SDL_RenderPresent(m_windows);
  }
}

EVENTTYPE::E
WindowManager::getLastEventType(const uint32& window) {
  CY_ASSERT(window < m_windows.size() && "Invalid window index");
  if (!m_windows[window]) {
    return EVENTTYPE::E::eNONE;
  }
  return m_lastEvents[window];
}

WPtr<Window>
WindowManager::createWindow(const String& title,
                            const int32& width, 
                            const int32& height, 
                            const int32& flags) {
  WindowSettings settings;
  settings.title = title;
  settings.size = Vector2i(width, height);
  settings.flags = flags;
  return createWindow(settings);

  // SPtr<WEventQueue> newWindowEvent = std::make_shared<WEventQueue>();
  // SPtr<Window> newWindow = std::make_shared<Window>();
  // 
  // WindowDesc settings;
  // 
  // settings.title = title;
  // settings.width = static_cast<uint32>(width);
  // settings.height = static_cast<uint32>(height);
  // settings.centered = flags & WINDOW_FLAGS::E::eCENTERED;
  // settings.resizable = flags & WINDOW_FLAGS::E::eRESIZABLE;
  // settings.movable = flags & WINDOW_FLAGS::E::eMOVABLE;
  // settings.closable = flags & WINDOW_FLAGS::E::eCLOSABLE;
  // settings.minimizable = flags & WINDOW_FLAGS::E::eMINIMIZABLE;
  // settings.maximizable = flags & WINDOW_FLAGS::E::eMAXIMIZABLE;
  // settings.canFullscreen = flags & WINDOW_FLAGS::E::eCAN_FULLSCREEN;
  // settings.transparent = flags & WINDOW_FLAGS::E::eTRANSPARENT;
  // settings.frame = flags & WINDOW_FLAGS::E::eFRAME;
  // settings.hasShadow = flags & WINDOW_FLAGS::E::eSHADOW;
  // settings.fullscreen = flags & WINDOW_FLAGS::E::eFULLSCREEN;
  // settings.modal = flags & WINDOW_FLAGS::E::eMODAL;


  // if (!newWindow->create(settings, *newWindowEvent)) {
  //   CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
  // }
  // else {
  //   m_windows.push_back(std::make_pair(newWindow,newWindowEvent));
  //   m_lastEvents.push_back(EVENTTYPE::E::eNONE);
  // }
  // 
  // return newWindowEvent;
}

WPtr<Window>
WindowManager::createWindow(const String& title,
                            const Vector2i& size, 
                            const int32& flags) {
  return createWindow(title, size.x, size.y, flags);
}

WPtr<Window>
WindowManager::createWindow(const WindowSettings& settings) {

  SPtr<Window> newWindow = makeSharedPtr<Window>(SDL_CreateWindow(settings.title.c_str(), 
                                                                  settings.size.x, 
                                                                  settings.size.y, 
                                                                  settings.flags),
                                                 [](Window* w) { if (w) SDL_DestroyWindow(w); });
  if (!newWindow) {
    Utils::throwException("Was not able to create window");
  }

  return newWindow;
}
  
WPtr<Window>
WindowManager::getWindow(const uint32& window) {
  return m_windows[window];
}

void
WindowManager::destroyWindow(const uint32& window) {
  auto wndow = m_windows[window];
  SDL_DestroyWindow(wndow.get());

  m_windows.erase(m_windows.begin() + window);
  m_lastEvents.erase(m_lastEvents.begin() + window);

}

void*
WindowManager::getWindowHandle(const uint32& window) {
  return getWindowProperty(window, WINDOW_PROPERTY::E::eHWND);
}

void*
WindowManager::getWindowProperty(const uint32& window, const WINDOW_PROPERTY::E& property) {
  auto wndow = m_windows[window];
  String propertyStr = Utils::BLANKSTRING;
  if (+WINDOW_PROPERTY::E::eHWND == property) {
    propertyStr = SDL_PROP_WINDOW_WIN32_HWND_POINTER;
  }
  auto hwnd = SDL_GetPointerProperty(SDL_GetWindowProperties(wndow.get()),
                                     propertyStr.c_str(),
                                     nullptr);
  return hwnd;
}


const int32
WindowManager::getWindowID(WPtr<Window> wndw) {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    if (m_windows[i] == wndw.lock()) { return i; }
  }
  return -1;
}

void
WindowManager::finish() {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    m_windows[i].reset();
  }
  SDL_Quit();
}

} // namespace CYLLENE_SDK