#pragma once

#include "cyCorePrerequisites.h"
#include "cyInputAction.h"
#include "cyInput.h"

namespace CYLLENE_SDK
{

class InputMapping
{
public:

  InputMapping() = default;
  ~InputMapping() = default;

  void 
  bindAction(const INPUTCODE::E& button, const InputCallback& c);

  void
  removeAction(const INPUTCODE::E& button);

  void
  onInputUpdated(const INPUTCODE::E& button, const SPtr<InputValue>& value);

  void
  setActive(bool active);

  Map<INPUTCODE::E, SPtr<InputAction>> m_actions;

  bool m_enabled;

};

}

