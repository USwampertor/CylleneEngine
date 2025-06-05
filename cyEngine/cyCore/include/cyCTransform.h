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
  
  CTransform(const CTransform& other) : 
    CComponent(CTransform::staticType()) {
    m_tMatrix = other.m_tMatrix;
    m_parent.reset();
    m_parent = { other.m_parent };
    m_children = other.m_children;
  }


  CTransform(const Vector3f& position = Vector3f::ZERO,
             const Vector3f& scale = Vector3f::ONE,
             const Quaternion& rotation = Quaternion::IDENTITY,
    const SPtr<CTransform>& parent = nullptr) : CComponent(CTransform::staticType()) {
    m_tMatrix.setTransformMatrix(position, rotation, scale);
    setParent(parent);
    // m_parent.reset();
    // m_parent = { parent }; // TODO: Maybe this can be changed to setParent(parent);
  }

  Matrix4
  getWorldTransform() const;

  Matrix4
  getLocalTransform() const {
    return m_tMatrix;
  }



  const Vector3f&
  getLocalPosition() {
    return m_tMatrix.getPosition();
  }

  void
  setLocalPosition(const Vector3f& newPos) {
    m_tMatrix.setPosition(newPos);
  }

  const Vector3f&
  getLocalScale() {
    return m_tMatrix.getScale();
  }

  void
  setLocalScale(const Vector3f& newScale) {
    m_tMatrix.setScale(newScale);
  }

  Quaternion
  getLocalRotation() {
    return m_tMatrix.getQuatRotation();
  }

  void
  setLocalRotation(const Quaternion& newRotation) {
    m_tMatrix.setRotation(newRotation);
  }

  Vector3f
  getLocalEulerRotation() {
    Euler e = m_tMatrix.getEulerRotation();
    return Vector3f(e.x, e.y, e.z);
  }

  void
  setLocalEulerAngle(const Vector3f& newEulerAngles) {
    m_tMatrix.setRotation(newEulerAngles);
  }

  void
  setLocalTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot) {
    m_tMatrix.setTransformMatrix(newPos, newRot, newSc);
  }

  void
  setLocalTransform(const CTransform& other) {
    m_tMatrix = other.m_tMatrix;
  }

  void
  setLocalLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir = Vector3f::UP);

  void
  updateLocalFromWorld();

  uint32 
  getChildCount() const {
    return static_cast<uint32>(m_children.size());
  }

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
  translateLocal(const Vector3f& delta) {
    m_tMatrix.translate(delta);
  }

  void 
  scaleLocal(const Vector3f& delta) {
    m_tMatrix.scale(delta);
  }

  void
  rotateLocal(const Quaternion& delta) {
    m_tMatrix.rotate(delta);
  }

  void
  rotateLocal(const Vector3f& deltaAngles) {
    m_tMatrix.rotate(Quaternion(Euler(deltaAngles)));
  }

  void
  resetLocal() {
    m_tMatrix.identity();
  }

  void attachChildren(const SPtr<CTransform>& newChild);

  void removeChildren(const String& name);

  void removeChildrenAt(const uint32& index);

  WPtr<CTransform> getChild(const String& name);

  void setParent(const SPtr<CTransform>& newParent);

  bool isChildOf(const SPtr<CTransform>& potentialParent) const;
  
  bool isParentOf(const SPtr<CTransform>& potentialChild) const;

  SPtr<CTransform> findChild(const String& name, bool recursive = false) const;

  Vector3f transformPoint(const Vector3f& point) const;

  Vector3f transformDirection(const Vector3f& direction) const;
  
  Vector3f inverseTransformPoint(const Vector3f& point) const;
  
  Vector3f inverseTransformDirection(const Vector3f& direction) const;

  void forEachChild(Callback<void, SPtr<CTransform>> callback, bool recursive = false);

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

  Matrix4 m_tMatrix;

  WPtr<CTransform> m_parent;
  
  Vector<WPtr<CTransform>> m_children;
};

}

