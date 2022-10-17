#pragma once

#include "cyUtilitiesPrerequisites.h"

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