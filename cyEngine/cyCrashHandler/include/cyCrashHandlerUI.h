#pragma once

#include "cyPlatformCrashHandlerUI.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32 
# include "cyWindowsCrashHandlerUI.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32 
  using CrashHandlerUI = WindowsCrashHandlerUI;
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#else
  // This should never be called as this is a virtual class
  // If you got here, your OS is likely TempleOS or some shit like that
  // PLEASE try using something more common
  using CrashHandlerUI = PlatformCrashHandlerUI;
#endif
}