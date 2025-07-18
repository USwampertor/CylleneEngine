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
#include "cyInput.h"


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

using Window         = SDL_Window; 
// using SDLWindowEvent = SDL_WindowEvent;
using WindowRenderer = SDL_Renderer;
using SDLEvent       = SDL_Event; 
using WinMsgDesc     = SDL_MessageBoxData; 

struct WindowEvent;

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
              // Application events
              eQUIT = 0x100,
              eTERMINATING,
              eLOW_MEMORY,
              eWILL_ENTER_BACKGROUND,
              eDID_ENTER_BACKGROUND,
              eWILL_ENTER_FOREGROUND,
              eDID_ENTER_FOREGROUND,
              eLOCALE_CHANGED,
              eSYSTEM_THEME_CHANGED,

              // Display events
              eDISPLAY_ORIENTATION = 0x151,
              eDISPLAY_ADDED,
              eDISPLAY_REMOVED,
              eDISPLAY_MOVED,
              eDISPLAY_DESKTOP_MODE_CHANGED,
              eDISPLAY_CURRENT_MODE_CHANGED,
              eDISPLAY_CONTENT_SCALE_CHANGED,

              // Window events
              eWINDOW_SHOWN = 0x202,
              eWINDOW_HIDDEN,
              eWINDOW_EXPOSED,
              eWINDOW_MOVED,
              eWINDOW_RESIZED,
              eWINDOW_PIXEL_SIZE_CHANGED,
              eWINDOW_METAL_VIEW_RESIZED,
              eWINDOW_MINIMIZED,
              eWINDOW_MAXIMIZED,
              eWINDOW_RESTORED,
              eWINDOW_MOUSE_ENTER,
              eWINDOW_MOUSE_LEAVE,
              eWINDOW_FOCUS_GAINED,
              eWINDOW_FOCUS_LOST,
              eWINDOW_CLOSE_REQUESTED,
              eWINDOW_HIT_TEST,
              eWINDOW_ICCPROF_CHANGED,
              eWINDOW_DISPLAY_CHANGED,
              eWINDOW_DISPLAY_SCALE_CHANGED,
              eWINDOW_SAFE_AREA_CHANGED,
              eWINDOW_OCCLUDED,
              eWINDOW_ENTER_FULLSCREEN,
              eWINDOW_LEAVE_FULLSCREEN,
              eWINDOW_DESTROYED,
              eWINDOW_HDR_STATE_CHANGED,

              // Keyboard events
              eKEY_DOWN = 0x300,
              eKEY_UP,
              eTEXT_EDITING,
              eTEXT_INPUT,
              eKEYMAP_CHANGED,
              eKEYBOARD_ADDED,
              eKEYBOARD_REMOVED,
              eTEXT_EDITING_CANDIDATES,

              // Mouse events
              eMOUSE_MOTION = 0x400,
              eMOUSE_BUTTON_DOWN,
              eMOUSE_BUTTON_UP,
              eMOUSE_WHEEL,
              eMOUSE_ADDED,
              eMOUSE_REMOVED,

              // Joystick events
              eJOYSTICK_AXIS_MOTION = 0x600,
              eJOYSTICK_BALL_MOTION,
              eJOYSTICK_HAT_MOTION,
              eJOYSTICK_BUTTON_DOWN,
              eJOYSTICK_BUTTON_UP,
              eJOYSTICK_ADDED,
              eJOYSTICK_REMOVED,
              eJOYSTICK_BATTERY_UPDATED,
              eJOYSTICK_UPDATE_COMPLETE,

              // Gamepad events
              eGAMEPAD_AXIS_MOTION = 0x650,
              eGAMEPAD_BUTTON_DOWN,
              eGAMEPAD_BUTTON_UP,
              eGAMEPAD_ADDED,
              eGAMEPAD_REMOVED,
              eGAMEPAD_REMAPPED,
              eGAMEPAD_TOUCHPAD_DOWN,
              eGAMEPAD_TOUCHPAD_MOTION,
              eGAMEPAD_TOUCHPAD_UP,
              eGAMEPAD_SENSOR_UPDATE,
              eGAMEPAD_UPDATE_COMPLETE,
              eGAMEPAD_STEAM_HANDLE_UPDATED,

              // Touch events
              eFINGER_DOWN = 0x700,
              eFINGER_UP,
              eFINGER_MOTION,
              eFINGER_CANCELED,

              // Clipboard events
              CLIPBOARD_UPDATE = 0x900,

              // Drag and drop events
              eDROP_FILE = 0x1000,
              eDROP_TEXT,
              eDROP_BEGIN,
              eDROP_COMPLETE,
              eDROP_POSITION,

              // Audio events
              eAUDIO_DEVICE_ADDED = 0x1100,
              eAUDIO_DEVICE_REMOVED,
              eAUDIO_DEVICE_FORMAT_CHANGED,

              // Sensor events
              eSENSOR_UPDATE = 0x1200,

              // Pen events
              ePEN_PROXIMITY_IN = 0x1300,
              ePEN_PROXIMITY_OUT,
              ePEN_DOWN,
              ePEN_UP,
              ePEN_BUTTON_DOWN,
              ePEN_BUTTON_UP,
              ePEN_MOTION,
              ePEN_AXIS,

              // Camera events
              eCAMERA_DEVICE_ADDED = 0x1400,
              eCAMERA_DEVICE_REMOVED,
              eCAMERA_DEVICE_APPROVED,
              eCAMERA_DEVICE_DENIED,

              // Render events
              eRENDER_TARGETS_RESET = 0x2000,
              eRENDER_DEVICE_RESET,
              eRENDER_DEVICE_LOST,

              // Private/platform-specific events
              ePRIVATE0 = 0x4000,
              ePRIVATE1,
              ePRIVATE2,
              ePRIVATE3,

              // Internal events
              ePOLL_SENTINEL = 0x7F00,

              // User-defined events
              eUSER = 0x8000);
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
              eDEFAULT_WIN         = eRESIZABLE);
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
    int buttonId = 0;

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
  Event<void, WPtr<WindowEvent>> m_windowEvent;
};
}