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

#define SDL_MAIN_USE_CALLBACKS 0
#include <SDL3/SDL.h>

#ifdef CreateWindow
# undef CreateWindow
#endif // CreateWindow


namespace CYLLENE_SDK {

using Window          = SDL_Window;
using WindowEvent     = SDL_WindowEvent;
using MessageBoxData  = SDL_MessageBoxData;
using WindowRenderer  = SDL_Renderer;
using SDLEvent        = SDL_Event;


  namespace WINDOW_INIT
  {
    BETTER_ENUM(E, uint32, 
                eAUDIO    = SDL_INIT_AUDIO,
                eVIDEO    = SDL_INIT_VIDEO,
                eJOYSTICK = SDL_INIT_JOYSTICK,
                eHAPTIC   = SDL_INIT_HAPTIC,
                eGAMEPAD  = SDL_INIT_GAMEPAD,
                eEVENTS   = SDL_INIT_EVENTS,
                eSENSOR   = SDL_INIT_SENSOR,
                eCAMERA   = SDL_INIT_CAMERA);
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

  SPtr<Window> 
  createWindow(const String& title,
               const int32& width, 
               const int32& height, 
               const int32& flags);

  SPtr<Window> 
  createWindow(const String& title,
               const Vector2i& size, 
               const int32& flags);

  SPtr<Window>
  createWindow(const WindowSettings& settings);

  SPtr<Window>
  getWindow(const int32& window);

  void
  destroyWindow(const int32& window);

  SPtr<WindowRenderer>
  createRenderer();

  void*
  getWindowHandle(const int32& window);

  void*
  getWindowProperty(const int32& window, 
                    const String& property);

  const int32
  getWindowID(SPtr<Window> wndw);

  void
  finish();

  bool
  pollEvent(SPtr<SDLEvent> event);

  Vector<SPtr<Window>> m_windows;

  static int32
  ShowWarningMessage( const String& title, const String& message) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title.c_str(), message.c_str(), nullptr);
  }

  static int32
  ShowMessageBox(const SPtr<MessageBoxData> data, int32* id) {
    return 0;
  }

  static int32
  ShowErrorMessage( const String& title, const String& message) {
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
  }
};
}