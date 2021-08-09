/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyCrashHandler.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyModule.h"
#include "cyUtilitiesPrerequisites.h"


namespace CYLLENE_SDK
{
  class CrashHandler : public Module<CrashHandler>
  {
  public:
    
    void
    init();

    void 
    createReport();
    
  };
}
