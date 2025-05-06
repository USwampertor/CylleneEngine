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


#ifdef CY_PLATFORM == CY_PLATFORM_WIN32
# define XWIN_WIN32
#endif

#include <CrossWindow/CrossWindow.h>
#include <CrossWindow/Common/DialogsDesc.h>
#include <CrossWindow/Common/Dialogs.h>

// 
// #define SDL_MAIN_USE_CALLBACKS 0
// #include <SDL3/SDL.h>

#ifdef CreateWindow
# undef CreateWindow
#endif // CreateWindow
#ifdef TRANSPARENT
# undef TRANSPARENT
#endif // TRANSPARENT

#include <better-enums/enum.h>

namespace CYLLENE_SDK {

using Window      = xwin::Window;
using WindowDesc  = xwin::WindowDesc;
using WEventQueue = xwin::EventQueue;
using WindowEvent = xwin::Event;
using WinMsgDesc  = xwin::MessageDesc;
using WinMsgRspns = xwin::MessageResponse;

struct MessageBoxData
{
  String title;
  String message;
  String buttonText;
  int32  flags;
};

// using WindowEvent     = SDL_WindowEvent;
// using MessageBoxData  = SDL_MessageBoxData;
// using WindowRenderer  = SDL_Renderer;
// using SDLEvent        = SDL_Event;


  // namespace WINDOW_INIT
  // {
  //   BETTER_ENUM(E, uint32, 
  //               eAUDIO    = SDL_INIT_AUDIO,
  //               eVIDEO    = SDL_INIT_VIDEO,
  //               eJOYSTICK = SDL_INIT_JOYSTICK,
  //               eHAPTIC   = SDL_INIT_HAPTIC,
  //               eGAMEPAD  = SDL_INIT_GAMEPAD,
  //               eEVENTS   = SDL_INIT_EVENTS,
  //               eSENSOR   = SDL_INIT_SENSOR,
  //               eCAMERA   = SDL_INIT_CAMERA);
  // }

namespace WINDOW_FLAGS
{
  BETTER_ENUM(E, uint32,
              CENTERED,
              RESIZABLE,
              MOVABLE,
              CLOSABLE,
              MINIMIZABLE,
              MAXIMIZABLE,
              CAN_FULLSCREEN,
              TRANSPARENT,
              FRAME,
              SHADOW,
              VISIBLE,
              FULLSCREEN,
              MODAL);    
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

  SPtr<WEventQueue>
  createWindow(const String& title,
               const int32& width, 
               const int32& height, 
               const int32& flags);

  SPtr<WEventQueue>
  createWindow(const String& title,
               const Vector2i& size, 
               const int32& flags);

  SPtr<WEventQueue>
  createWindow(const WindowSettings& settings);

  SPtr<Window>
  getWindow(const int32& window);

  SPtr<WEventQueue>
  getWEventQueue(const int32& window);


  void
  destroyWindow(const int32& window);

//   SPtr<WindowRenderer>
//   createRenderer();

  void*
  getWindowHandle(const int32& window);

//   void*
//   getWindowProperty(const int32& window, 
//                     const String& property);

  const int32
  getWindowID(SPtr<Window> wndw);

  void
  finish();

  WindowEvent
  pollEvent();

  static WinMsgRspns
  ShowMessageBox(const SPtr<MessageBoxData> data, int32* id) {

    WinMsgDesc desc;
    desc.title = data->title;
    desc.content = data->message;
    desc.buttons = xwin::MessageButtons::OK;
    desc.icon = xwin::MessageIcon::Information;
    desc.defaultButton = xwin::MessageDefaultButton::Button1;

    return xwin::showMessageBox(desc);
  }

  static WinMsgRspns
  ShowWarningMessage( const String& title, const String& message) {
    
    // xwin::MessageDesc desc;
    // xwin::showMessageBox(desc);
    // return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title.c_str(), message.c_str(), nullptr);
    
    WinMsgDesc desc;
    desc.title = title;
    desc.content = message;
    desc.buttons = xwin::MessageButtons::OK;
    desc.icon = xwin::MessageIcon::Warning;
    desc.defaultButton = xwin::MessageDefaultButton::Button1;
    
    return WinMsgRspns::Cancel;
  }

  static WinMsgRspns
  ShowErrorMessage( const String& title, const String& message) {
    // return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), nullptr);
    
    WinMsgDesc desc;
    desc.title = title;
    desc.content = message;
    desc.buttons = xwin::MessageButtons::OK;
    desc.icon = xwin::MessageIcon::Error;
    desc.defaultButton = xwin::MessageDefaultButton::Button1;
    
    return WinMsgRspns::Cancel;
  }


public:

  Vector<Tuple<SPtr<Window>, SPtr<WEventQueue>>> m_windows;
};
}