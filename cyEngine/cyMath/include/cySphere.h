#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT Sphere : public Primitive
{
public:

  Sphere()
    : Primitive(Sphere::staticType()),
      m_center(Vector3f::ZERO),
      m_radius(0.0f) {}


  Sphere(const Sphere& other)
    : Primitive(Sphere::staticType()),
      m_center(other.m_center),
      m_radius(other.m_radius) {}

  Sphere(const Vector3f& center, const float& radius)
    : Primitive(Sphere::staticType()),
      m_center(center),
      m_radius(radius) {}

  static PRIMITIVE_TYPE::E
  staticType() {
    return PRIMITIVE_TYPE::E::SPHERE;
  }

  float
  getDiameter() {
    return m_radius * 2.0f;
  }

  float
  getDiameter() const {
    return m_radius * 2.0f;
  }

  /**
   * @brief Sets both center and radius.
   */
  void
  set(const Vector3f& center, const float& radius) {
    m_center = center;
    m_radius = radius;
  }

  /**
   * @brief Sets sphere center.
   */
  void
  setCenter(const Vector3f& center) {
    m_center = center;
  }

  /**
   * @brief Sets sphere radius.
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Gets sphere center.
   */
  const Vector3f&
  getCenter() const {
    return m_center;
  }

  /**
   * @brief Gets sphere radius.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Checks if sphere has valid dimensions.
   */
  bool
  isValid() const {
    return m_radius >= 0.0f;
  }

  /**
   * @brief Circumference of the great circle.
   */
  float
  getCircumference() const {
    return 2.0f * Math::PI * m_radius;
  }

  /**
   * @brief Surface area.
   */
  float
  getSurfaceArea() const {
    return 4.0f * Math::PI * Math::sqr(m_radius);
  }

  /**
   * @brief Volume.
   */
  float
  getVolume() const {
    return (4.0f / 3.0f) * Math::PI * Math::pow(m_radius, 3.0f);
  }

  /**
   * @brief Point containment test (inside or on boundary).
   */
  bool
  contains(const Vector3f& point) const {
    return Vector3f::sqrDistance(point, m_center) <= Math::sqr(m_radius);
  }

  /**
   * @brief Closest point on sphere surface to world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const {
    const Vector3f toPoint = point - m_center;
    if (toPoint.sqrMagnitude() <= Math::EPSILONF) {
      return m_center + Vector3f::RIGHT * m_radius;
    }
    return m_center + toPoint.normalized() * m_radius;
  }

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

