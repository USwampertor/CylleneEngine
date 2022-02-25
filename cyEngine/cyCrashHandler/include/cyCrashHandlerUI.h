#pragma once

#include "cyPlatformCrashHandlerUI.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32 
#include "cyWindowsCrashHandlerUI.h"
#endif

namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32 
  using CrashHandlerUI = WindowsCrashHandlerUI;
#else
  using CrashHandlerUI = PlatformCrashHandlerUI;
#endif
}