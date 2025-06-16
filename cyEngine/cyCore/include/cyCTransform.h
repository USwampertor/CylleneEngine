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
#pragma region Constructors
  CTransform(const CTransform& other, const bool setAsLocalTransform = true)
    : CComponent(CTransform::staticType()),
      m_localMatrix(Matrix4::IDENTITY),
      m_worldMatrix(Matrix4::IDENTITY)
  {
    if (setAsLocalTransform) {
      setLocalTransform(other.m_localMatrix);
    }
    else {
      setWorldTransform(other.m_worldMatrix);
    }
  }

  CTransform(const Vector3f&    position  = Vector3f::ZERO,
             const Vector3f&    scale     = Vector3f::ONE,
             const Quaternion&  rotation  = Quaternion::IDENTITY,
             const bool         setAsLocalTransform = true)
    : CComponent(CTransform::staticType()),
      m_localMatrix(Matrix4::IDENTITY),
      m_worldMatrix(Matrix4::IDENTITY) {
    if (setAsLocalTransform) {
      setLocalTransform(position, scale, rotation);
    }
    else {
      setWorldTransform(position, scale, rotation);
    }
  }

  CTransform(const Matrix4& transformMatrix, const bool setAsLocalTransform = true)
    : CComponent(CTransform::staticType()),
      m_localMatrix(Matrix4::IDENTITY),
      m_worldMatrix(Matrix4::IDENTITY) {
    if (setAsLocalTransform) {
      setLocalTransform(transformMatrix);
    }
    else {
      setWorldTransform(transformMatrix);
    }
  }
#pragma endregion

#pragma region get/set Position
  Vector3f
  getLocalPosition() const;

  Vector3f
  getWorldPosition() const;

  void
  setLocalPosition(const Vector3f& newPos);

  void
  setWorldPosition(const Vector3f& newPos);
#pragma endregion

#pragma region get/set Rotation
  Quaternion
  getLocalRotation() const;

  Vector3f
  getLocalEulerRotation() const;

  Quaternion
  getWorldRotation() const;

  Vector3f
  getWorldEulerRotation() const;

  void
  setLocalRotation(const Quaternion& newRotation);

  void
  setLocalEulerAngle(const Vector3f& newEulerAngles);

  void
  setWorldRotation(const Quaternion& newRotation);

  void
  setWorldEulerAngle(const Vector3f& newEulerAngles);
#pragma endregion

#pragma region get/set Scale
  Vector3f
  getLocalScale() const;

  Vector3f
  getWorldScale() const;

  void
  setLocalScale(const Vector3f& newScale);

  void
  setWorldScale(const Vector3f& newScale);
#pragma endregion

#pragma region get/set Transform
  Matrix4
  getLocalTransform() const;

  Matrix4
  getWorldTransform() const;

  void
  setLocalTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot);

  void
  setLocalTransform(const Matrix4& other);

  void
  setWorldTransform(const Vector3f& newPos, 
                    const Vector3f& newScale, 
                    const Quaternion& newRot = Quaternion::IDENTITY);

  void
  setWorldTransform(const Matrix4& other);
#pragma endregion

  void
  setLocalLookAt(const Vector3f& target,
                 const Vector3f& upDir);

  void
  setWorldLookAt(const Vector3f& target,
                 const Vector3f& upDir);

  void
  setLocalLookAt(const Vector3f& eyePos,
                 const Vector3f& target,
                 const Vector3f& upDir);

  void
  setWorldLookAt(const Vector3f& eyePos,
                 const Vector3f& target,
                 const Vector3f& upDir);

  bool
  getParentTransform(SPtr<CTransform>& out);

  WPtr<CTransform>
  getChildTransform(const uint32& index);

  void
  translateLocal(const Vector3f& delta) {
    m_localMatrix.translate(delta);
    
    updateWorld();
  }

  void 
  scaleLocal(const Vector3f& delta) {
    m_localMatrix.scale(delta);

    updateWorld();
  }

  void
  rotateLocal(const Quaternion& delta) {
    m_localMatrix.rotate(delta);

    updateWorld();
  }

  void
  rotateLocal(const Vector3f& deltaAngles) {
    m_localMatrix.rotate(Quaternion(Euler(deltaAngles)));

    updateWorld();
  }

  void
  resetTransform() {
    m_localMatrix.identity();

    updateWorld();
  }

#pragma region Transformations
  Vector3f
  transformPoint(const Vector3f& point) const;

  Vector3f
  transformDirection(const Vector3f& direction) const;

  Quaternion
  transformRotation(const Quaternion& rotation) const;

  Vector3f
  transformScale(const Vector3f& scale) const;
  
  Vector3f
  inverseTransformPoint(const Vector3f& point) const;
  
  Vector3f
  inverseTransformDirection(const Vector3f& direction) const;
  
  Quaternion
  inverseTransformRotation(const Quaternion& rotation) const;

  Vector3f
  inverseTransformScale(const Vector3f& scale) const;
#pragma endregion

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eTRANSFORM; }

  virtual const String 
  toString() override {
    String toReturn;

    toReturn = Utils::format("Local matrix:\n%s \n %s \n %s\nWorld matrix:\n%s \n %s \n %s\n",
                             m_localMatrix.getPosition().toString().c_str(), 
                             m_localMatrix.getEulerRotation().toString().c_str(),
                             m_localMatrix.getScale().toString().c_str(),
                             m_worldMatrix.getPosition().toString().c_str(), 
                             m_worldMatrix.getEulerRotation().toString().c_str(),
                             m_worldMatrix.getScale().toString().c_str());

    return toReturn;
  }

private:

  bool
  isRootSC(SPtr<CTransform> parent);

  bool
  isPartOfSceneSC(SPtr<CTransform> child);

  void
  updateWorld();

  void
  UpdateChildren();

public:

  Matrix4 m_localMatrix;
  Matrix4 m_worldMatrix;
};

}