/**
 * @file cyRay.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Ray.
 */
#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"
#include "cyVector2f.h"

namespace CYLLENE_SDK {

/**
 * @class HitInfo
 * @brief Struct to hold ray hit information.
 */
struct HitInfo
{
  /**
   * @brief True if ray hit a primitive, false otherwise.
   */
  bool hit = false;

  /**
   * @brief Distance from ray origin to hit point. Infinity if no hit.
   */
  float distance = std::numeric_limits<float>::infinity();

  /**
   * @brief Index of the triangle hit, if applicable. Max uint32 if no hit or not a triangle mesh.
   */
  uint32 triangleIndex = std::numeric_limits<uint32>::max();

  /**
   * @brief Barycentric coordinates of the hit point on the triangle, if applicable. ZERO if no hit or not a triangle mesh.
   */
  Vector2f uv = Vector2f::ZERO;
  
  /**
   * @brief World space position of the hit point. ZERO if no hit.
   */
  Vector3f position = Vector3f::ZERO;
  
  /**
   * @brief World space normal at the hit point. ZERO if no hit.
   */
  Vector3f normal = Vector3f::ZERO;
};

/**
 * @class Ray
 * @brief A ray primitive defined by an origin and a direction. 
 *        The direction can have non-unit magnitude, which can be useful for ray 
 *        marching or other applications where the ray's length is relevant.
 */
class CY_MATH_EXPORT Ray : public Primitive
{
public:
  Ray()
    : Primitive(Ray::staticType()),
      m_origin(Vector3f::ZERO),
      m_direction(Vector3f::FRONT) {}

  Ray(const Ray& other)
    : Primitive(Ray::staticType()),
      m_origin(other.m_origin),
      m_direction(other.m_direction) {}

  Ray(const Vector3f& origin, const Vector3f& direction)
    : Primitive(Ray::staticType()),
      m_origin(origin),
      m_direction(direction) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::eRAY;
  }

  /**
   * @brief Checks if this ray intersects another primitive.
   * @param other Primitive to test.
   * @return True if intersecting.
   */
  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Converts ray to string representation.
   * @return Formatted ray string.
   */
  virtual String
  toString() override;

  /**
   * @brief Sets both origin and direction.
   * @param origin The world space point where the ray starts.
   * @param direction The world space vector representing the ray's direction and magnitude.
   */
  void
  setDimensions(const Vector3f& origin, const Vector3f& direction);

  /**
   * @brief Sets ray origin.
   * @param origin The world space point where the ray starts.
   */
  void
  setOrigin(const Vector3f& origin) {
    m_origin = origin;
  }

  /**
   * @brief Sets ray direction.
   * @param direction The world space vector representing the ray's direction and magnitude.
   */
  void
  setDirection(const Vector3f& direction) {
    m_direction = direction;
  }

  /**
   * @brief Gets ray origin.
   * @return The world space point where the ray starts.
   */
  const Vector3f&
  getOrigin() const {
    return m_origin;
  }

  /**
   * @brief Gets ray direction.
   * @return The world space vector representing the ray's direction and magnitude.
   *         Direction may have non-unit magnitude, which can be useful for ray marching 
   *         or other applications where the ray's length is relevant.
   */
  const Vector3f&
  getDirection() const {
    return m_direction;
  }

  /**
   * @brief Returns true if direction has non-zero magnitude.
   * @return True if direction has non-zero magnitude, false if direction is zero 
   *         vector or near zero (invalid ray).
   *         A ray with a zero or near-zero direction is considered invalid, as it does 
   *         not define a meaningful path in space.
   */
  bool
  isValid() const;

  /**
   * @brief Returns normalized direction, or ZERO if invalid.
   * @return A unit vector in the same direction as the ray's direction, 
   *         or ZERO if the ray is invalid.
   */
  Vector3f
  getUnitDirection() const;

  /**
   * @brief Normalizes direction in place if valid.
   */
  void
  normalizeDirection();

  /**
   * @brief Returns a world point at ray parameter t.
   * @param t The parameter along the ray, where 0 corresponds to the origin, 
   *        and 1 corresponds to the point at origin + direction.
   * @return The world space point at the given parameter t along the ray.
   */
  Vector3f
  getPointAt(const float& t) const;

  /**
   * @brief Returns projected parameter t for point onto this ray.
   * @param point The world space point to project onto the ray.
   * @return The parameter t along the ray where the closest point to the 
   *         given world point lies.
   */
  float
  projectParameter(const Vector3f& point) const;

  /**
   * @brief Closest point on ray to world point.
   * @param point The world space point to find the closest point to.
   * @return The world space point on the ray that is closest to the given world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const;

  /**
   * @brief Squared distance from world point to this ray.
   * @param point The world space point to measure distance to.
   * @return The squared distance from the given world point to the
   *         closest point on the ray.
   */
  float
  sqrDistanceToPoint(const Vector3f& point) const;

  /**
   * @brief Distance from world point to this ray.
   * @param point The world space point to measure distance to.
   * @return The distance from the given world point to the closest point on the ray.
   */
  float
  distanceToPoint(const Vector3f& point) const;

public:

  /**
   * @brief The world space point where the ray starts.
   */
  Vector3f m_origin;

  /**
   * @brief The world space vector representing the ray's direction and magnitude.
   */
  Vector3f m_direction;
};

}


