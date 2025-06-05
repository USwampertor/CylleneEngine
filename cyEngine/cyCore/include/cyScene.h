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
#include <cyEvent.h>

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

  template<typename T>
  Vector<SPtr<BBeing>> getAllBeingsWithComponent() const {
    Vector<SPtr<BBeing>> result;
    for (const auto& node : m_nodes) {
      if (auto being = std::static_pointer_cast<BBeing>(node)) {
        if (being->getComponent<T>()) {
          result.push_back(being);
        }
      }
    }
    return result;
  }

  template<typename T>
  SPtr<BBeing> getFirstBeingWithComponent() const {
    for (const auto& node : m_nodes) {
      if (auto being = std::static_pointer_cast<BBeing>(node)) {
        if (auto comp = being->getComponent<T>()) {
          return being;
        }
      }
    }
    return nullptr;
  }

  Vector<SPtr<BBeing>> 
  getAllBeings() const {
    Vector<SPtr<BBeing>> result;
    for (const auto& node : m_nodes) {
      if (auto being = std::static_pointer_cast<BBeing>(node)) {
        result.push_back(being);
      }
    }
    return result;
  }

  template<typename T>
  Vector<SPtr<T>> 
  getBeingsOfType() const {
    Vector<SPtr<T>> result;
    for (const auto& node : m_nodes) {
      if (auto being = std::static_pointer_cast<T>(node)) {
        result.push_back(being);
      }
    }
    return result;
  }

  friend class SceneManager;

protected:

  Vector<SPtr<BBeing>> m_toRemove;

  Event<void, SPtr<BBeing>> onBeingAdded;
  Event<void, SPtr<BBeing>> onBeingRemoved;
  Event<void> onSceneLoadedEvent;

private:

  String m_name;
  UPtr<SceneSettings> m_settings;
  Vector<SPtr<SNode>> m_nodes;

};

}

