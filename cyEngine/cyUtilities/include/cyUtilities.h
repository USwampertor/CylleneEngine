/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file Utilities.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
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
