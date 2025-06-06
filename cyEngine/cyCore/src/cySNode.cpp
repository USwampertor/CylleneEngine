#include "cySNode.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void 
SNode::addChild(SPtr<SNode> child, bool keepWorldTransform) {
  if (!child || child.get() == this) return;

  // Detach from existing parent
  if (auto oldParent = child->m_parent.lock()) {
    oldParent->removeChild(child);
  }

  // Add to this node's children
  m_children.push_back(child);
  child->m_parent = makeSharedPtr<SNode>(*this);

  // Optionally preserve world transform
  if (keepWorldTransform && child->getTransform()) {
    auto childTransform = child->getTransform();
    auto worldMatrix = childTransform->getWorldTransform();
    if (auto thisTransform = getTransform()) {
      worldMatrix = thisTransform->getWorldTransform().inversed() * worldMatrix;
    }
    childTransform->m_tMatrix = worldMatrix;
  }
}

void 
SNode::removeChild(SPtr<SNode> child, bool recursive) {
  auto it = std::find(m_children.begin(), m_children.end(), child);
  if (it != m_children.end()) {
    (*it)->m_parent.reset();
    m_children.erase(it);
  }

  // Recursively remove all descendants if requested
  if (recursive) {
    for (auto& grandchild : child->m_children) {
      child->removeChild(grandchild, true);
    }
  }
}

SPtr<SNode> 
SNode::findChild(const String& name, bool recursive) const {
  for (const auto& child : m_children) {
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

Vector<SPtr<SNode>> 
SNode::findChildren(Callback<bool, SPtr<SNode>> condition, bool recursive) const {
  Vector<SPtr<SNode>> results;
  for (const auto& child : m_children) {
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

bool 
SNode::isDescendantOf(const SPtr<SNode>& ancestor) const {
  auto parent = m_parent.lock();
  while (parent) {
    if (parent == ancestor) return true;
    parent = parent->m_parent.lock();
  }
  return false;
}

void 
SNode::setParent(SPtr<SNode> parent, bool keepWorldTransform) {
  if (parent) {
    parent->addChild(makeSharedPtr<SNode>(*this), keepWorldTransform);
  }
  else if (auto oldParent = m_parent.lock()) {
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