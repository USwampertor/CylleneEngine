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

  SNode(const SNode& other)
    : m_nodeName(other.m_nodeName),
      m_nodeID(other.m_nodeID),
      m_isActive(other.m_isActive),
      m_markedToDestroy(other.m_markedToDestroy),
      m_parentNode(other.m_parentNode),
      m_childrenNodes(other.m_childrenNodes) {}

  virtual ~SNode() = default;

  // Hierarchy management
  virtual void addChild(WPtr<SNode> child, bool keepWorldTransform = true);
  virtual void addChildren(const Vector<WPtr<SNode>>& newChildren, bool keepWorldTransform = true);
  virtual void removeChild(WPtr<SNode> child, bool recursive = false);
  virtual void removeChild(const String& child);
  virtual void removeAllChildren();

  // Transform access
  virtual WPtr<CTransform> getTransform() { return {}; }

  // Parent/child relationships
  WPtr<SNode> getParent() const { return m_parentNode; }
  Vector<WPtr<SNode>>& getChildren() { return m_childrenNodes; }

  // Find functionality
  virtual WPtr<SNode> findChild(const String& nodeName, bool recursive = true) const;
  virtual WPtr<SNode> findChildWhere(Callback<bool, WPtr<SNode>> condition, bool recursive = true) const;
  virtual Vector<WPtr<SNode>> findChildren(Callback<bool, WPtr<SNode>> condition, bool recursive = true) const;
  // void setParent(const SPtr<SNode>& newParent);
  virtual bool isChildOf(const WPtr<SNode>& potentialParent) const;
  bool isParentOf(const WPtr<SNode>& potentialChild);
  virtual bool isDescendantOf(const WPtr<SNode>& ancestor) const;

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
  friend class SceneManager;

  void setParent(const WPtr<SNode>& parent, bool keepWorldTransform = true);
protected:
  /*Matrix4 getWorldTransform() const;*/

  Vector<WPtr<SNode>> m_childrenNodes;

  WPtr<SNode> m_parentNode;

  String m_nodeName;

  uint32 m_nodeID = 0;

  bool m_isActive = true;

  bool m_markedToDestroy = false;

};

}


