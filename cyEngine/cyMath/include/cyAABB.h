#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector4f.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT AABB
{
public:

  AABB() = default;

  AABB(const Point& min, const Point& max)
    : m_min(min),
      m_max(max) {}

  AABB(const AABB& other)
    : m_min(other.m_min),
      m_max(other.m_max) {}

  Vector3f
  getDimensions();

  Point
  getCenter();

  String
  toString();

  bool
  intersects();

  void
  expandTo(const Point& pos);

  void
  expandTo(const AABB& other);

public:
  Point m_min;
  Point m_max;
};

}

