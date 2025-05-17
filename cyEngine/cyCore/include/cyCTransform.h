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
#include "cyCComponent.h"

#include <cyVector3f.h>
#include <cyEulerHelpers.h>
#include <cyQuaternion.h>
#include <cyUtilities.h>

#include <cyMatrix4.h>

namespace CYLLENE_SDK {

class CY_CORE_EXPORT CTransform : public CComponent
{
public:
  
  // TransformComponent() : Component(TransformComponent::staticType()) {}


  CTransform(const Vector3f& position = Vector3f::ZERO,
                     const Vector3f& scale = Vector3f::ONE,
                     const Quaternion& rotation = Quaternion::IDENTITY)
    : CComponent(CTransform::staticType()) {
        m_tMatrix.setTransformMatrix(position, rotation, scale);
      }

  const Vector3f&
  getPosition() {
    return m_tMatrix.getPosition();
  }

  void
  setPosition(const Vector3f& newPos) {
    m_tMatrix.setPosition(newPos);
  }

  const Vector3f&
  getScale() {
    return m_tMatrix.getScale();
  }

  void
  setScale(const Vector3f& newScale) {
    m_tMatrix.setScale(newScale);
  }

  Quaternion
  getRotation() {
    return m_tMatrix.getQuatRotation();
  }

  void
  setRotation(const Quaternion& newRotation) {
    m_tMatrix.setRotation(newRotation);
  }

  Vector3f
  getEulerRotation() {
    Euler e = m_tMatrix.getEulerRotation();
    return Vector3f(e.x, e.y, e.z);
  }

  void
  setEulerAngle(const Vector3f& newEulerAngles) {
    m_tMatrix.setRotation(newEulerAngles);
  }

  void
  setTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot) {
    m_tMatrix.setTransformMatrix(newPos, newRot, newSc);
  }

  void
  setTransform(const CTransform& other) {
    m_tMatrix = other.m_tMatrix;
  }

  void
  setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir);


  WPtr<CTransform>&
  getParent() {
    // TODO: Check if parent is still valid
    return m_parent;
  }

  Vector<WPtr<CTransform>>&
  getChildren() {
    return m_children;
  }

  void
  translate(const Vector3f& delta) {
    m_tMatrix.translate(delta);
  }

  void 
  scale(const Vector3f& delta) {
    m_tMatrix.scale(delta);
  }

  void
  rotate(const Quaternion& delta) {
    m_tMatrix.rotate(delta);
  }

  void
  rotate(const Vector3f& deltaAngles) {
    m_tMatrix.rotate(Quaternion(Euler(deltaAngles)));
  }

  void
  reset() {
    m_tMatrix.identity();
  }

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eTRANSFORM; }

  virtual const String 
  toString() override {
    String toReturn;

    toReturn = Utils::format("%s \n %s \n %s", 
                             m_tMatrix.getPosition().toString().c_str(), 
                             m_tMatrix.getEulerRotation().toString().c_str(),
                             m_tMatrix.getScale().toString().c_str());

    return toReturn;
  }

public:

//   Vector3f m_position;
//   
//   Vector3f m_scale;
//   
//   Quaternion m_rotation;

  Matrix4 m_tMatrix;

  WPtr<CTransform> m_parent;
  
  Vector<WPtr<CTransform>> m_children;
};

}

