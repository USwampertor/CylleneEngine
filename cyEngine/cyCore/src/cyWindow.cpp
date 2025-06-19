#include "cyWindow.h"
#include "cyLogger.h"

namespace CYLLENE_SDK {
  
bool 
WindowManager::init() {

  return true;
}

void
WindowManager::update() {
  uint32 i = 0;

  // TODO: Maybe this should be managed by thread, and not in a single thread.
  for (auto& pair : m_windows) {
    auto& eventqueue = std::get<1>(pair);
    eventqueue->update();
    if (!eventqueue->empty()) {
      xwin::Event wEvent = eventqueue->front();

      eventqueue->pop();
      
      WindowEvent e(wEvent,i);
      m_windowEvent.invoke(makeSharedPtr<WindowEvent>(e));
      m_lastEvents[i] = e.type;

      if (+EVENTTYPE::E::eCLOSE == e.type) {
        destroyWindow(i);
        break;
      }
    }
    ++i;
  }
}

EVENTTYPE::E
WindowManager::getLastEventType(const uint32& window) {
  CY_ASSERT(window < m_windows.size() && "Invalid window index");
  if (std::get<1>(m_windows[window])->empty()) {
    return EVENTTYPE::E::eNONE;
  }
  return m_lastEvents[window];
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
  settings.centered = flags & WINDOW_FLAGS::E::eCENTERED;
  settings.resizable = flags & WINDOW_FLAGS::E::eRESIZABLE;
  settings.movable = flags & WINDOW_FLAGS::E::eMOVABLE;
  settings.closable = flags & WINDOW_FLAGS::E::eCLOSABLE;
  settings.minimizable = flags & WINDOW_FLAGS::E::eMINIMIZABLE;
  settings.maximizable = flags & WINDOW_FLAGS::E::eMAXIMIZABLE;
  settings.canFullscreen = flags & WINDOW_FLAGS::E::eCAN_FULLSCREEN;
  settings.transparent = flags & WINDOW_FLAGS::E::eTRANSPARENT;
  settings.frame = flags & WINDOW_FLAGS::E::eFRAME;
  settings.hasShadow = flags & WINDOW_FLAGS::E::eSHADOW;
  settings.fullscreen = flags & WINDOW_FLAGS::E::eFULLSCREEN;
  settings.modal = flags & WINDOW_FLAGS::E::eMODAL;

  if (!newWindow->create(settings, *newWindowEvent)) {
    CY_EXCEPT(InvalidStateException, "Window Manager was not able to create a window");
  }
  else {
    m_windows.push_back(std::make_pair(newWindow,newWindowEvent));
    m_lastEvents.push_back(EVENTTYPE::E::eNONE);
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
WindowManager::getWindow(const uint32& window) {
  return std::get<0>(m_windows[window]);
}

SPtr<WEventQueue>
WindowManager::getWEventQueue(const uint32& window) {
  return std::get<1>(m_windows[window]);
}

void
WindowManager::destroyWindow(const uint32& window) {
  auto wndow = std::get<0>(m_windows[window]);
  wndow->close();

  m_windows.erase(m_windows.begin() + window);
  m_lastEvents.erase(m_lastEvents.begin() + window);

}

void*
WindowManager::getWindowHandle(const uint32& window) {
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

void
WindowManager::finish() {
  for (int32 i = 0; i < m_windows.size(); ++i) {
    std::get<0>(m_windows[i]).reset();
  }
  // SDL_Quit();
}

} // namespace CYLLENE_SDK