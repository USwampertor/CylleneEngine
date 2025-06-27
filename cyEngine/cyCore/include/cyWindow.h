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
#include <cyColor.h>


// #ifdef CY_PLATFORM == CY_PLATFORM_WIN32
// # define XWIN_WIN32
// #endif
// 
// #include <CrossWindow/CrossWindow.h>
// #include <CrossWindow/Common/DialogsDesc.h>
// #include <CrossWindow/Common/Dialogs.h>

// 
#define SDL_MAIN_USE_CALLBACKS 0
#include <SDL3/SDL.h>


#ifdef CreateWindow
# undef CreateWindow
#endif // CreateWindow

#include <better-enums/enum.h>

namespace CYLLENE_SDK {

using Window            = SDL_Window; 
using SDLWindowEvent    = SDL_WindowEvent;
using WindowRenderer    = SDL_Renderer;
using SDLEvent          = SDL_Event; 
using WinMsgDesc        = SDL_MessageBoxData; 

namespace SDLINITFLAGS
{
  BETTER_ENUM(E, uint32, 
              eAUDIO      = 0x00000010u, 
              eVIDEO      = 0x00000020u, 
              eJOYSTICK   = 0x00000200u, 
              eHAPTIC     = 0x00001000u, 
              eGAMEPAD    = 0x00002000u, 
              eEVENTS     = 0x00004000u, 
              eSENSOR     = 0x00008000u, 
              eCAMERA     = 0x00010000u);
}

namespace MESSAGEBOXFLAGS
{
  BETTER_ENUM(E, uint32, 
              eERROR        = 0x00000010u, 
              eWARNING      = 0x00000020u, 
              eINFORMATION  = 0x00000040u, 
              eLEFTTORIGHT  = 0x00000080u, 
              eRIGHTTOLEFT  = 0x00000100u);
}

namespace MESSAGEBOXBUTTONFLAGS
{
  BETTER_ENUM(E, uint32,
              eRETURN = 0x00000001u, 
              eESCAPE = 0x00000002u);
}

namespace MESSAGEBOXCOLORTYPE
{
  BETTER_ENUM(E, uint32,
              eBACKGROUND, 
              eTEXT, 
              eBUTTONBORDER, 
              eBUTTONBACKGROUND, 
              eBUTTONSELECTED);
}

namespace WINDOW_PROPERTY
{
  BETTER_ENUM(E, uint32,
              eHWND);
}

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

struct WindowDesc
{
  WindowDesc(const String& newTitle, const Vector2i& newSize, const uint32& newFlags)
    : title(newTitle), size(newSize), flags(newFlags) {
  }
  WindowDesc(const String& newTitle, const int32& width, const int32& height, const uint32& newFlags)
    : title(newTitle), size(Vector2i(width, height)), flags(newFlags) {
  }
  String title;
  Vector2i size;
  Vector2i position;
  uint32 flags;
};

struct WindowEvent {

  WindowEvent(const EVENTTYPE::E& newType, uint32 index)
    : type(newType) {
  }

  WindowEvent(const WindowEvent& other, uint32 index)
    : type(other.type) {}

  EVENTTYPE::E type;
};

struct MessageBoxButtonData
{
  MESSAGEBOXBUTTONFLAGS::E flags;
  int32 id;
  String text;
};

struct MessageBoxColorScheme
{
  Color colors[MESSAGEBOXCOLORTYPE::E::_size()];
};

struct MessageBoxData
{
  String title;
  String message;
  String buttonText;
  int32  flags;
  uint32 parentIndex;
  Vector<MessageBoxButtonData> buttons;
  SPtr<MessageBoxColorScheme> colorScheme;
};


namespace WINDOW_FLAGS
{
  BETTER_ENUM(E, uint64,
              eFULLSCREEN          = 0x0000000000000001,
              // eOPENGL              = 0x0000000000000002,
              eOCCLUDED            = 0x0000000000000004,
              eHIDDEN              = 0x0000000000000008,
              eBORDERLESS          = 0x0000000000000010,
              eRESIZABLE           = 0x0000000000000020,
              eMINIMIZED           = 0x0000000000000040,
              eMAXIMIZED           = 0x0000000000000080,
              eMOUSE_GRABBED       = 0x0000000000000100,
              eINPUT_FOCUS         = 0x0000000000000200,
              eMOUSE_FOCUS         = 0x0000000000000400,
              eEXTERNAL            = 0x0000000000000800,
              eMODAL               = 0x0000000000001000,
              eHIGH_PIXEL_DENSITY  = 0x0000000000002000,
              eMOUSE_CAPTURE       = 0x0000000000004000,
              eMOUSE_RELATIVE_MODE = 0x0000000000008000,
              eALWAYS_ON_TOP       = 0x0000000000010000,
              eUTILITY             = 0x0000000000020000,
              eTOOLTIP             = 0x0000000000040000,
              ePOPUP_MENU          = 0x0000000000080000,
              eKEYBOARD_GRABBED    = 0x0000000000100000,
              // eVULKAN              = 0x0000000010000000,
              // eMETAL               = 0x0000000020000000,
              eTRANSPARENT         = 0x0000000040000000,
              eNOT_FOCUSABLE       = 0x0000000080000000,
              eCENTERED            = 0x0000000100000000,
              eDEFAULT_WIN     = eCENTERED    |
                                 eRESIZABLE   |
                                 eINPUT_FOCUS );
                                 // eMOVABLE |
                                 // eCLOSABLE |
                                 // eMINIMIZABLE |
                                 // eMAXIMIZABLE |
                                 // eCAN_FULLSCREEN |
                                 // eFRAME |
                                 // eSHADOW |
                                 // eVISIBLE);
}


struct CY_CORE_EXPORT WindowSettings
{
  String    name;
  String    title;
  Vector2i  position;
  Vector2i  size;
  uint32    flags = WINDOW_FLAGS::E::eDEFAULT_WIN;
};

class CY_CORE_EXPORT WindowManager : public Module<WindowManager>
{
public:

  WindowManager() = default;

  ~WindowManager() = default;


  bool 
  init();

  WPtr<Window>
  createWindow(const String& title,
               const int32& width, 
               const int32& height, 
               const int32& flags = WINDOW_FLAGS::E::eDEFAULT_WIN);

  WPtr<Window>
  createWindow(const String& title,
               const Vector2i& size, 
               const int32& flags = WINDOW_FLAGS::E::eDEFAULT_WIN);

  WPtr<Window>
  createWindow(const WindowSettings& settings);

  WPtr<Window>
  getWindow(const uint32& window);

  void
  destroyWindow(const uint32& window);

  void*
  getWindowHandle(const uint32& window);

  void*
  getWindowProperty(const uint32& window, 
                    const WINDOW_PROPERTY::E& property);

  const int32
  getWindowID(WPtr<Window> wndw);

  void
  finish();

  void
  update();

  bool
  pollEvent(SPtr<WindowEvent> event);


  EVENTTYPE::E
  getLastEventType(const uint32& window);

  static int
  showMessageBox(const MessageBoxData data, int32 id) {

    // WinMsgDesc desc;
    // desc.title = data->title;
    // desc.content = data->message;
    // desc.buttons = WinMsgButtons::OK;
    // desc.icon = WinMsgIcon::Information;
    // desc.defaultButton = WinMsgDefaultButton::Button1;
    // 
    // return xwin::showMessageBox(desc);

    Vector<SDL_MessageBoxButtonData> buttons;

    for (const auto& button : data.buttons) {
      SDL_MessageBoxButtonData btn = {
        button.flags,
        button.id,
        button.text.c_str()
      };
      buttons.push_back(btn);
    }

    SDL_MessageBoxColorScheme* colorScheme = nullptr;
    if (data.colorScheme != nullptr) {
      colorScheme = new SDL_MessageBoxColorScheme();
      for (int i = 0; i < MESSAGEBOXCOLORTYPE::E::_size(); ++i) {
        colorScheme->colors[i].r = data.colorScheme->colors[i].r * 255;
        colorScheme->colors[i].g = data.colorScheme->colors[i].g * 255;
        colorScheme->colors[i].b = data.colorScheme->colors[i].b * 255;
      }
    }

    int buttonId = 0;
    SDL_MessageBoxData msgBoxData = { 
      data.flags, 
      (id > 0 ) ? WindowManager::instance().m_windows[data.parentIndex].get(): nullptr, 
      data.title.c_str(),
      data.message.c_str(),
      buttons.size(),
      buttons.data(),
      colorScheme};
    if (SDL_ShowMessageBox(&msgBoxData, &buttonId) != 0) {
      // Error
      return -1;
    }
    return buttonId;
  }

  static bool
  showSimpleMessageBox(MESSAGEBOXFLAGS::E flags, 
                       const String& title, 
                       const String& message, 
                       SPtr<Window> parent = nullptr) {
    return SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), parent.get());
  }

  static bool
  showWarningMessage( const String& title, const String& message) {
    return WindowManager::showSimpleMessageBox(MESSAGEBOXFLAGS::E::eWARNING, title, message);

    // WinMsgDesc desc;
    // desc.title = title;
    // desc.content = message;
    // desc.buttons = WinMsgButtons::OK;
    // desc.icon = WinMsgIcon::Warning;
    // desc.defaultButton = WinMsgDefaultButton::Button1;
    // 
    // return WinMsgRspns::Cancel;
  }

  static bool
  showErrorMessage(const String& title, const String& message, uint32 index) {
    return WindowManager::showSimpleMessageBox(MESSAGEBOXFLAGS::E::eERROR, title, message);
    // WinMsgDesc desc;
    // desc.title = title;
    // desc.content = message;
    // desc.buttons = WinMsgButtons::OK;
    // desc.icon = WinMsgIcon::Error;
    // desc.defaultButton = WinMsgDefaultButton::Button1;
    // return WinMsgRspns::Cancel;
  }

public:

  Vector<SPtr<Window>> m_windows;
  Vector<EVENTTYPE::E> m_lastEvents;
  Event<void, SPtr<WindowEvent>> m_windowEvent;
};
}