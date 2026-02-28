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
    
    for (int32 i = 0; i < InputDefinitions::MAX_GAMEPADS; ++i) {
      m_gamepad.push_back(Map<INPUTCODE::E, SPtr<InputValue>>());
    }

    // Initialize the amount of gamepads we can have
    // Initialize the amount of keyboards we can have
    // Initialize the amount of mouses we can have
    // Initialize the amount of extra elements we can have
    for (INPUTCODE::E i : INPUTCODE::E::_values()) {
      int32 hexValue = i._to_integral();
      String hexStr = Utils::intToHex(hexValue);
      String deviceStr = hexStr.substr(hexStr.size() - 1);
      String inputStr = hexStr.substr(0, hexStr.size() - 1);

      String prefixD(static_cast<SizeT>(8 - deviceStr.size()), '0');
      String prefixI(static_cast<SizeT>(8 - inputStr.size()), inputStr.size() > 6 ? 'F' : '0');
      
      int32 device = std::stoi(prefixD + deviceStr, nullptr, 16);
      
      uint32 keycode = std::stoul(prefixI + inputStr, nullptr, 16);

      INPUTDEVICEID::E deviceType = INPUTDEVICEID::E::_from_integral(device);
      // m_values.try_emplace(i, Vector<SPtr<InputValue>>());

      if (+INPUTDEVICEID::E::eGAMEPAD == deviceType) {
        for (int32 j = 0; j < InputDefinitions::MAX_GAMEPADS; ++j) {
          m_gamepad[j].try_emplace(i, makeSharedPtr<InputValue>());
          m_gamepad[j][i]->reset();
        }
      }
      else if (+INPUTDEVICEID::E::eKEYBOARD == deviceType) {
        m_keyboard[i] = makeSharedPtr<InputValue>();
        m_keyboard[i]->reset();
      }
      else if (+INPUTDEVICEID::E::eMOUSE == deviceType) {
        m_mouse[i] = makeSharedPtr<InputValue>();
        m_mouse[i]->reset();
      }

    }

    init();
  }

  bool 
  init();

  void
  pollEvents(WPtr<WindowEvent> event);

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

  WPtr<InputValue>
  getMouseData(const INPUTCODE::E& code);

  WPtr<InputValue>
  getKeyboardData(const INPUTCODE::E& code);

  WPtr<InputValue>
  getGamepadData(const uint32& device, const INPUTCODE::E& code);

  void
  registerGamepad(const uint32 value);

  void
  removeGamepad(const uint32 value);

  void
  notifyInputChange(const INPUTCODE::E& code, const uint32& device);

  void
  registerInputMapping(SPtr<InputMapping> newMap);


  Vector<SPtr<InputMapping>> m_maps;

  // TODO: remove this
  // Map<INPUTCODE::E, Vector<SPtr<InputValue>>> m_values;
  // TODO: remove this
  // uint32 m_gamepads;


  Map<INPUTCODE::E, SPtr<InputValue>> m_mouse;
  
  Map<INPUTCODE::E, SPtr<InputValue>> m_keyboard;

  Vector<Map<INPUTCODE::E, SPtr<InputValue>>> m_gamepad;

};

}


