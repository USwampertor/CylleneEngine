#pragma once

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
# include <shellapi.h>
# include <WinUser.h>
#endif

#include "cyPlatformUtilities.h"
#include "cyFileSystem.h"


namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32

struct CY_UTILITY_EXPORT WindowsUtils : public PlatformUtils
{
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
    ShellExecute(nullptr, "open", path.c_str(), nullptr, nullptr, showOption);
  }
  
  static void
  open(const Path& path, const String& parameters = "", const int32& showOption = SW_SHOWNORMAL) {
    ShellExecute(nullptr, "open", path.fullPath().c_str(), nullptr, nullptr, showOption);
  
  }
  
};

#endif
}
