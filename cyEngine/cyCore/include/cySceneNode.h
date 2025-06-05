#pragma once
#include "cyCorePrerequisites.h"

#include <cyMatrix4.h>

namespace CYLLENE_SDK {

class SNode
{
  SNode() = default;
  
  virtual ~SNode() {}

  void
  addChild(SPtr<SNode> newChild);

  void
  removeChildAt(uint32 index);

  void
  removeChild(SPtr<SNode> oldChild);

  void
  setParent(SPtr<SNode> newParent);

  const Matrix4& 
  getWorldTransform() const;

  void 
  setLocalTransform(const Matrix4& transform);
  
  void 
  updateWorldTransform(); // Recursively updates world transform

protected:
  Vector<SPtr<SNode>> m_children;
  SPtr<SNode> m_parent = nullptr;
  Matrix4 m_localTransform;
  Matrix4 m_worldTransform;
  bool m_dirty = true; // For lazy world transform updates

};

}


