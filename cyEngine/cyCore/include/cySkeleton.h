#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

#include <cyMatrix4.h>

namespace CYLLENE_SDK {

struct CY_CORE_EXPORT VertexWeight
{
public:
  uint32 m_ID;
  float m_weight;
};

struct CY_CORE_EXPORT Bone
{
public:
  String m_name;

  Matrix4 m_offset;

  Matrix4 m_worldPosition;

  Matrix4 m_localPosition;

  uint32 m_ID;

  Vector<VertexWeight> m_weights;

  Bone* m_parent;

  Vector<Bone*> m_children;
};

class CY_CORE_EXPORT SkeletonResource : public Resource
{
public:

  uint32 m_ID;

  Map<String, Bone*> m_set;

};
}

