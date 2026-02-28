/**
 * @file cyMath.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Math.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyPlatformMath.h"
#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindowsMath.h"
#endif

namespace CYLLENE_SDK {
#if CY_PLATFORM == CY_PLATFORM_WIN32
  using Math = WindowsMath;
#else
  using Math = PlatformMath;
#endif

}
