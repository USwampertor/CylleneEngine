/**
 * @file cyUtilities.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Utilities.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindowsUtilities.h"
#else
# include "cyPlatformUtilities.h"
#endif

namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32
  using Utils = WindowsUtils;
#else
  using Utils = PlatformUtils;
#endif
}

