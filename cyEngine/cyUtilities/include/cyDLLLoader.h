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



namespace CYLLENE_SDK {
  struct DLLLoader {
  public:
    DLLLoader() = default;

    ~DLLLoader() = default;

    static void*
    load(const String& path, const String& functionName = "create", bool isOSDll = true);

  };
}

