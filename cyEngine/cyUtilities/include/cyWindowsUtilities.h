/**
 * @file cyWindowsUtilities.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for WindowsUtilities.
 */

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

/**
 * @struct WindowsUtils
 * @brief Windows-specific utility extensions.
 */
struct CY_UTILITY_EXPORT WindowsUtils : public PlatformUtils
{
 public:
  /**
   * @brief Terminates the application.
   * @param forceTermination If true, force process termination.
   * @param exitValue Exit code.
   */
  static void 
  terminate(bool forceTermination, int32 exitValue = 0) {
    if (!forceTermination) {
      PostQuitMessage(exitValue);
    }
    else {
      TerminateProcess(GetCurrentProcess(), exitValue);
    }
  }
  
  /**
   * @brief Opens a path using ShellExecute.
   * @param path Path or URL to open.
   * @param parameters Optional command parameters.
   * @param showOption Window show option.
   */
  static void
  open(const String& path, const String& parameters = "", const int32& showOption = SW_SHOWNORMAL) {
    ShellExecuteA(nullptr, LPCSTR("open"), path.c_str(), parameters.c_str(), nullptr, showOption);
  }
  
  /**
   * @brief Opens a filesystem path using ShellExecute.
   * @param path Path object to open.
   * @param parameters Optional command parameters.
   * @param showOption Window show option.
   */
  static void
  open(const Path& path, const String& parameters = "", const int32& showOption = SW_SHOWNORMAL) {
    ShellExecuteA(nullptr, LPCSTR("open"), path.fullPath().c_str(), parameters.c_str(), nullptr, showOption);
  }

};

#endif
}

