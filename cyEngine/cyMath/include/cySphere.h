#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT Sphere : public Primitive
{
public:

  Sphere()
    : Primitive(PRIMITIVE_TYPE::E::SPHERE),
      m_center(Vector3f::ZERO),
      m_radius(0.0f) {}


  Sphere(const Sphere& other)
    : Primitive(PRIMITIVE_TYPE::E::SPHERE),
      m_center(other.m_center),
      m_radius(other.m_radius) {}

  Sphere(const Vector3f& center, const float& radius)
    : Primitive(PRIMITIVE_TYPE::E::SPHERE),
      m_center(center),
      m_radius(radius) {}

  float
  getDiameter();

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  void
  expandTo(const Vector3f& pos);

public:

  Vector3f m_center;
  float m_radius;

};

}

