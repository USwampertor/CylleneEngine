#pragma once

#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

class CY_MATH_EXPORT Plane : public Primitive
{
  Plane()
    : Primitive(PRIMITIVE_TYPE::E::PLANE),
      m_origin(Vector3f::ZERO),
      m_normal(Vector3f::ONEZ) {}

  Plane(const Vector3f& origin, const Vector3f& normal)
    : Primitive(PRIMITIVE_TYPE::E::PLANE),
      m_origin(origin),
      m_normal(normal) {}


  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

private:

  Vector3f m_origin;

  Vector3f m_normal;
};


}
