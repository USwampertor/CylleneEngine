#include "cyCTransform.h"
#include "cyBBeing.h"
#include "cySceneManager.h"

namespace CYLLENE_SDK {

Vector3f
CTransform::getWorldPosition() {
  // If we can lock, either we have a parent or we are not root
  if (auto parent = m_owner.lock()->getParent().lock()) {
    if (parent->getTransform().lock()) {
      return parent->getTransform().lock()->getWorldTransform().transformPosition(m_tMatrix.getPosition());
    }
  }
  return m_tMatrix.getPosition();
}

void
CTransform::setWorldPosition(const Vector3f& newPos) {
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Vector3f localPos = parent->getTransform().lock()->getWorldTransform().inversed().transformPosition(newPos);
    m_tMatrix.setPosition(localPos);
  }
  else {
    m_tMatrix.setPosition(newPos);
  }
}

Vector3f
CTransform::getWorldScale() {
  Vector3f worldScale = m_tMatrix.getScale();
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Vector3f parentScale = parent->getTransform().lock()->getWorldScale();
    worldScale.x *= parentScale.x;
    worldScale.y *= parentScale.y;
    worldScale.z *= parentScale.z;
  }
  return worldScale;
}

void
CTransform::setWorldScale(const Vector3f& newScale) {
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Vector3f parentScale = parent->getTransform().lock()->getWorldScale();
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
  if (auto parent = m_owner.lock()->getParent().lock()) {
    return parent->getTransform().lock()->getWorldRotation() * m_tMatrix.getQuatRotation();
  }
  return m_tMatrix.getQuatRotation();
}

void 
CTransform::setWorldRotation(const Quaternion& rotation) {
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Quaternion localRot = parent->getTransform().lock()->getWorldRotation().inversed() * rotation;
    m_tMatrix.setRotation(localRot);
  }
  else {
    m_tMatrix.setRotation(rotation);
  }
}

Vector3f
CTransform::getWorldEulerRotation() {
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Quaternion q = parent->getTransform().lock()->getWorldRotation() * m_tMatrix.getQuatRotation();
    Euler e = q.getEulerRotation();
    return Vector3f(e.x, e.y, e.z);
  }
  Euler e = m_tMatrix.getEulerRotation();
  return Vector3f(e.x, e.y, e.z);
}

void
CTransform::setWorldEulerAngle(const Vector3f& rotation) {
  if (auto parent = m_owner.lock()->getParent().lock()) {
    Quaternion r = Quaternion(Euler(rotation));
    Quaternion localRot = parent->getTransform().lock()->getWorldRotation().inversed() * r;
    m_tMatrix.setRotation(localRot);
  }
  else {
    m_tMatrix.setRotation(rotation);
  }
}

Matrix4 
CTransform::getWorldTransform() {
  if (m_owner.expired()) {
    return m_tMatrix;
  }

  SPtr<BBeing> owner = m_owner.lock();
  if (owner->getParent().expired()) {
    return m_tMatrix;
  }

  SPtr<BBeing> parent = std::static_pointer_cast<BBeing>(owner->getParent().lock());
  if (parent->getTransform().expired()) {
    return m_tMatrix;
  }

  SPtr<CTransform> parentTransform = parent->getTransform().lock();
  if (parentTransform) {
    return parentTransform->getWorldTransform() * m_tMatrix;
  }

  return m_tMatrix;
}

void
CTransform::setWorldTransform(const Vector3f& newPos, 
                              const Vector3f& newScale, 
                              const Quaternion& newRot /*= Quaternion::IDENTITY*/) {
  // If we have no parent, just set the local transform directly
  if (auto parent = m_owner.lock()->getParent().lock()) {
    // Get parent's world matrix and its inverse
    Matrix4 parentWorld = parent->getTransform().lock()->getWorldTransform();
    Matrix4 invParentWorld = parentWorld.inversed();

    // Calculate local position
    const Vector3f localPos = invParentWorld.transformPosition(newPos);

    // Calculate local rotation
    const Quaternion parentRot = parent->getTransform().lock()->getWorldRotation();
    const Quaternion localRot = parentRot.inversed() * newRot;

    // Calculate local scale
    const Vector3f parentScale = parent->getTransform().lock()->getWorldScale();
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
  if (m_owner.expired()) {
    m_tMatrix = other;
    return;
  }

  SPtr<BBeing> owner = m_owner.lock();
  if (owner->getParent().expired()) {
    m_tMatrix = other;
    return;
  }

  SPtr<BBeing> parent = std::static_pointer_cast<BBeing>(owner->getParent().lock());
  if (parent->getTransform().expired()) {
    m_tMatrix = other;
    return;
  }

  SPtr<CTransform> parentTransform = parent->getTransform().lock();
  if (parentTransform) {
    Matrix4 parentWorld = parentTransform->getWorldTransform();
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

void
CTransform::updateLocalFromWorld() {
  if (m_owner.expired()) {
    return;
  }

  SPtr<BBeing> owner = m_owner.lock();
  if (owner->getParent().expired()) {
    return;
  }

  SPtr<BBeing> parent = std::static_pointer_cast<BBeing>(owner->getParent().lock());
  if (parent->getTransform().expired()) {
    return;
  }

  SPtr<CTransform> parentTransform = parent->getTransform().lock();
  if (parentTransform) {
    Matrix4 parentWorld = parentTransform->getWorldTransform();
    Matrix4 worldMatrix = getWorldTransform();
    Matrix4 localMatrix = parentWorld.inversed() * worldMatrix;

    m_tMatrix = localMatrix;
  }
}

WPtr<CTransform>
CTransform::getParentTransform() {
  // TODO: Check if parent is still valid
  return m_owner.lock()->getParent().lock()->getTransform();
}

WPtr<CTransform>
CTransform::getChildTransform(const uint32& index) {
  CY_ASSERT(index < m_owner.lock()->getChildren().size(), "Index is less than 0");
  return m_owner.lock()->getChildren()[index].lock()->getTransform();
}

// void
// CTransform::addChild(const SPtr<CTransform>& newChild) {
// 
//   if (!newChild || newChild.get() == this) {
//     CY_ASSERT(false, "Should not add a null transform or itself as a child");
//     return;
//   }
// 
//   bool exists = std::any_of(m_children.begin(), 
//                             m_children.end(), 
//                             [&newChild](const WPtr<CTransform>& weak) {
//     auto strong = weak.lock();
//     return strong && strong == newChild;  // Compare ownership
//   });
// 
//   if (!exists) {
//     m_children.emplace_back(newChild);  // Add as weak_ptr
//     newChild->m_parent = makeSharedPtr<CTransform>(*this);
//   }
// }
// 
// void
// CTransform::addChildren(const Vector<SPtr<CTransform>>& newChildren) {
//   for (const auto& child : newChildren) {
//     if (child && child.get() != this) {  // Avoid self-attachment
//       addChild(child);
//     }
//   }
// }
// 
// void
// CTransform::removeChild(const String& name) {
// 
//   if (!isRootSC(makeSharedPtr<CTransform>(*this))) {
//     // This is not root, so we can detach the child
//     for (int i = 0; i < m_children.size(); ++i) {
//       if (m_children[i].lock()->m_owner->getName() == name) {
//         if (SPtr<CTransform> child = m_children[i].lock()) {
//           if (isPartOfSceneSC(child)) {
//             child->setParent(SceneManager::instance().getActiveScene()->getRootNode()->getTransform());
//           }
//           else {
//             m_children[i].lock()->m_parent.reset();
//           }
//         }
//         m_children.erase(m_children.begin() + i);
//         break;
//       }
//     }
//   }
// }
// 
// void
// CTransform::removeChildAt(const uint32& index) {
//   CY_ASSERT(index < m_children.size());
//   if (!isRootSC(makeSharedPtr<CTransform>(*this))) {
//     // This is not root, so we can detach the child
//     
//     if (SPtr<CTransform> child = m_children[index].lock()) {
//       if (isPartOfSceneSC(child)) {
//         child->setParent(SceneManager::instance().getActiveScene()->getRootNode()->getTransform());
//       }
//       else {
//         m_children[index].lock()->m_parent.reset();
//       }
//     }
//     m_children.erase(m_children.begin() + index);
//   }
// }
// 
// void
// CTransform::removeAllChildren() {
//   if (!isRootSC(makeSharedPtr<CTransform>(*this))) {
//     for (auto& childWeak : m_children) {
//       if (auto child = childWeak.lock()) {
//         // This is not root, so we can detach the child
//         if (isPartOfSceneSC(child)) {
//           child->setParent(SceneManager::instance().getActiveScene()->getRootNode()->getTransform());
//         }
//         else {
//          child->m_parent.reset();
//         }
//       }
//     }
//     m_children.clear();
//   }
// }

bool
CTransform::isRootSC(SPtr<CTransform> parent) {
  return SceneManager::instance().getActiveScene()->getRootNode().get() == parent->m_owner.lock().get();
}

bool
CTransform::isPartOfSceneSC(SPtr<CTransform> child) {
  return SceneManager::instance().findBeing<BBeing>(child->m_owner.lock()->getName()).lock() != nullptr;
}

// WPtr<CTransform>
// CTransform::getChild(const String& name) {
//   for (int i = 0; i < m_children.size(); ++i) {
//     if (m_children[i].lock()->m_owner->getName() == name) {
//       return m_children[i];
//     }
//   }
// }
// 
// void
// CTransform::setParent(const SPtr<CTransform>& newParent) {
// 
//   if (newParent.get() == this) {
//     CY_ASSERT(false, "Cannot parent a transform to itself");
//     return;
//   }
// 
//   // Check if we're already parented to this transform
//   if (auto currentParent = m_parent.lock()) {
//     if (currentParent == newParent) {
//       return;
//     }
//   }
// 
//   // Remove from current parent
//   if (auto oldParent = m_parent.lock()) {
//     oldParent->removeChild(m_owner->getName());
//   }
// 
//   // Set new parent
//   m_parent = newParent;
// 
//   // Add to new parent's children if valid
//   if (newParent) {
//     newParent->addChild(makeSharedPtr<CTransform>(*this));
// 
//     // Update local transform to maintain world position
//     updateLocalFromWorld();
//   }
// }
// 
// 
// bool 
// CTransform::isChildOf(const SPtr<CTransform>& potentialParent) {
//   if (!potentialParent) { return false; }
//   if (auto parent = m_parent.lock()) {
//     return parent == potentialParent || parent->isChildOf(potentialParent);
//   }
//   return false;
// }
// 
// bool 
// CTransform::isParentOf(const SPtr<CTransform>& potentialChild) {
//   if (!potentialChild) { return false; }
//   return potentialChild->isChildOf(makeSharedPtr<CTransform>(*this));
// }
// 
// SPtr<CTransform> 
// CTransform::findChild(const String& name, bool recursive) {
//   for (const auto& childWeak : m_children) {
//     if (auto child = childWeak.lock()) {
//       if (child->m_owner->getName() == name) {
//         return child;
//       }
// 
//       if (recursive) {
//         if (auto found = child->findChild(name, true)) {
//           return found;
//         }
//       }
//     }
//   }
//   return nullptr;
// }


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

// void CTransform::forEachChild(Callback<void, SPtr<CTransform>> callback, bool recursive) {
//   for (auto& childWeak : m_children) {
//     if (auto child = childWeak.lock()) {
//       callback(child);
//       if (recursive) {
//         child->forEachChild(callback, true);
//       }
//     }
//   }
// }

}