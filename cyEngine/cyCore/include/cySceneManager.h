#pragma once

#include "cyCorePrerequisites.h"

#include "cyScene.h"
#include "cySceneGraph.h"
#include "cyBBeing.h"

#include <cyModule.h>

namespace CYLLENE_SDK {

class CY_CORE_EXPORT SceneManager : public Module<SceneManager>
{
public:

  SceneManager() = default;

  ~SceneManager() = default;

  virtual void
  onStartUp() override;

  template <typename T,
  typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>>
    void destroyBeing(SPtr<T> toDelete) {
    toDelete->m_markedToDestroy = true;
    m_activeScene->m_toRemove.push_back(toDelete);

  }

  template <typename T,
    typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>,
    typename... Args>
  WPtr<T> createBeing(Args ... args)
  {
    SPtr<T> newBeing = makeSharedPtr<T>(std::forward<Args>(args)...);
    newBeing->init();
    m_activeScene->m_beingVector.push_back(newBeing);
    m_activeScene->m_rootNode->getChildren().push_back(newBeing);
    return newBeing;
  }

  template <typename T,
    typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>>
  WPtr<T> instantiateBeing(Vector3f position = Vector3f::ZERO, 
                           Quaternion rotation = Quaternion::IDENTITY,
                           WPtr<CTransform> parent = nullptr)
  {
    SPtr<T> newBeing = makeSharedPtr<T>();
    newBeing->init();
    m_activeScene->m_beingVector.push_back(newBeing);
    m_activeScene->m_rootNode->getChildren().push_back(newBeing);
    if (parent.lock() != nullptr) {
      parent.lock()->m_owner.lock()->addChild(newBeing);
    }
    return newBeing;
  }

  
  template <typename T,
  typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>>
  WPtr<T> 
  findBeing(const String& toFind, bool recursive = true) {
    for (const WPtr<SNode>& node : m_activeScene->m_rootNode->getChildren()) {
      if (auto being = std::static_pointer_cast<BBeing>(node.lock())) {
        if (being->getName() == toFind && !being->m_markedToDestroy) {
          return std::static_pointer_cast<T>(being);
        }
        if (recursive) {
          auto found = being->findBeing(toFind, true);
          if (found) return std::static_pointer_cast<T>(found);
        }
      }
    }
    return {};
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  Vector<WPtr<BBeing>> findBeingsWithComponent(T type) const {
    if (m_activeScene) {
      return m_activeScene->getAllBeingsWithComponent(type);
    }
    return {};
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  WPtr<BBeing> findFirstBeingWithComponent(T type) const {
    if (m_activeScene) {
      return m_activeScene->getFirstBeingWithComponent(type);
    }
    return {};
  }

  SPtr<Scene> createScene(const String& newSceneName);

  SPtr<Scene> loadScene(const String& newSceneName);

  SPtr<Scene> unloadScene(const String& sceneToUnload);

  SPtr<Scene> findScene(const String& sceneToFind);

  bool changeScene(const String& sceneToLoad);

  void saveScene();

  void update(const float& delta);

  // void UpdateRender(RenderWindow& w);

  SPtr<Scene> getActiveScene();

  JSONDocument serialize(const Vector<String>& names);

  void deserialize(const JSONValue& sceneArray);

  SPtr<Scene> m_activeScene;

  Vector<SPtr<Scene>> m_scenes;

  bool m_isDebug = false;
};

}

