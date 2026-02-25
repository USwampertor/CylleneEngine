#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"

#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

class CY_MATH_EXPORT Point : public Primitive, public Vector3f
{
public:
  Point() : Primitive(Point::staticType()) {}
  
  Point(float nx, float ny, float nz) : 
    Primitive(Point::staticType()), 
    Vector3f(nx, ny, nz) {}

  static PRIMITIVE_TYPE::E 
  staticType() { return PRIMITIVE_TYPE::E::POINT; }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets point coordinates.
   */
  void
  set(const float& nx, const float& ny, const float& nz) {
    x = nx;
    y = ny;
    z = nz;
  }

  /**
   * @brief Sets from a Vector3f.
   */
  void
  set(const Vector3f& v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  /**
   * @brief Returns point as Vector3f value.
   */
  Vector3f
  toVector3f() const {
    return Vector3f(x, y, z);
  }

  /**
   * @brief Squared distance to another point.
   */
  float
  sqrDistanceTo(const Point& other) const {
    return Vector3f::sqrDistance(*this, other);
  }

  /**
   * @brief Distance to another point.
   */
  float
  distanceTo(const Point& other) const {
    return Vector3f::distance(*this, other);
  }

  /**
   * @brief Squared distance to a world position.
   */
  float
  sqrDistanceTo(const Vector3f& other) const {
    return Vector3f::sqrDistance(*this, other);
  }

  /**
   * @brief Distance to a world position.
   */
  float
  distanceTo(const Vector3f& other) const {
    return Vector3f::distance(*this, other);
  }

  /**
   * @brief Midpoint between this and another point.
   */
  Point
  midpoint(const Point& other) const {
    return Point((x + other.x) * 0.5f,
                 (y + other.y) * 0.5f,
                 (z + other.z) * 0.5f);
  }

  /**
   * @brief Linear interpolation from this point to another.
   */
  Point
  lerp(const Point& other, const float& t) const {
    const float clampedT = Math::clamp(t, 0.0f, 1.0f);
    return Point(x + (other.x - x) * clampedT,
                 y + (other.y - y) * clampedT,
                 z + (other.z - z) * clampedT);
  }

};

}

