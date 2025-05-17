#pragma once

#include "cyCorePrerequisites.h"
#include "cyInput.h"

#include <cyEvent.h>


namespace CYLLENE_SDK {

using InputCallback = Callback<void, SPtr<InputValue>>;


class InputAction
{
public:

  InputAction() = default;

  InputAction(const String& name,
              const String& description)
    : m_name(name),
      m_description(description) {}

  ~InputAction() = default;

  uint32 GetHashID() { return static_cast<uint32>(Hash<String>()(m_name)); }

  String m_name;
  String m_description;

  InputCallback m_action;
};


}
