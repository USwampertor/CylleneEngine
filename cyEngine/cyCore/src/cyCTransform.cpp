#include "cyCTransform.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void
CTransform::setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
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
  m_parent.reset();
  m_parent = { newParent };
}




}