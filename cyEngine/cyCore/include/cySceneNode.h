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
  virtual void addChild(SPtr<SNode> child);
  virtual void removeChild(SPtr<SNode> child);

  // Transform access
  virtual SPtr<CTransform> getTransform() { return nullptr; }

  // Parent/child relationships
  WPtr<SNode> getParent() const { return m_parent; }
  const Vector<SPtr<SNode>>& getChildren() const { return m_children; }

  // Find functionality
  virtual SPtr<SNode> findChild(const String& name, bool recursive = false) const;

  
  void 
  setActive(bool active) {
    m_isActive = active;
  }

  const bool& 
  isActive() { return m_isActive; }


protected:
  void setParent(SPtr<SNode> parent) { m_parent = parent; }

  Vector<SPtr<SNode>> m_children;
  WPtr<SNode> m_parent;

  String m_nodeName;

  uint32 m_nodeID = 0;

  bool m_isActive = true;
};

}


