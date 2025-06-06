#pragma once
#include "cyCorePrerequisites.h"

#include "cyCTransform.h"

#include <cyMatrix4.h>


namespace CYLLENE_SDK {

class CY_CORE_EXPORT SNode
{
public:
  SNode() = default;
  virtual ~SNode() = default;

  // Hierarchy management
  virtual void addChild(SPtr<SNode> child, bool keepWorldTransform = true);
  virtual void removeChild(SPtr<SNode> child, bool recursive = false);

  // Transform access
  virtual SPtr<CTransform> getTransform() { return nullptr; }

  // Parent/child relationships
  WPtr<SNode> getParent() const { return m_parent; }
  Vector<SPtr<SNode>>& getChildren() { return m_children; }

  // Find functionality
  virtual SPtr<SNode> findChild(const String& name, bool recursive = true) const;

  virtual Vector<SPtr<SNode>> findChildren(Callback<bool, SPtr<SNode>> condition, bool recursive = true) const;
  
  virtual bool isDescendantOf(const SPtr<SNode>& ancestor) const;

  void 
  setActive(bool active) {
    m_isActive = active;
  }

  const bool& 
  isActive() { return m_isActive; }

  friend class BBeing;

protected:
  void setParent(SPtr<SNode> parent, bool keepWorldTransform = true);
  /*Matrix4 getWorldTransform() const;*/

  Vector<SPtr<SNode>> m_children;
  WPtr<SNode> m_parent;

  String m_nodeName;

  uint32 m_nodeID = 0;

  bool m_isActive = true;
};

}


