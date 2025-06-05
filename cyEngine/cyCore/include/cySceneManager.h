#pragma once

#include "cyCorePrerequisites.h"

#include "cyScene.h"
#include "cySceneGraph.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

class CY_CORE_EXPORT SceneManager
{
public:
  template <typename T,
  typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>>
    void destroyObject(SPtr<T> toDelete) {
    toDelete->m_markedToDestroy = true;
    m_activeScene->m_toRemove.push_back(toDelete);

  }

  template <typename T,
    typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>,
    typename... Args>
  SPtr<T> createObject(Args ... args)
  {
    SPtr<T> newBeing = makeSharedPtr<T>(std::forward<Args>(args)...);
    newBeing->Initialize();
    m_activeScene->m_nodes.push_back(newBeing);
    return newBeing;
  }

  template <typename T,
    typename = std::enable_if_t<std::is_base_of<BBeing, T>::value>>
  SPtr<T> findObject(const String& toFind) {
    int i = 0;
    for (const SPtr<BBeing>& e : m_activeScene->m_nodes)
    {
      if (e->GetName() == toFind && !e->m_markedToDestroy)
      {
        return std::static_pointer_cast<T>(e);
      }
      ++i;
    }
    return nullptr;
  }

  template<typename T>
  Vector<SPtr<BBeing>> findBBeingsWithComponent() const {
    if (m_activeScene) {
      return m_activeScene->getAllBeingsWithComponent<T>();
    }
    return {};
  }

  template<typename T>
  SPtr<BBeing> findFirstBBeingWithComponent() const {
    if (m_activeScene) {
      return m_activeScene->getFirstBeingWithComponent<T>();
    }
    return nullptr;
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

