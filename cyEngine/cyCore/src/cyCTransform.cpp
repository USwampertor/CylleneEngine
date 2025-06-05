#include "cyCTransform.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

Vector3f
CTransform::getWorldPosition() {
  if (auto parent = m_parent.lock()) {
    return parent->getWorldTransform().transformPosition(m_tMatrix.getPosition());
  }
  return m_tMatrix.getPosition();
}

void
CTransform::setWorldPosition(const Vector3f& newPos) {
  if (auto parent = m_parent.lock()) {
    Vector3f localPos = parent->getWorldTransform().inversed().transformPosition(newPos);
    m_tMatrix.setPosition(localPos);
  }
  else {
    m_tMatrix.setPosition(newPos);
  }
}

Vector3f
CTransform::getWorldScale() {
  Vector3f worldScale = m_tMatrix.getScale();
  if (auto parent = m_parent.lock()) {
    Vector3f parentScale = parent->getWorldScale();
    worldScale.x *= parentScale.x;
    worldScale.y *= parentScale.y;
    worldScale.z *= parentScale.z;
  }
  return worldScale;
}

void
CTransform::setWorldScale(const Vector3f& newScale) {
  if (auto parent = m_parent.lock()) {
    Vector3f parentScale = parent->getWorldScale();
    Vector3f localScale(newScale.x / parentScale.x,
                        newScale.y / parentScale.y,
                        newScale.z / parentScale.z);
    m_tMatrix.setScale(localScale);
  }
  else {
    m_tMatrix.setScale(newScale);
  }
}

Quaternion 
CTransform::getWorldRotation() {
  if (auto parent = m_parent.lock()) {
    return parent->getWorldRotation() * m_tMatrix.getQuatRotation();
  }
  return m_tMatrix.getQuatRotation();
}

void 
CTransform::setWorldRotation(const Quaternion& rotation) {
  if (auto parent = m_parent.lock()) {
    Quaternion localRot = parent->getWorldRotation().inversed() * rotation;
    m_tMatrix.setRotation(localRot);
  }
  else {
    m_tMatrix.setRotation(rotation);
  }
}

Vector3f
CTransform::getWorldEulerRotation() {
  if (auto parent = m_parent.lock()) {
    Quaternion q = parent->getWorldRotation() * m_tMatrix.getQuatRotation();
    Euler e = q.getEulerRotation();
    return Vector3f(e.x, e.y, e.z);
  }
  Euler e = m_tMatrix.getEulerRotation();
  return Vector3f(e.x, e.y, e.z);
}

void
CTransform::setWorldEulerAngle(const Vector3f& rotation) {
  if (auto parent = m_parent.lock()) {
    Quaternion r = Quaternion(Euler(rotation));
    Quaternion localRot = parent->getWorldRotation().inversed() * r;
    m_tMatrix.setRotation(localRot);
  }
  else {
    m_tMatrix.setRotation(rotation);
  }
}

Matrix4 
CTransform::getWorldTransform() {
  if (auto parent = m_parent.lock()) {
    return parent->getWorldTransform() * m_tMatrix;
  }
  return m_tMatrix;
}

void
CTransform::setWorldTransform(const Vector3f& newPos, 
                              const Vector3f& newScale, 
                              const Quaternion& newRot /*= Quaternion::IDENTITY*/) {
  // If we have no parent, just set the local transform directly
  if (auto parent = m_parent.lock()) {
    // Get parent's world matrix and its inverse
    Matrix4 parentWorld = parent->getWorldTransform();
    Matrix4 invParentWorld = parentWorld.inversed();

    // Calculate local position
    const Vector3f localPos = invParentWorld.transformPosition(newPos);

    // Calculate local rotation
    const Quaternion parentRot = parent->getWorldRotation();
    const Quaternion localRot = parentRot.inversed() * newRot;

    // Calculate local scale
    const Vector3f parentScale = parent->getWorldScale();
    Vector3f localScale(newScale.x / parentScale.x,
                        newScale.y / parentScale.y,
                        newScale.z / parentScale.z);

    // Set the local transform
    m_tMatrix.setTransformMatrix(localPos, localRot, localScale);
  }
  else {
    // No parent, just set directly
    m_tMatrix.setTransformMatrix(newPos, newRot, newScale);
  }
}

void
CTransform::setWorldTransform(const Matrix4& other) {
  // If we have a parent, convert to local space
  if (auto parent = m_parent.lock()) {
    Matrix4 parentWorld = parent->getWorldTransform();
    m_tMatrix = parentWorld.inversed() * other;
  }
  else {
    m_tMatrix = other;
  }
}

void 
CTransform::setLookAt(const Vector3f& target, const Vector3f& up) {
  Vector3f worldPos = getWorldPosition();
  m_tMatrix.setLookAt(worldPos, target, up);
  updateLocalFromWorld();
}

void
CTransform::setWorldLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  m_tMatrix.view(eyePos, targetPos, upDir);
  updateLocalFromWorld();
}

void
CTransform::setLocalLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  m_tMatrix.view(eyePos, targetPos, upDir);
}

void CTransform::updateLocalFromWorld()
{
  if (auto parent = m_parent.lock()) {
    Matrix4 parentWorld = parent->getWorldTransform();
    Matrix4 worldMatrix = getWorldTransform();
    Matrix4 localMatrix = parentWorld.inversed() * worldMatrix;

    m_tMatrix.setPosition(localMatrix.getPosition());
    m_tMatrix.setRotation(localMatrix.getQuatRotation());
    m_tMatrix.setScale(localMatrix.getScale());
  }
}

void
CTransform::attachChildren(const SPtr<CTransform>& newChild) {
  bool exists = std::any_of(m_children.begin(), 
                            m_children.end(), 
                            [&newChild](const WPtr<CTransform>& weak) {
    auto strong = weak.lock();
    return strong && strong == newChild;  // Compare ownership
  });

  if (!exists) {
    m_children.emplace_back(newChild);  // Add as weak_ptr
  }
}

void
CTransform::removeChildren(const String& name) {
  for (int i = 0; i < m_children.size(); ++i) {
    if (m_children[i].lock()->m_owner->getName() == name) {
      m_children.erase(m_children.begin() + i);
      break;
    }
  }
}

void
CTransform::removeChildrenAt(const uint32& index) {
  CY_ASSERT(index < m_children.size());
  m_children.erase(m_children.begin() + index);

}

WPtr<CTransform>
CTransform::getChild(const String& name) {
  for (int i = 0; i < m_children.size(); ++i) {
    if (m_children[i].lock()->m_owner->getName() == name) {
      return m_children[i];
    }
  }
}

void
CTransform::setParent(const SPtr<CTransform>& newParent) {
  // m_parent.reset();
  // m_parent = { newParent };


  if (newParent.get() == this) {
    CY_ASSERT(false, "Cannot parent a transform to itself");
    return;
  }

  // Check if we're already parented to this transform
  if (auto currentParent = m_parent.lock()) {
    if (currentParent == newParent) {
      return;
    }
  }

  // Remove from current parent
  if (auto oldParent = m_parent.lock()) {
    oldParent->removeChildren(m_owner->getName());
  }

  // Set new parent
  m_parent = newParent;

  // Add to new parent's children if valid
  if (newParent) {
    newParent->attachChildren(makeSharedPtr<CTransform>(*this));

    // Update local transform to maintain world position
    updateLocalFromWorld();
  }

}


bool 
CTransform::isChildOf(const SPtr<CTransform>& potentialParent) {
  if (!potentialParent) { return false; }
  if (auto parent = m_parent.lock()) {
    return parent == potentialParent || parent->isChildOf(potentialParent);
  }
  return false;
}

bool 
CTransform::isParentOf(const SPtr<CTransform>& potentialChild) {
  if (!potentialChild) { return false; }
  return potentialChild->isChildOf(makeSharedPtr<CTransform>(*this));
}

SPtr<CTransform> 
CTransform::findChild(const String& name, bool recursive) {
  for (const auto& childWeak : m_children) {
    if (auto child = childWeak.lock()) {
      if (child->m_owner->getName() == name) {
        return child;
      }

      if (recursive) {
        if (auto found = child->findChild(name, true)) {
          return found;
        }
      }
    }
  }
  return nullptr;
}


Vector3f CTransform::transformPoint(const Vector3f& point) {
  return getWorldTransform().transformPosition(point);
}

Vector3f CTransform::transformDirection(const Vector3f& direction) {
  return getWorldTransform().transformDirection(direction);
}

Vector3f CTransform::inverseTransformPoint(const Vector3f& point) {
  return getWorldTransform().inversed().transformPosition(point);
}

Vector3f CTransform::inverseTransformDirection(const Vector3f& direction) {
  return getWorldTransform().inversed().transformDirection(direction);
}

void CTransform::forEachChild(Callback<void, SPtr<CTransform>> callback, bool recursive) {
  for (auto& childWeak : m_children) {
    if (auto child = childWeak.lock()) {
      callback(child);
      if (recursive) {
        child->forEachChild(callback, true);
      }
    }
  }
}

}