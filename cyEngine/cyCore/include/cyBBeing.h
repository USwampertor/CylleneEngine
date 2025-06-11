/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyBBeing.h
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
#include "cyCComponent.h"
#include "cyCTransform.h"
#include "cySNode.h"


#include <cyUtilities.h>
#include <cyJSON.h>
#include <cyQuaternion.h>
#include <cyVector3f.h>

namespace CYLLENE_SDK
{


class CSprite;

/*
 *	@class	Being
 *	@brief	All objects that exist and are updated or not in the game engine
 *          are Being child classes
 *
 */
class CY_CORE_EXPORT BBeing : public SNode
{

  BODY();

public:
  /**
   * Default Constructor
   */
  BBeing() = default;

  BBeing(const String& name) : SNode(name) {}

  BBeing(const BBeing& other) 
    : SNode(other),
      m_components(other.m_components) {}

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  void 
  addComponent(SPtr<T>& component) {
    // Is there already a component like this?
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) == m_components.end()) {
      // There is no component like this, se we should be able to add it
      component->setOwner(this);
      // m_components.insert(Utils::makePair(type, makeSharedPtr<T>(args ...)));
      m_components.try_emplace(type, component);
    }
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  WPtr<T>
  getComponent() {
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) != m_components.end()) {
      return REINTERPRETPOINTER(T, m_components.at(type));
    }
    return {};
  }

  bool
  hasComponent(COMPONENT_TYPE::E type) const {
    return m_components.find(type) != m_components.end();
  }

  template<typename T, typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>>
  void 
  removeComponent() {
    COMPONENT_TYPE::E type = T::staticType();
    if (m_components.find(type) != m_components.end()) {
      m_components.at(type)->m_parent = nullptr;
      m_components.erase(type);
    }
  }

  void
  removeAllComponents();

  template <typename T, 
            typename = std::enable_if_t<std::is_base_of<CComponent, T>::value>, 
            typename ... Args>
  WPtr<T>
  createComponent(Args ... args) {

    COMPONENT_TYPE::E type = T::staticType();

    // Is the component already created?
    if (m_components.find(type) == m_components.end()) {
      // m_components.insert(Utils::makePair(type, makeSharedPtr<T>(args ...)));
      SPtr<T> newComponent = makeSharedPtr<T>(std::forward<Args>(args)...);
      m_components.try_emplace(type, newComponent);
      m_components.at(type)->setOwner(makeSharedPtr<BBeing>(*this));
    }

    // In any case, either existing or non existing, we can just return what is at
    // that location
    return REINTERPRETPOINTER(T, m_components.at(type));
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

  virtual void
  update(const float& delta) { 
    for (auto& component : m_components) {
      component.second->update(delta);
    }
  }

  virtual WPtr<CTransform>
  getTransform() override {
    return getComponent<CTransform>();
  }

  // void 
  // addChild(SPtr<SNode> child, bool keepWorldTransform = true) override;
  // 
  // void 
  // removeChild(SPtr<SNode> child, bool recursive = false) override;
  // 
  // SPtr<SNode> 
  // findChild(const String& name, bool recursive = true) const override;

  SPtr<BBeing> 
  findBeing(const String& name, bool recursive = true) const;

  Vector<SPtr<BBeing>> 
  getAllBeingsInHierarchy() const;

  // SPtr<BBeing> 
  // createChild(const String& name);

  friend class SceneManager;
  friend class SNode;

private:

  void
  markToDestroy() { m_markedToDestroy = true; }

private:
  /**
   * The name of the Being
   */
  // String m_nodeName;

  /**
   * The components that has the Being
   */
  Map<COMPONENT_TYPE::E, SPtr<CComponent>> m_components;


#if defined(CY_DEBUG) || defined(CY_DEVELOPMENT)
  SPtr<CSprite> m_gizmo = nullptr;
#endif

};

REGISTER_CLASS(BBeing);

}

