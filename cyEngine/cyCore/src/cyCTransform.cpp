#include "cyCTransform.h"
#include "cyBBeing.h"
#include "cySceneManager.h"

namespace CYLLENE_SDK {

#pragma region get/set Position
Vector3f
CTransform::getLocalPosition() const {
  return m_localMatrix.getPosition();
}

Vector3f
CTransform::getWorldPosition() const {
  return m_worldMatrix.getPosition();
}

void
CTransform::setLocalPosition(const Vector3f& newPos) {
  m_localMatrix.setPosition(newPos);

  updateWorld();
}

void
CTransform::setWorldPosition(const Vector3f& newPos) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Vector3f relativePosition = parentTransform->inverseTransformPoint(newPos);
    setLocalPosition(relativePosition);
  }
  else {
    setLocalPosition(newPos);
  }
}
#pragma endregion

#pragma region get/set Rotation
Quaternion
CTransform::getLocalRotation() const {
  return m_localMatrix.getQuatRotation();
}

Vector3f
CTransform::getLocalEulerRotation() const {
  Euler e = m_localMatrix.getEulerRotation();
  return Vector3f(e.x, e.y, e.z);
}

Quaternion
CTransform::getWorldRotation() const {
  return m_worldMatrix.getQuatRotation();
}

Vector3f
CTransform::getWorldEulerRotation() const {
  Euler e = m_worldMatrix.getEulerRotation();
  return Vector3f(e.x, e.y, e.z);
}

void
CTransform::setLocalRotation(const Quaternion& newRotation) {
  m_localMatrix.setRotation(newRotation);
  
  updateWorld();
}

void
CTransform::setLocalEulerAngle(const Vector3f& newEulerAngles) {
  Euler e(newEulerAngles.x, newEulerAngles.y, newEulerAngles.z);
  m_localMatrix.setRotation(Quaternion(e));

  updateWorld();
}

void
CTransform::setWorldRotation(const Quaternion& rotation) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Quaternion relativeRotation = parentTransform->inverseTransformRotation(rotation);
    setLocalRotation(relativeRotation);
  }
  else {
    setLocalRotation(rotation);
  }
}

void
CTransform::setWorldEulerAngle(const Vector3f& rotation) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Quaternion relativeRotation = parentTransform->inverseTransformRotation(Quaternion(Euler(rotation)));
    Euler relativeEuler = relativeRotation.getEulerRotation();
    setLocalEulerAngle(Vector3f(relativeEuler.x, relativeEuler.y, relativeEuler.z));
  }
  else {
    setLocalEulerAngle(rotation);
  }
}
#pragma endregion

#pragma region get/set Scale
Vector3f
CTransform::getLocalScale() const {
  return m_localMatrix.getScale();
}

Vector3f
CTransform::getWorldScale() const {
  return m_worldMatrix.getScale();
}

void
CTransform::setLocalScale(const Vector3f& newScale) {
  m_localMatrix.setScale(newScale);

  updateWorld();
}

void
CTransform::setWorldScale(const Vector3f& newScale) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Vector3f relativeScale = parentTransform->inverseTransformScale(newScale);
    setLocalScale(relativeScale);
  }
  else {
    setLocalScale(newScale);
  }
}
#pragma endregion

#pragma region get/set Transform
Matrix4
CTransform::getLocalTransform() const {
  return m_localMatrix;
}

Matrix4 
CTransform::getWorldTransform() const {
  return m_worldMatrix;
}

void
CTransform::setLocalTransform(const Vector3f& newPos, const Vector3f& newSc, const Quaternion& newRot) {
  m_localMatrix.setTransformMatrix(newPos, newRot, newSc);

  updateWorld();
}

void
CTransform::setLocalTransform(const Matrix4& other) {
  m_localMatrix = other;

  updateWorld();
}

void
CTransform::setWorldTransform(const Vector3f& newPos, 
                              const Vector3f& newScale, 
                              const Quaternion& newRot) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Vector3f    relativePos    = parentTransform->inverseTransformPoint(newPos);
    Vector3f    relativeScale  = parentTransform->inverseTransformScale(newScale);
    Quaternion  relativeRot    = parentTransform->inverseTransformRotation(newRot);

    setLocalTransform(relativePos, relativeScale, relativeRot);
  }
  else {
    setLocalTransform(newPos, newScale, newRot);
  }
}

void
CTransform::setWorldTransform(const Matrix4& other) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Matrix4 relativeMatrix = parentTransform->m_worldMatrix.inversed() * other;
    setLocalTransform(relativeMatrix);
  }
  else {
    setLocalTransform(other);
  }
}
#pragma endregion

void
CTransform::setLocalLookAt(const Vector3f& targetPos, const Vector3f& upDir) {
  Vector3f eyePos = m_localMatrix.getPosition();

  m_localMatrix.setLookAt(eyePos, targetPos, upDir);

  updateWorld();
}

void
CTransform::setWorldLookAt(const Vector3f& targetPos, const Vector3f& upDir) {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    Vector3f relativeTarget = parentTransform->inverseTransformPoint(targetPos);
    Vector3f relativeUpDir = parentTransform->inverseTransformDirection(upDir);

    setLocalLookAt(relativeTarget, relativeUpDir);
  }
  else {
    setLocalLookAt(targetPos, upDir);
  }
}

bool
CTransform::getParentTransform(SPtr<CTransform>& out) {
  if (m_owner.expired()) {
    return false;
  }

  SPtr<BBeing> owner = m_owner.lock();
  if (owner->getParent().expired()) {
    return false;
  }

  SPtr<BBeing> parent = std::static_pointer_cast<BBeing>(owner->getParent().lock());
  if (parent->getTransform().expired()) {
    return false;
  }

  SPtr<CTransform> parentTransform = parent->getTransform().lock();
  if (parentTransform) {
    out = parentTransform;
    return true;
  }

  return false;
}

WPtr<CTransform>
CTransform::getChildTransform(const uint32& index) {
  CY_ASSERT(index < m_owner.lock()->getChildren().size(), "Index is out of bounds");
  return m_owner.lock()->getChildren()[index].lock()->getTransform();
}

#pragma region Transformations
Vector3f
CTransform::transformPoint(const Vector3f& point) const {
  return m_worldMatrix.transformPosition(point);
}

Vector3f
CTransform::transformDirection(const Vector3f& direction) const {
  return m_worldMatrix.transformDirection(direction);
}

Quaternion
CTransform::transformRotation(const Quaternion& rotation) const {
  return m_worldMatrix.getQuatRotation() * rotation;
}

Vector3f
CTransform::transformScale(const Vector3f& scale) const {
  return m_worldMatrix.getScale() * scale;
}

Vector3f
CTransform::inverseTransformPoint(const Vector3f& point) const {
  return m_worldMatrix.inversed().transformPosition(point);
}

Vector3f
CTransform::inverseTransformDirection(const Vector3f& direction) const {
  return m_worldMatrix.inversed().transformDirection(direction);
}

Quaternion
CTransform::inverseTransformRotation(const Quaternion& rotation) const {
  return m_worldMatrix.getQuatRotation().inversed() * rotation;
}

Vector3f
CTransform::inverseTransformScale(const Vector3f& scale) const {
  Vector3f parentScale = m_worldMatrix.getScale();
  CY_ASSERT(!Math::isNearSame(parentScale.x, 0.0f) &&
            !Math::isNearSame(parentScale.y, 0.0f) &&
            !Math::isNearSame(parentScale.z, 0.0f),
            "Scale cannot be zero for inverse transform scale");
  return scale / parentScale;
}
#pragma endregion

bool
CTransform::isRootSC(SPtr<CTransform> parent) {
  return SceneManager::instance().getActiveScene()->getRootNode().get() == parent->m_owner.lock().get();
}

bool
CTransform::isPartOfSceneSC(SPtr<CTransform> child) {
  return SceneManager::instance().findBeing<BBeing>(child->m_owner.lock()->getName()).lock() != nullptr;
}

void
CTransform::updateWorld() {
  SPtr<CTransform> parentTransform;

  if (getParentTransform(parentTransform)) {
    m_worldMatrix = parentTransform->m_worldMatrix * m_localMatrix;
  }
  else {
    m_worldMatrix = m_localMatrix;
  }

  UpdateChildren();
}

void
CTransform::UpdateChildren() {
  if (m_owner.expired()) {
    return;
  }

  SPtr<BBeing> owner = m_owner.lock();
  if (owner->getParent().expired()) {
    return;
  }

  for (WPtr<SNode> &childWeak : owner->getChildren()) {
    if (SPtr<SNode> child = childWeak.lock()) {
      if (SPtr<CTransform> childTransform = child->getTransform().lock()) {
        childTransform->updateWorld();
      }
    }
  }
}

}