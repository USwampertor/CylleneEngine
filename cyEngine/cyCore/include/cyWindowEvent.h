#pragma once
#include "cyCorePrerequisites.h"
#include "cyWindow.h"

namespace CYLLENE_SDK {




struct WindowEvent {
public:
  WindowEvent(const SDLEvent& newEvent) : type(EVENTTYPE::E::_from_integral(newEvent.type)) {
    timestamp = newEvent.common.timestamp;
    displayID = newEvent.display.displayID;
    windowID = newEvent.window.windowID;
    keyboardID = newEvent.kdevice.which;

    // Check which type of event was
    if (+EVENTTYPE::E::eKEY_DOWN || +EVENTTYPE::E::eKEY_UP) {
      
      m_inputState = (+EVENTTYPE::E::eKEY_DOWN) ? INPUTSTATE::E::ePRESSED :
                   (+EVENTTYPE::E::eKEY_UP) ? INPUTSTATE::E::eRELEASED : INPUTSTATE::E::eNONE;
      m_mainInputCode = SDLKeyCodetoINPUTCODE.at(static_cast<uint32>(newEvent.key.key));
      m_modifierState = MODIFIERSTATE::E::_from_integral(newEvent.key.mod);
    }
    else if (+EVENTTYPE::E::eMOUSE_BUTTON_DOWN || +EVENTTYPE::E::eMOUSE_BUTTON_UP) {
      m_inputState = (+EVENTTYPE::E::eKEY_DOWN) ? INPUTSTATE::E::ePRESSED :
                   (+EVENTTYPE::E::eKEY_UP) ? INPUTSTATE::E::eRELEASED : INPUTSTATE::E::eNONE;
      m_mainInputCode = INPUTCODE::E::_from_integral((newEvent.button.button) << 16 | (newEvent.button.button << 4) | (7));
      mousePosition.x = newEvent.button.x; 
      mousePosition.y = newEvent.button.y;
    }
    else if (+EVENTTYPE::E::eMOUSE_MOTION) {
      // inputState = INPUTSTATE::E::eHELD;
      m_mainInputCode = INPUTCODE::E::eMMWINDOW;
      m_secondaryInputCode = INPUTCODE::E::eMMDELTA;

      mousePosition.x = newEvent.button.x;
      mousePosition.y = newEvent.button.y;
      mouseDelta.x = newEvent.motion.xrel;
      mouseDelta.y = newEvent.motion.yrel;


    }
    else if (+EVENTTYPE::E::eMOUSE_WHEEL) {
      m_inputState = INPUTSTATE::E::eHELD;
      m_mainInputCode = INPUTCODE::E::eMWHORIZONTAL;
    }
    else if (+EVENTTYPE::E::eWINDOW_RESIZED) {

    }

  }

  WindowEvent(const EVENTTYPE::E& newType, uint32 index)
    : type(newType) {
  }

  WindowEvent(const WindowEvent& other, uint32 index)
    : type(other.type) {
  }


  EVENTTYPE::E type = EVENTTYPE::E::eNONE;
  uint32 windowIndex = 0;
  uint64 timestamp = 0;
  int32 data1 = 0;
  int32 data2 = 0;
  int32 userCode = 0;

  uint32 displayID = 0;
  uint32 windowID = 0;
  uint32 keyboardID = 0;
  uint32 mouseID = 0;
  uint32 joystickID = 0;
  Vector2i mousePosition = Vector2i(-1, -1);
  Vector2f mouseScroll = Vector2f::ZERO;
  Vector2f mouseDelta = Vector2f::ZERO;

  INPUTCODE::E m_mainInputCode        = INPUTCODE::E::eNONE;
  INPUTCODE::E m_secondaryInputCode   = INPUTCODE::E::eNONE;
  INPUTCODE::E m_tertiaryInputCode    = INPUTCODE::E::eNONE;
  INPUTCODE::E m_quaternaryInputCode  = INPUTCODE::E::eNONE;
  INPUTCODE::E m_fifthInputCode       = INPUTCODE::E::eNONE;

  MODIFIERSTATE::E m_modifierState = MODIFIERSTATE::E::eNONE;
  INPUTSTATE::E m_inputState = INPUTSTATE::E::eINACTIVE;
  
  Vector2f filePosition = Vector2f::ZERO;
  String m_fileSourceApp = Utils::BLANKSTRING;
  String m_fileName = Utils::BLANKSTRING;

  Vector<String> clipboardMIME;

  uint64 sensorTimestamp = 0;

  // TODO: ADD SCANCODE
  // TODO: ADD TEXTEDITING DATA
  // TODO: ADD TEXTEDITING CANDIDATE EVENTS
  // TODO: ADD TEXT INPUT EVENT
  // TODO: ADD JOYBALL EVENT
  // TODO: ADD JOYHAT EVENT
  // TODO: ADD GAMEPAD SENSOR EVENT
  // TODO: ADD AUDIO DEVICE EVENT
  // TODO: ADD CAMERA DEVICE EVENT
  // TODO: ADD TOUCH FINGER EVENT
  // TODO: ADD PEN PROXIMITY EVENT
  // TODO: ADD PEN MOTION EVENT
  // TODO: ADD PEN TOUCH EVENT
  // TODO: ADD PEN BUTTON EVENT
  // TODO: ADD PEN AXIS EVENT
  // TODO: ADD SENSOR EVENT


};
}

