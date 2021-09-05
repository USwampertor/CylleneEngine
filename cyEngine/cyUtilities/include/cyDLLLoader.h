/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyDynamicLibrary.h
 * @author Marco "Swampy" Millan
 * @date 2019/07/28 2019
 * @brief
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyModule.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "windows.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

namespace CYLLENE_SDK {
  struct DLLLoader {
  public:
    DLLLoader() = default;

    ~DLLLoader() = default;

    static void*
    Load(String path, String functionName = "create");

  };
}

