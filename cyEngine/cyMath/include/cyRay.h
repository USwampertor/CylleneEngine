#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"
#include "cyVector2f.h"

namespace CYLLENE_SDK
{

struct HitInfo
{
  bool hit = false;
  float distance = std::numeric_limits<float>::infinity();

  uint32 triangleIndex = std::numeric_limits<uint32>::max();

  Vector2f uv = Vector2f::ZERO;
  Vector3f position = Vector3f::ZERO;
  Vector3f normal = Vector3f::ZERO;
};

class Ray : public Primitive
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
    return PRIMITIVE_TYPE::E::RAY;
  }

  virtual String
  toString() override;

  /**
   * @brief Sets both origin and direction.
   */
  void
  set(const Vector3f& origin, const Vector3f& direction) {
    m_origin = origin;
    m_direction = direction;
  }

  /**
   * @brief Sets ray origin.
   */
  void
  setOrigin(const Vector3f& origin) {
    m_origin = origin;
  }

  /**
   * @brief Sets ray direction.
   */
  void
  setDirection(const Vector3f& direction) {
    m_direction = direction;
  }

  /**
   * @brief Gets ray origin.
   */
  const Vector3f&
  getOrigin() const {
    return m_origin;
  }

  /**
   * @brief Gets ray direction.
   */
  const Vector3f&
  getDirection() const {
    return m_direction;
  }

  /**
   * @brief Returns true if direction has non-zero magnitude.
   */
  bool
  isValid() const {
    return m_direction.sqrMagnitude() > Math::EPSILONF;
  }

  /**
   * @brief Returns normalized direction, or ZERO if invalid.
   */
  Vector3f
  getUnitDirection() const {
    if (!isValid()) {
      return Vector3f::ZERO;
    }
    return m_direction.normalized();
  }

  /**
   * @brief Normalizes direction in place if valid.
   */
  void
  normalizeDirection() {
    if (isValid()) {
      m_direction.normalize();
    }
  }

  /**
   * @brief Returns a world point at ray parameter t.
   */
  Vector3f
  getPointAt(const float& t) const {
    return m_origin + m_direction * t;
  }

  /**
   * @brief Returns projected parameter t for point onto this ray.
   */
  float
  projectParameter(const Vector3f& point) const {
    const float denom = m_direction.sqrMagnitude();
    if (denom <= Math::EPSILONF) {
      return 0.0f;
    }
    return Math::max(0.0f, Vector3f::dot(point - m_origin, m_direction) / denom);
  }

  /**
   * @brief Closest point on ray to world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const {
    return getPointAt(projectParameter(point));
  }

  /**
   * @brief Squared distance from world point to this ray.
   */
  float
  sqrDistanceToPoint(const Vector3f& point) const {
    const Vector3f c = closestPoint(point);
    return Vector3f::sqrDistance(c, point);
  }

  /**
   * @brief Distance from world point to this ray.
   */
  float
  distanceToPoint(const Vector3f& point) const {
    return Vector3f::distance(closestPoint(point), point);
  }


public:

  Vector3f m_origin;
  Vector3f m_direction;
};

}

