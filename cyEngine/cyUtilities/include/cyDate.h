/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyDate.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyUtilities.h"
#include "cyModule.h"

namespace CYLLENE_SDK {

  class CY_UTILITY_EXPORT Date : public Module<Date> {
  public:
    Date() = default;

    ~Date() = default;
  };
}

