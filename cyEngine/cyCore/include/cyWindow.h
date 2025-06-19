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
#include <cyEvent.h>


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
#ifdef MOUSEINPUT
# undef MOUSEINPUT
#endif // MOUSEINPUT

#include <better-enums/enum.h>

namespace CYLLENE_SDK {

using Window              = xwin::Window;
using WindowDesc          = xwin::WindowDesc;
using WEventQueue         = xwin::EventQueue;
// using WindowEvent         = xwin::Event;
using WinMsgDesc          = xwin::MessageDesc;
using WinMsgRspns         = xwin::MessageResponse;
using WinMsgButtons       = xwin::MessageButtons;
using WinMsgIcon          = xwin::MessageIcon;
using WinMsgDefaultButton = xwin::MessageDefaultButton;
using WinMsgResponse      = xwin::MessageResponse;
using WinEventData        = xwin::EventData;


namespace EVENTTYPE
{
  BETTER_ENUM(E, uint32, 
              eNONE = 0,
              eCLOSE,
              eCREATE,
              eFOCUS,
              ePAINT,
              eRESIZE,
              eDPI,
              eKEYBOARD,
              eMOUSEMOVE,
              eMOUSERAW,
              eMOUSEWHEEL,
              eMOUSEINPUT,
              eTOUCH,
              eGAMEPAD,
              eDROPFILE,
              eHOVERFILE);
}

struct WindowEvent
{
  WindowEvent(const EVENTTYPE::E& newType, const WinEventData& newData, uint32 index)
    : type(newType), data(newData) {
  }

  WindowEvent(const xwin::Event& newEventData, uint32 index)
    : type(EVENTTYPE::E::_from_integral(static_cast<uint32>(newEventData.type))),
      data(newEventData.data) {}

  WindowEvent(const WindowEvent& other, uint32 index)
    : type(other.type), data(other.data) {}

  EVENTTYPE::E type;
  WinEventData data;
};


struct MessageBoxData
{
  String title;
  String message;
  String buttonText;
  int32  flags;
};

namespace WINDOW_FLAGS
{
  BETTER_ENUM(E, uint32,
              eCENTERED        = 1 << 0,
              eRESIZABLE       = 1 << 1,
              eMOVABLE         = 1 << 2,
              eCLOSABLE        = 1 << 3 ,
              eMINIMIZABLE     = 1 << 4 ,
              eMAXIMIZABLE     = 1 << 5,
              eCAN_FULLSCREEN  = 1 << 5,
              eTRANSPARENT     = 1 << 6,
              eFRAME           = 1 << 7,
              eSHADOW          = 1 << 8,
              eVISIBLE         = 1 << 9,
              eFULLSCREEN      = 1 << 10,
              eMODAL           = 1 << 11,
              eDEFAULT_WIN     = eCENTERED |
                                 eRESIZABLE |
                                 eMOVABLE |
                                 eCLOSABLE |
                                 eMINIMIZABLE |
                                 eMAXIMIZABLE |
                                 eCAN_FULLSCREEN |
                                 eFRAME |
                                 eSHADOW |
                                 eVISIBLE);
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
               const int32& flags = WINDOW_FLAGS::E::eDEFAULT_WIN);

  SPtr<WEventQueue>
  createWindow(const String& title,
               const Vector2i& size, 
               const int32& flags = WINDOW_FLAGS::E::eDEFAULT_WIN);

  SPtr<WEventQueue>
  createWindow(const WindowSettings& settings);

  SPtr<Window>
  getWindow(const uint32& window);

  SPtr<WEventQueue>
  getWEventQueue(const uint32& window);


  void
  destroyWindow(const uint32& window);

  void*
  getWindowHandle(const uint32& window);

  const int32
  getWindowID(SPtr<Window> wndw);

  void
  finish();

  void
  update();

  EVENTTYPE::E
  getLastEventType(const uint32& window);

  static WinMsgRspns
  ShowMessageBox(const SPtr<MessageBoxData> data, int32* id) {

    WinMsgDesc desc;
    desc.title = data->title;
    desc.content = data->message;
    desc.buttons = WinMsgButtons::OK;
    desc.icon = WinMsgIcon::Information;
    desc.defaultButton = WinMsgDefaultButton::Button1;

    return xwin::showMessageBox(desc);
  }

  static WinMsgRspns
  ShowWarningMessage( const String& title, const String& message) {
    
    WinMsgDesc desc;
    desc.title = title;
    desc.content = message;
    desc.buttons = WinMsgButtons::OK;
    desc.icon = WinMsgIcon::Warning;
    desc.defaultButton = WinMsgDefaultButton::Button1;
    
    return WinMsgRspns::Cancel;
  }

  static WinMsgRspns
  ShowErrorMessage( const String& title, const String& message) {
    
    WinMsgDesc desc;
    desc.title = title;
    desc.content = message;
    desc.buttons = WinMsgButtons::OK;
    desc.icon = WinMsgIcon::Error;
    desc.defaultButton = WinMsgDefaultButton::Button1;
    
    return WinMsgRspns::Cancel;
  }

public:

  Vector<Tuple<SPtr<Window>, SPtr<WEventQueue>>> m_windows;
  Vector<EVENTTYPE::E> m_lastEvents;
  Event<void, SPtr<WindowEvent>> m_windowEvent;
};
}