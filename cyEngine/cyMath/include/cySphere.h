#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

/**
 * @class Sphere
 * @brief A sphere defined by a center point and radius.
 */
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
    return PRIMITIVE_TYPE::E::eSPHERE;
  }

  
  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

   /**
   * @brief Sets both center and radius.
   * @param center The new center of the sphere.
   * @param radius The new radius of the sphere. Must be non-negative for a valid
   */
  void
  setDimensions(const Vector3f& center, const float& radius);

  /**
   * @brief Sets sphere center.
   * @param center The new center of the sphere.
   */
  void
  setCenter(const Vector3f& center) {
    m_center = center;
  }

  /**
   * @brief Sets sphere radius.
   * @param radius The new radius of the sphere. Must be non-negative for a valid sphere.
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Gets sphere center.
   * @return The center of the sphere as a Vector3f.
   */
  const Vector3f&
  getCenter() const {
    return m_center;
  }

  /**
   * @brief Gets sphere radius.
   * @return The radius of the sphere as a float. Must be non-negative for a valid sphere.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Gets the diameter of the sphere.
   * @return The diameter of the sphere, which is twice the radius.
   */
  float
  getDiameter() const;

  /**
   * @brief Checks if sphere has valid dimensions.
   * @return True if the radius is non-negative, false otherwise. 
   *         A valid sphere must have a non-negative radius.
   */
  bool
  isValid() const;

  /**
   * @brief Circumference of the great circle.
   */
  float
  getCircumference() const;

  /**
   * @brief Surface area.
   */
  float
  getSurfaceArea() const;

  /**
   * @brief Volume.
   */
  float
  getVolume() const;

  /**
   * @brief Point containment test (inside or on boundary).
   */
  bool
  contains(const Vector3f& point) const;

  /**
   * @brief Closest point on sphere surface to world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const;

  /**
   * @brief Expands the sphere to include the given point if it's outside the current sphere.
   * @param pos The point to include in the sphere.
   */
  void
  expandTo(const Vector3f& pos);

public:

  /**
   * @brief Center of the sphere.
   */
  Vector3f m_center;

  /**
   * @brief Radius of the sphere. Must be non-negative for a valid sphere.
   */
  float m_radius;

};

}

