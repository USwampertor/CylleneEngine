
#include "cyCrashHandlerPrerequisites.h"
#include "cyCrashHandlerUI.h"


#include <imgui/imgui.h>
#include <tchar.h>

#include <cyFileSystem.h>
#include <cyUtilities.h>

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include <cyWindows.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#endif

using namespace CYLLENE_SDK;
int
main2(int argc, char* argv[]) {
  CrashHandlerUI window(1024, 576);
  window.init(nullptr);
  while (!window.isDone()) {
    window.newFrame();
    window.showWindow();
  }
  window.shutdown();
  return 0;
}
