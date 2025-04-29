#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"


namespace CYLLENE_SDK {

class CY_MATH_EXPORT Capsule : public Primitive
{
public:
  Capsule()
    : Primitive(PRIMITIVE_TYPE::E::CAPSULE),
      m_pointA(Vector3f::ZERO),
      m_pointB(Vector3f::ZERO),
      m_radius(0.0f) {}

  Capsule(const Capsule& other)
    : Primitive(PRIMITIVE_TYPE::E::CAPSULE),
      m_pointA(other.m_pointA),
      m_pointB(other.m_pointB),
      m_radius(other.m_radius) {}

  Capsule(const Vector3f& a, const Vector3f& b, const float& radius)
    : Primitive(PRIMITIVE_TYPE::E::CAPSULE),
      m_pointA(a),
      m_pointB(b),
      m_radius(radius) {}


  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

public:

  Vector3f m_pointA;
  Vector3f m_pointB;
  float m_radius;

};

}

