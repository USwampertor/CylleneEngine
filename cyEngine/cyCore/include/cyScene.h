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
#include "cySNode.h"
#include "cyGameMode.h"
#include "cyCComponent.h"

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
    : m_sceneName(name) {}
  
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
    return m_sceneName;
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  Vector<SPtr<BBeing>> getAllBeingsWithComponent(T type) const {
    // Vector<SPtr<BBeing>> result;
    // for (const auto& node : m_rootNode->getChildren()) {
    //   if (auto being = std::static_pointer_cast<BBeing>(node.lock())) {
    //     if (being->getComponent<T>()) {
    //       result.push_back(being);
    //     }
    //   }
    // }
    // return result;
    // return m_rootNode->findChildren([&](WPtr<BBeing> node) {
    //   if (auto being = std::static_pointer_cast<BBeing>(node.lock())) {
    //     return being->hasComponent(T::staticType());
    //   }
    //   return false;
    // }, true);)
    return {};
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  WPtr<BBeing> getFirstBeingWithComponent(T type) const {
    // return m_rootNode->findChildWhere([&](WPtr<BBeing> node) {
    //   if (auto being = std::static_pointer_cast<BBeing>(node.lock())) {
    //     return being->hasComponent(T::staticType());
    //   }
    //   return false;
    // }, true);)
    // for (const auto& node : m_rootNode->getChildren()) {
    //   if (auto being = std::static_pointer_cast<BBeing>(node.lock())) {
    //     if (auto comp = being->getComponent<T>()) {
    //       return being;
    //     }
    //   }
    // }
    // return {};
    return {};
  }

  Vector<WPtr<BBeing>> 
  getAllBeings() const {
    Vector<WPtr<BBeing>> result;
    for (const auto& node : m_beingVector) {
      result.push_back(node);
    }
    return result;
  }

  template<typename T>
  Vector<WPtr<T>> 
  getBeingsOfType() const {
    Vector<WPtr<T>> result;
    for (const auto& node : m_beingVector) {
      if (auto being = std::static_pointer_cast<T>(node)) {
        result.push_back(being);
      }
    }
    return result;
  }

  UPtr<SNode>& getRootNode() {
    return m_rootNode;
  }

  friend class SceneManager;

protected:

  Vector<SPtr<BBeing>> m_toRemove;

  Event<void, SPtr<BBeing>> onBeingAdded;
  Event<void, SPtr<BBeing>> onBeingRemoved;
  Event<void> onSceneLoadedEvent;

private:

  String m_sceneName;
  
  UPtr<SceneSettings> m_settings;
  UPtr<SNode> m_rootNode;

  Vector<SPtr<BBeing>> m_beingVector;

};

}

