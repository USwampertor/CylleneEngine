/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyWindow.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/04
 * @brief This is a cross platform integration of CrossWindow project
 *        for, as obvious as it could be, cross platform window handling
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"

#include <cyException.h>
#include <cyModule.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

#include <SDL2/SDL.h>

namespace CYLLENE_SDK {

using Window          = SDL_Window;
using WindowEvent     = SDL_WindowEvent;
using MessageBoxData  = SDL_MessageBoxData;


  namespace WINDOW_INIT
  {
    BETTER_ENUM(E, uint32, 
                eTIMER           = SDL_INIT_TIMER,
                eAUDIO           = SDL_INIT_AUDIO,
                eVIDEO           = SDL_INIT_VIDEO,
                eJOYSTICK        = SDL_INIT_JOYSTICK,
                eHAPTIC          = SDL_INIT_HAPTIC,
                eGAMECONTROLLER  = SDL_INIT_GAMECONTROLLER,
                eEVENTS          = SDL_INIT_EVENTS,
                eSENSOR          = SDL_INIT_SENSOR,
                eNOPARACHUTE     = SDL_INIT_NOPARACHUTE,
                eEVERYTHING      = SDL_INIT_EVERYTHING);
  }


struct CY_CORE_EXPORT WindowSettings
{
  String    name;
  String    title;
  Vector2i  position;
  Vector2i  size;
  uint32    flags;
};

class CY_CORE_EXPORT WindowManager : public Module<WindowManager>
{
public:

  WindowManager() = default;

  ~WindowManager() = default;


  bool 
  init();

  SharedPointer<Window*> 
  createWindow(const String& title,
               const int32& posX, 
               const int32& posY, 
               const int32& width, 
               const int32& height, 
               const int32& flags);

  SharedPointer<Window*> 
  createWindow(const String& title,
               const Vector2i& pos, 
               const Vector2i& size, 
               const int32& flags);

  SharedPointer<Window*>
  createWindow(const WindowSettings& settings);

  SharedPointer<Window*>
  getWindow(const int32& window);

  void
  finish();

  Vector<SharedPointer<Window*>> m_windows;

  static int32
  ShowWarningMessage( const String& title, const String& message) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title.c_str(), message.c_str(), nullptr);
  }

  static int32
  ShowMessageBox(const MessageBoxData* data, int32* id) {
    return 0;
  }

  static int32
  ShowErrorMessage( const String& title, const String& message) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
  }
};
}