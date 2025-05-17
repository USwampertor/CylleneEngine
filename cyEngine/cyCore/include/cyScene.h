/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyScene.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/20
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"

#include "cyBBeing.h"
#include "cySceneNode.h"
#include "cyGameMode.h"

#include <cyJSON.h>
#include <cyVector2f.h>

namespace CYLLENE_SDK {

struct SceneSettings
{
  Vector2f m_gravity;
  UPtr<GameMode> m_gameMode;
};

/*
 *	@class	Scene
 *	@brief	
 *
 */
class CY_CORE_EXPORT Scene
{
public:
  Scene() = default;
  ~Scene() = default;

  Scene(const String& name);

private:

  String m_name;
  UPtr<SceneSettings> m_settings;
  Vector<SPtr<SNode>> m_beings;

  Vector<SPtr<BBeing>> m_toRemove;
};

}

