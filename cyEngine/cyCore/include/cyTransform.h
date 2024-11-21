/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyTransform.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/14
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"
#include "cyComponent.h"

#include <cyVector2f.h>
#include <cyEulerHelpers.h>
#include <cyQuaternion.h>


class CY_CORE_EXPORT TransformComponent : public Component
{
  TransformComponent() : Component(TransformComponent::staticType()) {}


  TransformComponent(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot)
    : m_position(newPos),
      m_scale(newSc),
      m_rotation(newRot) {
    m_type = TransformComponent::staticType();
  }

  Vector3f&
  getPosition() {
    return m_position;
  }

  void
  setPosition(const Vector3f& newPos) {
    m_position = newPos;
  }

  Vector3f&
  getScale() {
    return m_scale;
  }

  void
  setScale(const Vector3f& newScale) {
    m_scale = newScale;
  }

  Quaternion&
  getRotation() {
    return m_rotation;
  }

  void
  setRotation(const Quaternion& newRotation) {
    m_rotation = newRotation;
  }

  Euler
  getEulerRotation() {
    return m_rotation.toEuler(0);
  }

  void
  setEulerAngle(const Vector3f& newEulerAngles) {
    Euler e(newEulerAngles);
    m_rotation.fromEuler(e, 0);
  }

  void
  setTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot) {
    m_position  = newPos;
    m_scale     = newSc;
    m_rotation  = newRot;
  }

  void
  setTransform(const TransformComponent& other) {
    setTransform(other.)
  }

  WeakPointer<TransformComponent>&
  getParent() {
    return !m_parent.expired() ? m_parent : {};
  }

  Vector<WeakPointer<TransformComponent>>&
  getChildren() {
    return m_children;
  }

  void
  translate(const Vector3f delta) {
    m_position += delta;
  }

  void 
  scale(const Vector3f delta) {
    m_scale += delta;
  }

  void
  rotate(const Quaternion& delta) {
    m_rotation += delta;
  }

  void
  rotate(const Vector3f deltaAngles) {
    Euler e(deltaAngles);
    m_rotation += Quaternion(e, 0);
  }

  void
  reset() {
    m_position  = { 0,0,0 };
    m_scale     = { 0,0,0 };
    m_rotation  = { 0,0,0,1 };
  }

  virtual COMPONENT_TYPE::E staticType() override { return COMPONENT_TYPE::E::eTRANSFORM; }


private:

  Vector3f m_position;
  
  Vector3f m_scale;

  Quaternion m_rotation;

  WeakPointer<TransformComponent> m_parent;

  Vector<WeakPointer<TransformComponent>> m_children;
};