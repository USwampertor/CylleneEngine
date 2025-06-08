#pragma once
#include "cyCorePrerequisites.h"

#include "cyCTransform.h"

#include <cyMatrix4.h>


namespace CYLLENE_SDK {

class CY_CORE_EXPORT SNode
{
public:
  SNode() = default;

  SNode(const String& name)
    : m_nodeName(name) {}

  virtual ~SNode() = default;

  // Hierarchy management
  virtual void addChild(SPtr<SNode> child, bool keepWorldTransform = true);
  virtual void addChildren(const Vector<SPtr<SNode>>& newChildren, bool keepWorldTransform = true);
  virtual void removeChild(SPtr<SNode> child, bool recursive = false);
  virtual void removeChild(const String& child);
  virtual void removeAllChildren();

  // Transform access
  virtual SPtr<CTransform> getTransform() { return nullptr; }

  // Parent/child relationships
  WPtr<SNode> getParent() const { return m_parentNode; }
  Vector<SPtr<SNode>>& getChildren() { return m_childrenNodes; }

  // Find functionality
  virtual SPtr<SNode> findChild(const String& nodeName, bool recursive = true) const;
  virtual SPtr<SNode> findChildWhere(Callback<bool, SPtr<SNode>> condition, bool recursive = true) const;
  virtual Vector<SPtr<SNode>> findChildren(Callback<bool, SPtr<SNode>> condition, bool recursive = true) const;
  void setParent(const SPtr<SNode>& newParent);
  virtual bool isChildOf(const SPtr<SNode>& potentialParent) const;
  bool isParentOf(const SPtr<SNode>& potentialChild);
  virtual bool isDescendantOf(const SPtr<SNode>& ancestor) const;

  void 
  setActive(bool active) {
    m_isActive = active;
  }

  const bool& 
  isActive() { return m_isActive; }

  const String& 
  getName() { return m_nodeName; }

  void
  setName(const String& name) { m_nodeName = name; }

  friend class BBeing;

protected:
  void setParent(SPtr<SNode> parent, bool keepWorldTransform = true);
  /*Matrix4 getWorldTransform() const;*/

  Vector<SPtr<SNode>> m_childrenNodes;

  WPtr<SNode> m_parentNode;

  String m_nodeName;

  uint32 m_nodeID = 0;

  bool m_isActive = true;

  bool m_markedToDestroy = false;

};

}


