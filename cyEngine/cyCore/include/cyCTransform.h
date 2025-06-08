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
    m_parent = other.m_parent;  
    m_children = other.m_children;
  }

  CTransform(const Vector3f& position = Vector3f::ZERO,
             const Vector3f& scale = Vector3f::ONE,
             const Quaternion& rotation = Quaternion::IDENTITY,
             const SPtr<CTransform>& parent = nullptr) : CComponent(CTransform::staticType()) {
    m_tMatrix.setTransformMatrix(position, rotation, scale);
    setParent(parent);
  }

  Vector3f
  getWorldPosition();

  const Vector3f&
  getLocalPosition() {
    return m_tMatrix.getPosition();
  }

  void
  setWorldPosition(const Vector3f& newPos);

  void
  setLocalPosition(const Vector3f& newPos) {
    m_tMatrix.setPosition(newPos);
  }

  Vector3f
  getWorldScale();

  const Vector3f&
  getLocalScale() {
    return m_tMatrix.getScale();
  }

  void
  setWorldScale(const Vector3f& newScale);

  void
  setLocalScale(const Vector3f& newScale) {
    m_tMatrix.setScale(newScale);
  }

  Quaternion
  getWorldRotation();

  Quaternion
  getLocalRotation() {
    return m_tMatrix.getQuatRotation();
  }

  void
  setWorldRotation(const Quaternion& newRotation);

  void
  setLocalRotation(const Quaternion& newRotation) {
    m_tMatrix.setRotation(newRotation);
  }

  Vector3f
  getWorldEulerRotation();

  Vector3f
  getLocalEulerRotation() {
    Euler e = m_tMatrix.getEulerRotation();
    return Vector3f(e.x, e.y, e.z);
  }

  void
  setWorldEulerAngle(const Vector3f& newEulerAngles);

  void
  setLocalEulerAngle(const Vector3f& newEulerAngles) {
    m_tMatrix.setRotation(newEulerAngles);
  }

  Matrix4
  getWorldTransform();

  Matrix4
  getLocalTransform() {
    return m_tMatrix;
  }

  void
  setWorldTransform(const Vector3f& newPos, 
                    const Vector3f& newScale, 
                    const Quaternion& newRot = Quaternion::IDENTITY);

  void
  setWorldTransform(const Matrix4& other);

  void
  setLocalTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot) {
    m_tMatrix.setTransformMatrix(newPos, newRot, newSc);
  }

  void
  setLocalTransform(const Matrix4& other) {
    m_tMatrix = other;
  }

  void
  setLookAt(const Vector3f& targetPos, const Vector3f& upDir = Vector3f::UP);

  void
  setWorldLookAt(const Vector3f& eyePos, 
                 const Vector3f& targetPos, 
                 const Vector3f& upDir = Vector3f::UP);

  void
  setLocalLookAt(const Vector3f& eyePos, 
                 const Vector3f& targetPos, 
                 const Vector3f& upDir = Vector3f::UP);

  void
  updateLocalFromWorld();

  uint32 
  getChildCount() {
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
  reset() {
    m_tMatrix.identity();
  }

  void addChild(const SPtr<CTransform>& newChild);

  void addChildren(const Vector<SPtr<CTransform>>& newChildren);

  void removeChild(const String& name);

  void removeChildAt(const uint32& index);

  void removeAllChildren();

  WPtr<CTransform> getChild(const String& name);

  void setParent(const SPtr<CTransform>& newParent);

  bool isChildOf(const SPtr<CTransform>& potentialParent);
  
  bool isParentOf(const SPtr<CTransform>& potentialChild);

  SPtr<CTransform> findChild(const String& name, bool recursive = false);

  Vector3f transformPoint(const Vector3f& point);

  Vector3f transformDirection(const Vector3f& direction);
  
  Vector3f inverseTransformPoint(const Vector3f& point);
  
  Vector3f inverseTransformDirection(const Vector3f& direction);

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

private:

  bool
  isRootSC(SPtr<CTransform> parent);

  bool
  isPartOfSceneSC(SPtr<CTransform> child);

public:

  Matrix4 m_tMatrix;

  WPtr<CTransform> m_parent;
  
  Vector<WPtr<CTransform>> m_children;
};

}

