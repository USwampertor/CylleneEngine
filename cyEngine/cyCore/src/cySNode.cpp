#include "cySNode.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void 
SNode::addChild(SPtr<SNode> child, bool keepWorldTransform) {
  if (!child || child.get() == this) {
    CY_ASSERT(false, "Should not add a null transform or itself as a child");
    return;
  }

  // Detach from existing parent
  if (auto oldParent = child->m_parentNode.lock()) {
    oldParent->removeChild(child);
  }

  bool exists = std::any_of(m_childrenNodes.begin(), 
                            m_childrenNodes.end(), 
                            [&child](const SPtr<SNode>& reference) {
   
    return reference && reference == child;  // Compare ownership
  });

  if (!exists) {
    // Add to this node's children
    m_childrenNodes.push_back(child);
    child->m_parentNode = makeSharedPtr<SNode>(*this);
  }

  // Optionally preserve world transform
  if (child->getTransform()) {
    // auto childTransform = child->getTransform();
    // auto worldMatrix = childTransform->getWorldTransform();
    if (auto thisTransform = getTransform()) {
      // worldMatrix = thisTransform->getWorldTransform().inversed() * worldMatrix;
      thisTransform->addChild(child->getTransform());
      if (keepWorldTransform) {
        child->getTransform()->setWorldTransform(thisTransform->getWorldTransform().inversed() * 
                                                 child->getTransform()->getWorldTransform());
        child->getTransform()->updateLocalFromWorld();
      }
    }
    // childTransform->m_tMatrix = worldMatrix;
  }
}

void
SNode::addChildren(const Vector<SPtr<SNode>>& newChildren, bool keepWorldTransform /* = true */) {
  for (const auto& child : newChildren) {
    if (child && child.get() != this) {
      addChild(child, keepWorldTransform);
    }
  }
}

void
SNode::removeChild(const String& name) {

  for (int i = 0; i < m_childrenNodes.size(); ++i) {
    if (m_childrenNodes[i]->getName() == name) {
      
      // Optionally preserve world transform
      if (m_childrenNodes[i]->getTransform()) {
        // auto childTransform = child->getTransform();
        // auto worldMatrix = childTransform->getWorldTransform();
        if (auto thisTransform = getTransform()) {
          // worldMatrix = thisTransform->getWorldTransform().inversed() * worldMatrix;
          thisTransform->removeChild(name);
        }
        // childTransform->m_tMatrix = worldMatrix;
      }

      m_childrenNodes[i]->m_parentNode.reset();
      m_childrenNodes.erase(m_childrenNodes.begin() + i);
    }
  }
}

void 
SNode::removeChild(SPtr<SNode> child, bool recursive) {
  auto it = std::find(m_childrenNodes.begin(), m_childrenNodes.end(), child);
  if (it != m_childrenNodes.end()) {
    // Recursively remove all descendants if requested
    if (recursive) {
      for (auto& grandchild : child->m_childrenNodes) {
        child->removeChild(grandchild, true);
      }
    }
    (*it)->m_parentNode.reset();
    m_childrenNodes.erase(it);
  }
}

void
SNode::removeAllChildren() {
  for (auto& child : m_childrenNodes) {
    child->m_parentNode.reset();
  }
  m_childrenNodes.clear();
}

SPtr<SNode> 
SNode::findChild(const String& name, bool recursive) const {
  for (const auto& child : m_childrenNodes) {
    if (child->m_nodeName == name) {
      return child;
    }
    if (recursive) {
      auto found = child->findChild(name, true);
      if (found) return found;
    }
  }
  return nullptr;
}

SPtr<SNode>
SNode::findChildWhere(Callback<bool, SPtr<SNode>> condition, bool recursive) const {
  for (const auto& child : m_childrenNodes) {
    if (condition(child)) {
      return child;
    }

    if (recursive) {
      auto found = child->findChildWhere(condition, true);
      if (found) return found;
    }
  }

  return nullptr;
}

Vector<SPtr<SNode>> 
SNode::findChildren(Callback<bool, SPtr<SNode>> condition, bool recursive) const {
  Vector<SPtr<SNode>> results;
  for (const auto& child : m_childrenNodes) {
    if (condition(child)) {
      results.push_back(child);
    }
    if (recursive) {
      auto nested = child->findChildren(condition, true);
      results.insert(results.end(), nested.begin(), nested.end());
    }
  }
  return results;
}

void
SNode::setParent(const SPtr<SNode>& newParent) {
  if (newParent) {
    newParent->addChild(makeSharedPtr<SNode>(*this));
  }
  else if (auto oldParent = m_parentNode.lock()) {
    oldParent->removeChild(makeSharedPtr<SNode>(*this));
  }
}

bool
SNode::isChildOf(const SPtr<SNode>& potentialParent) const {
  if (!potentialParent) {
    return false;
  }

  SPtr<SNode> parent = m_parentNode.lock();

  if (!parent) {
    return false;
  }

  return parent == potentialParent;
}

bool
SNode::isParentOf(const SPtr<SNode>& potentialChild) {
  if (!potentialChild) {
    return false;
  }

  WPtr<SNode> parent = potentialChild->getParent();

  if (parent.expired()) {
    return false;
  }

  return parent.lock().get() == this;
}

bool 
SNode::isDescendantOf(const SPtr<SNode>& ancestor) const {
  auto parent = m_parentNode.lock();
  while (parent) {
    if (parent == ancestor) return true;
    parent = parent->m_parentNode.lock();
  }
  return false;
}

void 
SNode::setParent(SPtr<SNode> parent, bool keepWorldTransform) {
  if (parent) {
    parent->addChild(makeSharedPtr<SNode>(*this), keepWorldTransform);
  }
  else if (auto oldParent = m_parentNode.lock()) {
    oldParent->removeChild(makeSharedPtr<SNode>(*this));
  }
}

// Matrix4 SNode::getWorldTransform() const {
//   if (auto transform = getTransform()) {
//     return transform->GetWorldMatrix();
//   }
//   return Matrix4(1.0f); // Identity matrix if no transform
// }

}