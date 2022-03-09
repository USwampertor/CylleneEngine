#pragma once

#include "cyCorePrerequisites.h"
#include <imgui/imgui.h>
#include <cyModule.h>

namespace CYLLENE_SDK {
class CY_CORE_EXPORT BaseUI : public Module<BaseUI>
{
public:
  BaseUI() = default;

  ~BaseUI() = default;

  bool
  registerMenu(const String& tabName);

  bool
  registerSubMenu(const String& father, const String& child);

  bool
  unregisterMenu(const String& tabName);

  bool
  unregisterSubMenu(const String& father, const String& child);

  Vector<Vector<String>> menus;

  void
  displayUI();

  void
  newFrame();

  void
  endFrame();


};
}
