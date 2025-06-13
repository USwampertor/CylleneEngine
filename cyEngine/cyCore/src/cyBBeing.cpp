#include "cyBBeing.h"
#include "cySceneManager.h"

namespace CYLLENE_SDK {

void
BBeing::removeAllComponents() {
  // Remove all components and clear the map
  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
    it->second->onDestroy(); // Clear owner reference
  }
  m_components.clear();
}

// void 
// BBeing::addChild(SPtr<SNode> child, bool keepWorldTransform) {
//   SNode::addChild(child, keepWorldTransform);
// 
//   // Sync transform hierarchy if both have transforms
//   if (auto myTransform = getTransform()) {
//     if (auto childBeing = std::static_pointer_cast<BBeing>(child)) {
//       // childBeing->onInit(); // Initialize the child Being
//       // if (auto childTransform = childBeing->getTransform()) {
//       //   childTransform->setParent(myTransform);
//       // }
//     }
//   }
// }
// 
// void BBeing::removeChild(SPtr<SNode> child, bool recursive) {
//   // 1. Check if child exists in this Being's children
//   auto it = std::find(m_childrenNodes.begin(), m_childrenNodes.end(), child);
//   if (it == m_childrenNodes.end()) return;
// 
//   // 2. Handle recursive removal (if requested)
//   if (recursive) {
//     for (auto& grandchild : child->getChildren()) {
//       child->removeChild(grandchild, true);
//     }
//   }
// 
//   // 3. Special handling for BBeing children
//   if (auto beingChild = std::static_pointer_cast<BBeing>(child)) {
//     // Notify components (e.g., physics cleanup)
//     for (auto& component : beingChild->m_components) {
//       component.second->onDestroy();
//     }
//   }
// 
//   // 4. Detach from hierarchy
//   child->m_parentNode.reset();
//   m_childrenNodes.erase(it);
// 
//   // 5. Mark for destruction if it's a BBeing
//   if (auto beingChild = std::static_pointer_cast<BBeing>(child)) {
//     beingChild->markToDestroy();
//   }
// }
// 
// 
// SPtr<SNode> 
// BBeing::findChild(const String& name, bool recursive) const {
//   // First check direct children
//   auto result = SNode::findChild(name, false);
//   if (result) return result;
// 
//   // Recursive search if enabled
//   if (recursive) {
//     for (const auto& child : m_childrenNodes) {
//       if (auto being = std::static_pointer_cast<BBeing>(child)) {
//         result = being->findChild(name, true);
//         if (result) return result;
//       }
//     }
//   }
//   return nullptr;
// }

WPtr<BBeing> 
BBeing::findBeing(const String& name, bool recursive) const {
  for (const auto& child : m_childrenNodes) {
    if (auto being = std::static_pointer_cast<BBeing>(child.lock())) {
      if (being->m_nodeName == name) {
        return being;
      }
      if (recursive) {
        auto nested = being->findBeing(name, true);
        if (nested.lock()) return nested;
      }
    }
  }
  return {};
}

Vector<WPtr<BBeing>> 
BBeing::getAllBeingsInHierarchy() const {
  Vector<WPtr<BBeing>> beings;
  for (const auto& child : m_childrenNodes) {
    if (auto being = std::static_pointer_cast<BBeing>(child.lock())) {
      beings.push_back(being);
      auto nested = being->getAllBeingsInHierarchy();
      beings.insert(beings.end(), nested.begin(), nested.end());
    }
  }
  return beings;
}

WPtr<BBeing> 
BBeing::createChild(const String& name) {
  WPtr<BBeing> child = SceneManager::instance().createBeing<BBeing>(name);
  // WPtr<CTransform> childTransform = child.lock()->createComponent<CTransform>();
  addChild(child);

  // Ensure child has transform component

  return child;
}

}