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

    // Initialize the amount of gamepads we can have
    // Initialize the amount of keyboards we can have
    // Initialize the amount of mouses we can have
    // Initialize the amount of extra elements we can have
    for (INPUTCODE::E i : INPUTCODE::E::_values()) {
    }

    init();
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
  setWindowMousePosition(const Vector2i& pos, uint32 windowIndex = 0);

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


