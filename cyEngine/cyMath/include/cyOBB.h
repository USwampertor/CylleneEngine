#pragma once

#include "cyMathPrerequisites.h"

#include "cyMatrix3.h"
#include "cyPrimitive.h"
#include "cyQuaternion.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT OBB : public Primitive
{
public:

  OBB()
    : Primitive(PRIMITIVE_TYPE::E::OBB),
      m_center(Vector3f::ZERO),
      m_hExtents(Vector3f::ZERO),
      m_orientation(Quaternion::IDENTITY) {}

  OBB(const OBB& other)
    : Primitive(PRIMITIVE_TYPE::E::OBB),
      m_center(other.m_center),
      m_hExtents(other.m_hExtents),
      m_orientation(other.m_orientation) {}

  OBB(const Vector3f& center, const Vector3f& halfExtents, const Quaternion& orientation)
    : Primitive(PRIMITIVE_TYPE::E::OBB),
      m_center(center),
      m_hExtents(halfExtents),
      m_orientation(orientation) {}

  Vector3f
  getDimensions();

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  void
  expandTo(const Vector3f& pos);

  void
  expandTo(const OBB& other);

  float
  projectOntoAxis(const Vector3f& axis) const;

public:

  Vector3f m_center;
  Vector3f m_hExtents;
  Quaternion m_orientation;

};

}

