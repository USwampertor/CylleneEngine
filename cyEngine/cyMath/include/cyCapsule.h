#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyVector4f.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT Capsule
{
public:
  Capsule() = default;
  Capsule(const Capsule& other)
    : m_pointA(other.m_pointA),
      m_pointB(other.m_pointB),
      m_radius(other.m_radius) {}

  Capsule(const Point& a, const Point& b, const float& radius)
    : m_pointA(a),
      m_pointB(b),
      m_radius(radius) {}

  String
  toString();

public:

  Point m_pointA;
  Point m_pointB;
  float m_radius;

};

}

