/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyBeing.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/20
 * @brief 	The smallest unit object that can exist, not exist, 
 *          be spawned, used, etc.
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once
#include "cyCorePrerequisites.h"

#include "cyClassRegister.h"
#include "cyComponent.h"
#include "cyTransform.h"


#include <cyUtilities.h>
#include <cyJSON.h>
#include <cyQuaternion.h>
#include <cyVector3f.h>

namespace CYLLENE_SDK
{


class Sprite;

/*
 *	@class	Being
 *	@brief	All objects that exist and are updated or not in the game engine
 *          are Being child classes
 *
 */
class CY_CORE_EXPORT Being
{

  BODY();

public:
  /**
   * Default Constructor
   */
  Being() = default;

  Being(const String& name) : m_name(name) {}

  template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
  void 
  addComponent(SPtr<T>& component) {
    // Is there already a component like this?
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) == m_components.end()) {
      // There is no component like this, se we should be able to add it
      component->setOwner(this);
      m_components.insert(Utils::makePair(type, component));
    }
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
  T* 
  getComponent() {
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) != m_components.end()) {
      return REINTERPRETPOINTER(T, m_components.at(type)).get();
    }
    return nullptr;
  }


  template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
  void 
  removeComponent() {
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) != m_components.end()) {
      m_components.at(type)->m_parent = nullptr;
      m_components.erase(type);
    }
  }

  template <typename T, 
            typename = std::enable_if_t<std::is_base_of<Component, T>::value>, 
            typename ... Args>
  T* 
  createComponent(Args ... args) {

    COMPONENT_TYPE::E type = T::staticType();

    // Is the component already created?
    if (m_components.find(type) == m_components.end()) {

      m_components.insert(Utils::makePair(type, makeSharedPtr<T>(args ...)));
      m_components.at(type)->setOwner(std::make_shared(this));
    }

    // In any case, either existing or non existing, we can just return what is at
    // that location
    return REINTERPRETPOINTER(T, m_components.at(type)).get();
  }

  // TODO: Serialization - Check if it can be automatized to avoid writing it every
  // time you create a new Being child

  virtual void 
  onCreate() {}

  virtual void 
  onInit() {}

  virtual void 
  onDestroy() {}

  virtual void 
  init() {}

  void 
  setActive(bool active) {
    m_isActive = active;
  }

  const bool& 
  isActive() { return m_isActive; }

  TransformComponent*
  getTransform() {
    return getComponent<TransformComponent>();
  }

  const String& 
  getName() { return m_name; }

  void
  setName(const String& name) { m_name = name; }

  friend class SceneManager;

private:

  void
  markToDestroy() { m_markedToDestroy = true; }

private:
  /**
   * The name of the Being
   */
  String m_name;

  /**
   * The components that has the Being
   */
  Map<COMPONENT_TYPE::E, SPtr<Component>> m_components;

  bool m_isActive;

  bool m_markedToDestroy;

  Sprite* m_gizmo;
};

REGISTER_CLASS(Being);

}

