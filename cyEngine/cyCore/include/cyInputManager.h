#pragma once
#include "cyCorePrerequisites.h"
#include "cyInputMapping.h"

#include <cyModule.h>
#include <cyEvent.h>
#include <cyVector2i.h>
#include <cyWindow.h>

namespace CYLLENE_SDK {

class InputManager : Module<InputManager>
{
  virtual void onStartUp() override {



    for (INPUTCODE::E input : INPUTCODE::E::_values()) {
      
      
    }
  }

  bool 
  init();

  void
  pollEvents();

  void
  update(const float& delta);

  Vector2i
  getWindowMousePosition();

  Vector2i
  getScreenMousePosition();

  void
  setWindowMousePosition(SPtr<Window> window, const Vector2i& pos);

  void
  setScreenMousePosition(const Vector2i& pos);

  void
  registerGamepad(const uint32 value);

  void
  removeGamepad(const uint32 value);

  void
  notifyInputChange(const INPUTCODE::E& code, const uint32& device);

  void
  registerInputMapping(SPtr<InputMapping> newMap);


  Vector<SPtr<InputMapping>> m_maps;

  Map<INPUTCODE::E, Vector<SPtr<InputValue>>> m_values;

  uint32 m_gamepads;

};

}


