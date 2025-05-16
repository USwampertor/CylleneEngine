#pragma once
#include "cyCorePrerequisites.h"


namespace CYLLENE_SDK {


  namespace INPUTSTATE
  {
    BETTER_ENUM(E,
      uint32,
      PRESSED,
      HELD,
      RELEASED,
      INACTIVE
    );
  }

  namespace DEVICEID
  {
    BETTER_ENUM(E, uint32,
                MOUSE,
                MOUSEWHEEL,
                JOYSTICK,
                JOYSTICKAXIS,
                KEYBOARD,
                KEYSCAN);
  }


  namespace INPUTCODE
  {
    BETTER_ENUM(E, int32,
      MLEFT,
      MRIGHT,
      MMIDDLE,
      MXBUTTON1,
      MXBUTTON2,
      MWVERTICAL,
      MWHORIZONTAL

      );
  }

struct InputValue
{
  INPUTSTATE::E m_state = INPUTSTATE::E::INACTIVE;

  float m_timePressed;

  float m_timeInactive;

  float m_value;
};



}
