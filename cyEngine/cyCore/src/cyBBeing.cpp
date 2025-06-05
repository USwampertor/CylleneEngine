#include "cyBBeing.h"

namespace CYLLENE_SDK {

void
BBeing::removeAllComponents() {
  // Remove all components and clear the map
  for (auto it = m_components.begin(); it != m_components.end();) {
    it->second->setOwner(nullptr); // Clear owner reference
  }
  m_components.clear();
}

void 
BBeing::addChild(SPtr<SNode> child) {
  SNode::addChild(child);

  // Sync transform hierarchy if both have transforms
  if (auto myTransform = getTransform()) {
    if (auto childBeing = std::static_pointer_cast<BBeing>(child)) {
      if (auto childTransform = childBeing->getTransform()) {
        childTransform->setParent(myTransform);
      }
    }
  }
}

void 
BBeing::removeChild(SPtr<SNode> child) {
  // Sync transform hierarchy if both have transforms
  if (auto childBeing = std::static_pointer_cast<BBeing>(child)) {
    if (auto childTransform = childBeing->getTransform()) {
      childTransform->setParent(nullptr);
    }
  }

  SNode::removeChild(child);
}


SPtr<SNode> BBeing::findChild(const String& name, bool recursive) const {
  // First check if any child is a BBeing with matching name
  for (const auto& child : m_children) {
    if (auto being = std::static_pointer_cast<BBeing>(child)) {
      if (being->getName() == name) {
        return child;
      }
    }
  }

  // Fall back to recursive search if needed
  return recursive ? SNode::findChild(name, true) : nullptr;
}

SPtr<BBeing> 
BBeing::createChild(const String& name) {
  auto child = makeSharedPtr<BBeing>(name);
  addChild(child);

  // Ensure child has transform component
  child->createComponent<CTransform>();

  return child;
}

}