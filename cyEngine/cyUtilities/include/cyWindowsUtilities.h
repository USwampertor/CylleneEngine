#pragma once

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
# include <shellapi.h>
# include <WinUser.h>
#endif

#include "cyUtilitiesPrerequisites.h"

#include "cyFileSystem.h"
#include "cyPlatformUtilities.h"


namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32

struct CY_UTILITY_EXPORT WindowsUtils : public PlatformUtils
{
 public:
  static void 
  terminate(bool forceTermination, int32 exitValue = 0) {
    if (!forceTermination) {
      PostQuitMessage(exitValue);
    }
    else {
      TerminateProcess(GetCurrentProcess(), exitValue);
    }
  }
  
  static void
  open(const String& path, const String& parameters = "", const int32& showOption = SW_SHOWNORMAL) {
    ShellExecute(nullptr, LPCSTR("open"), path.c_str(), parameters.c_str(), nullptr, showOption);
  }
  
  static void
  open(const Path& path, const String& parameters = "", const int32& showOption = SW_SHOWNORMAL) {
    ShellExecute(nullptr, LPCSTR("open"), path.fullPath().c_str(), parameters.c_str(), nullptr, showOption);
  }

};

#endif
}
