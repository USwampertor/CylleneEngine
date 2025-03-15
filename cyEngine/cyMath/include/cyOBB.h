#pragma once

#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyVector4f.h"
#include "cyMatrix3x3.h"
#include "cyQuaternion.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT OBB
{
public:

  OBB() = default;

  OBB(const OBB& other)
    : m_center(other.m_center),
      m_hExtents(other.m_hExtents),
      m_orientation(other.m_orientation) {}

  OBB(const Point& center, const Vector3f& halfExtents, const Quaternion& orientation)
    : m_center(center),
      m_hExtents(halfExtents),
      m_orientation(orientation) {}

  Vector3f
  getDimensions();

  String
  toString();

  bool
  intersects();

  void
  expandTo(const Point& pos);

  void
  expandTo(const OBB& other);

public:

  Point m_center;
  Vector3f m_hExtents;
  Quaternion m_orientation;


};

}

