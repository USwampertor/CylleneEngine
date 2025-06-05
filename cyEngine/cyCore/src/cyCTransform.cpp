#include "cyCTransform.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void
CTransform::setLocalLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  m_tMatrix.view(eyePos, targetPos, upDir);
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
    newParent->attachChildren(makeSharedPtr<CTransform>(this));

    // Update local transform to maintain world position
    updateLocalFromWorld();
  }

}

void 
CTransform::updateLocalFromWorld()
{
  if (auto parent = m_parent.lock()) {
    Matrix4 parentWorld = parent->getTr();
    Matrix4 worldMatrix = getWorldMatrix();
    Matrix4 localMatrix = parentWorld.inversed() * worldMatrix;

    m_tMatrix.setPosition(localMatrix.getPosition());
    m_tMatrix.setRotation(localMatrix.getQuatRotation());
    m_tMatrix.setScale(localMatrix.getScale());
  }
}


}