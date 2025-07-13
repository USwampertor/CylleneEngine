#include "cyInputManager.h"
#include "cyLogger.h"
#include "cyWindow.h"

namespace CYLLENE_SDK {

bool
InputManager::init() {

  if (SDL_WasInit(SDLINITFLAGS::E::eGAMEPAD) || 
      SDL_WasInit(SDLINITFLAGS::E::eJOYSTICK) || 
      SDL_WasInit(SDLINITFLAGS::E::eHAPTIC)) {
    String errorStr = Utils::format("SDL Video was already initialized: %s", SDL_GetError());
    Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
    return true;
  }

  if (!SDL_InitSubSystem(SDLINITFLAGS::E::eGAMEPAD) ||
      !SDL_InitSubSystem(SDLINITFLAGS::E::eJOYSTICK) ||
      !SDL_InitSubSystem(SDLINITFLAGS::E::eHAPTIC)) {
    String errorStr = Utils::format("Error initializing SDL: %s", SDL_GetError());
    Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
    Utils::throwException(errorStr);
    return false;
  }


  if (!SDL_WasInit(SDLINITFLAGS::E::eEVENTS)) {
    if (!SDL_InitSubSystem(SDLINITFLAGS::E::eEVENTS)) {
      String errorStr = Utils::format("Error initializing SDL Events: %s", SDL_GetError());
      Logger::instance().logError(errorStr, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
      Utils::throwException(errorStr);
      return false;
    }
  }

  return true;

}

Vector2i
InputManager::getWindowMousePosition() {
  Vector2f pos;
  SDL_GetMouseState(&pos.x, &pos.y);
  return Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

Vector2i
InputManager::getScreenMousePosition() {
  Vector2f pos;
  SDL_GetGlobalMouseState(&pos.x, &pos.y);
  return Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

void
InputManager::setWindowMousePosition(const Vector2i& pos, uint32 windowIndex) {
  WPtr<Window> window = WindowManager::instance().getWindow(windowIndex);
  if (window.lock()) {
    SDL_WarpMouseInWindow(window.lock().get(), pos.x, pos.y);
  }

}

void
InputManager::setScreenMousePosition(const Vector2i& pos) {
  SDL_WarpMouseGlobal(pos.x, pos.y);

}




}
