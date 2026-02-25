#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

class CY_MATH_EXPORT Plane : public Primitive
{
public:
  Plane()
    : Primitive(Plane::staticType()),
      m_origin(Vector3f::ZERO),
      m_normal(Vector3f::ONEZ) {}

  Plane(const Vector3f& origin, const Vector3f& normal)
    : Primitive(Plane::staticType()),
      m_origin(origin),
      m_normal(normal) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::PLANE;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets plane by origin and normal.
   */
  void
  setOriginNormal(const Vector3f& origin, const Vector3f& normal) {
    m_origin = origin;
    m_normal = normal;
  }

  /**
   * @brief Sets plane origin.
   */
  void
  setOrigin(const Vector3f& origin) {
    m_origin = origin;
  }

  /**
   * @brief Sets plane normal.
   */
  void
  setNormal(const Vector3f& normal) {
    m_normal = normal;
  }

  /**
   * @brief Gets plane origin.
   */
  const Vector3f&
  getOrigin() const {
    return m_origin;
  }

  /**
   * @brief Gets plane normal.
   */
  const Vector3f&
  getNormal() const {
    return m_normal;
  }

  /**
   * @brief Returns true when normal has non-zero magnitude.
   */
  bool
  isValid() const {
    return m_normal.sqrMagnitude() > Math::EPSILONF;
  }

  /**
   * @brief Normalized copy of the normal vector.
   */
  Vector3f
  getUnitNormal() const {
    if (!isValid()) {
      return Vector3f::ZERO;
    }
    return m_normal.normalized();
  }

  /**
   * @brief Normalizes the stored normal in place.
   */
  void
  normalize() {
    if (isValid()) {
      m_normal.normalize();
    }
  }

  /**
   * @brief Plane constant d in equation n.x + d = 0.
   */
  float
  getConstantD() const {
    const Vector3f n = getUnitNormal();
    return -(n | m_origin);
  }

  /**
   * @brief Plane distance term for equation n.x = d.
   */
  float
  getDistance() const;

  /**
   * @brief Signed distance from point to plane.
   */
  float
  signedDistanceTo(const Vector3f& point) const {
    const Vector3f n = getUnitNormal();
    if (n.isZero()) {
      return 0.0f;
    }
    return (n | (point - m_origin));
  }

  /**
   * @brief Absolute distance from point to plane.
   */
  float
  distanceTo(const Vector3f& point) const {
    return Math::abs(signedDistanceTo(point));
  }

  /**
   * @brief Projects a point onto the plane.
   */
  Vector3f
  projectPoint(const Vector3f& point) const {
    const Vector3f n = getUnitNormal();
    if (n.isZero()) {
      return point;
    }
    const float dist = signedDistanceTo(point);
    return point - n * dist;
  }

  /**
   * @brief Returns true if point is on plane within tolerance.
   */
  bool
  contains(const Vector3f& point, const float& tolerance = Math::SMALLNUMBER) const {
    return Math::abs(signedDistanceTo(point)) <= tolerance;
  }

  /**
   * @brief Returns +1 if in front, -1 if behind, 0 if on plane.
   */
  int32
  sideOf(const Vector3f& point, const float& tolerance = Math::SMALLNUMBER) const {
    const float s = signedDistanceTo(point);
    if (s > tolerance) {
      return 1;
    }
    if (s < -tolerance) {
      return -1;
    }
    return 0;
  }

private:

  Vector3f m_origin;

  Vector3f m_normal;
};


}
