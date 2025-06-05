#include "cySceneNode.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void SNode::addChild(SPtr<SNode> child) {
  if (child && child.get() != this) {
    m_children.push_back(child);
    child->setParent(makeSharedPtr<SNode>(*this));
  }
}

void SNode::removeChild(SPtr<SNode> child) {
  auto it = std::remove_if(m_children.begin(), m_children.end(),
    [&child](const SPtr<SNode>& node) { return node == child; });
  m_children.erase(it, m_children.end());

  if (child) {
    child->setParent(nullptr);
  }
}

SPtr<SNode> SNode::findChild(const String& name, bool recursive) const {
  for (const auto& child : m_children) {
    if (auto being = std::static_pointer_cast<BBeing>(child)) {
      if (being->getName() == name) {
        return child;
      }
    }

    if (recursive) {
      if (auto found = child->findChild(name, true)) {
        return found;
      }
    }
  }
  return nullptr;
}

}