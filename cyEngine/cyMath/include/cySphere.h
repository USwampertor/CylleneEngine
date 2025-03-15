#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector4f.h"


namespace CYLLENE_SDK {

class CY_MATH_EXPORT Sphere
{
public:

  Sphere() = default;
  Sphere(const Sphere& other)
    : m_center(other.m_center),
      m_radius(other.m_radius) {}

  Sphere(const Point& center, const float& radius)
    : m_center(center),
      m_radius(radius) {}

  float
  getDiameter();

  String
  toString();

  bool
  intersects();

  void
  expandTo(const Point& pos);

public:

  Point m_center;
  float m_radius;

};

}

