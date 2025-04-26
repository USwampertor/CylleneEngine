#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT AABB : public Primitive
{
public:

  AABB() 
    : Primitive(PRIMITIVE_TYPE::E::AABB), 
      m_min(Vector3f::ZERO),
      m_max(Vector3f::ONE) {}

  AABB(const Vector3f& min, const Vector3f& max)
    : Primitive(PRIMITIVE_TYPE::E::AABB),
      m_min(min),
      m_max(max) {}


  AABB(const AABB& other)
    : Primitive(PRIMITIVE_TYPE::E::AABB),
      m_min(other.m_min),
      m_max(other.m_max) {}

  Vector3f
  getDimensions();

  Vector3f
  getCenter();

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  void
  expandTo(const Vector3f& pos);

  void
  expandTo(const AABB& other);

public:
  Vector3f m_min;
  Vector3f m_max;
};

}

