#include "cySNode.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void 
SNode::addChild(WPtr<SNode> child, bool keepWorldTransform) {
  if (child.expired()) {
    CY_ASSERT(false && "Child is invalid.");
    return;
  }

  SPtr<SNode> childPtr = child.lock();

  if (!childPtr || childPtr.get() == this) {
    CY_ASSERT(false && "Should not add a null transform or itself as a child");
    return;
  }

  // Detach from existing parent
  if (SPtr<SNode> oldParent = childPtr->m_parentNode.lock()) {
    oldParent->removeChild(child);
  }

  bool exists = std::any_of(m_childrenNodes.begin(), 
                            m_childrenNodes.end(), 
                            [&childPtr](const WPtr<SNode>& reference) {
    return reference.lock() && reference.lock() == childPtr;  // Compare ownership
  });

  if (!exists) {
    // Add to this node's children
    m_childrenNodes.push_back(child);
    childPtr->m_parentNode = m_self;
  }

  // Next steps should be handled by Being override
  // Optionally preserve world transform
  WPtr<CTransform> thisTransform = getTransform();
  WPtr<CTransform> childTransform = childPtr->getTransform();

  if (!childTransform.expired() && !thisTransform.expired()) {
    if (keepWorldTransform) {
      SPtr<CTransform> childTransformPtr = childTransform.lock();
      SPtr<CTransform> thisTransformPtr = thisTransform.lock();

      childTransformPtr->setLocalTransform(thisTransformPtr->getWorldTransform().inversed() *
                                           childTransformPtr->getWorldTransform());
      // childTransformPtr->updateLocalFromWorld();
    }
    // childTransform->m_tMatrix = worldMatrix;
  }
}

void
SNode::addChildren(const Vector<WPtr<SNode>>& newChildren, bool keepWorldTransform /* = true */) {
  for (const auto& child : newChildren) {
    if (child.lock() && child.lock().get() != this) {
      addChild(child, keepWorldTransform);
    }
  }
}

void
SNode::removeChild(const String& name) {

  for (int i = 0; i < m_childrenNodes.size(); ++i) {
    if (m_childrenNodes[i].lock()->getName() == name) {
      
      // Optionally preserve world transform
      if (m_childrenNodes[i].lock()->getTransform().lock()) {
        // auto childTransform = child->getTransform();
        // auto worldMatrix = childTransform->getWorldTransform();
        WPtr<CTransform> thisTransform = getTransform();
        if (thisTransform.lock()) {
          // worldMatrix = thisTransform->getWorldTransform().inversed() * worldMatrix;
          // thisTransform->removeChild(name);
        }
        // childTransform->m_tMatrix = worldMatrix;
      }

      m_childrenNodes[i].lock()->m_parentNode.reset();
      m_childrenNodes.erase(m_childrenNodes.begin() + i);
    }
  }
}

void 
SNode::removeChild(WPtr<SNode> child, bool recursive) {
  
  for (int i = 0; i < m_childrenNodes.size(); ++i) {
    // If found the child node
    if (m_childrenNodes[i].lock().get() == child.lock().get()) {
      if (recursive) {
        for (auto& grandchild : child.lock()->m_childrenNodes) {
          child.lock()->removeChild(grandchild, true);
        }
      }
      m_childrenNodes[i].lock()->m_parentNode.reset();
      m_childrenNodes.erase(m_childrenNodes.begin() + i);
      break;
    }
  }
  
}

void
SNode::removeAllChildren() {
  for (auto& child : m_childrenNodes) {
    child.lock()->m_parentNode.reset();
  }
  m_childrenNodes.clear();
}

//////////////////////////////////////////////////////////////////////////

WPtr<SNode> 
SNode::findChild(const String& name, bool recursive) const {
  for (const auto& child : m_childrenNodes) {
    if (child.lock()->m_nodeName == name) {
      return child;
    }
    if (recursive) {
      auto found = child.lock()->findChild(name, true);
      if (found.lock()) return found;
    }
  }
  return {};
}

WPtr<SNode>
SNode::findChildWhere(Callback<bool, WPtr<SNode>> condition, bool recursive) const {
  for (const auto& child : m_childrenNodes) {
    if (condition(child)) {
      return child;
    }

    if (recursive) {
      auto found = child.lock()->findChildWhere(condition, true);
      if (found.lock()) return found;
    }
  }

  return {};
}

Vector<WPtr<SNode>>
SNode::findChildren(Callback<bool, WPtr<SNode>> condition, bool recursive) const {
  Vector<WPtr<SNode>> results;
  for (const auto& child : m_childrenNodes) {
    if (condition(child)) {
      results.push_back(child);
    }
    if (recursive) {
      auto nested = child.lock()->findChildren(condition, true);
      results.insert(results.end(), nested.begin(), nested.end());
    }
  }
  return results;
}

bool
SNode::isChildOf(const WPtr<SNode>& potentialParent) const {
  if (!potentialParent.lock()) {
    return false;
  }

  SPtr<SNode> parent = m_parentNode.lock();

  if (!parent) {
    return false;
  }

  return parent == potentialParent.lock();
}

bool
SNode::isParentOf(const WPtr<SNode>& potentialChild) {
  if (!potentialChild.lock()) {
    return false;
  }

  WPtr<SNode> parent = potentialChild.lock()->getParent();

  if (parent.expired()) {
    return false;
  }

  return parent.lock().get() == this;
}

bool 
SNode::isDescendantOf(const WPtr<SNode>& ancestor) const {
  auto parent = m_parentNode.lock();
  while (parent) {
    if (parent == ancestor.lock()) { return true; }
    parent = parent->m_parentNode.lock();
  }
  return false;
}

void 
SNode::setParent(const WPtr<SNode>& parent, bool keepWorldTransform) {
  if (parent.lock()) {
    parent.lock()->addChild(m_self, keepWorldTransform);
  }
  else if (auto oldParent = m_parentNode.lock()) {
    oldParent->removeChild(m_self);
  }
}

// Matrix4 SNode::getWorldTransform() const {
//   if (auto transform = getTransform()) {
//     return transform->GetWorldMatrix();
//   }
//   return Matrix4(1.0f); // Identity matrix if no transform
// }

}