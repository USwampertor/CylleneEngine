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
  Vector2f m_gravity = { 0 , 0 };
  UPtr<GameMode> m_gameMode = nullptr;
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

  Scene(const String& name)
    : m_name(name) {}
  
  ~Scene() = default;
  
  void 
  init();

  void
  onSceneLoaded();

  void
  onSceneUnloaded();

  JSONDocument
  serialize();

  void
  deserialize(const JSONValue& sceneData);

  UPtr<SceneSettings>& getSettings() {
    return m_settings;
  }

  const String& getName() const {
    return m_name;
  }

  friend class SceneManager;

protected:

  Vector<SPtr<BBeing>> m_toRemove;

private:

  String m_name;
  UPtr<SceneSettings> m_settings;
  Vector<SPtr<SNode>> m_beings;

};

}

