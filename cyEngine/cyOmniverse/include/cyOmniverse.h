/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyOmniverse.h
 * @author Marco "Swampy" Millan
 * @date 06/09/2022
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyOmniversePrerequisites.h"
#include <cyModule.h>

static bool           g_doLiveEdit = true;
static bool           g_omniverseLogEnabled = false;
static std::string    g_error;
static UsdStageRefPtr g_stage;
std::mutex            g_LogMutex;

namespace CYLLENE_SDK
{
class CY_OMNIVERSE_EXPORT Omniverse : Module<Omniverse> {
  void
  init();

  void
  clear();

};
}
