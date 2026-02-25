#pragma once

#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyMatrix3.h"
#include "cyMatrix4.h"
#include "cyPrimitive.h"
#include "cyQuaternion.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT OBB : public Primitive
{
public:

  OBB()
    : Primitive(OBB::staticType()),
      m_center(Vector3f::ZERO),
      m_hExtents(Vector3f::ZERO),
      m_orientation(Quaternion::IDENTITY) {}

  OBB(const OBB& other)
    : Primitive(OBB::staticType()),
      m_center(other.m_center),
      m_hExtents(other.m_hExtents),
      m_orientation(other.m_orientation) {}

  OBB(const Vector3f& center, const Vector3f& halfExtents, const Quaternion& orientation)
    : Primitive(OBB::staticType()),
      m_center(center),
      m_hExtents(halfExtents),
      m_orientation(orientation) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::OBB;
  }

  Vector3f
  getDimensions();

  Vector3f
  getDimensions() const {
    return m_hExtents * 2.0f;
  }

  /**
   * @brief Sets all OBB properties at once.
   */
  void
  setTransform(const Vector3f& center, const Vector3f& halfExtents, const Quaternion& orientation) {
    m_center = center;
    m_hExtents = halfExtents;
    m_orientation = orientation;
  }

  /**
   * @brief Sets center.
   */
  void
  setCenter(const Vector3f& center) {
    m_center = center;
  }

  /**
   * @brief Sets half extents.
   */
  void
  setHalfExtents(const Vector3f& halfExtents) {
    m_hExtents = halfExtents;
  }

  /**
   * @brief Sets orientation.
   */
  void
  setOrientation(const Quaternion& orientation) {
    m_orientation = orientation;
  }

  /**
   * @brief Gets center.
   */
  const Vector3f&
  getCenter() const {
    return m_center;
  }

  /**
   * @brief Gets half extents.
   */
  const Vector3f&
  getHalfExtents() const {
    return m_hExtents;
  }

  /**
   * @brief Gets orientation.
   */
  const Quaternion&
  getOrientation() const {
    return m_orientation;
  }

  /**
   * @brief Returns true when extents are non-negative.
   */
  bool
  isValid() const {
    return m_hExtents.x >= 0.0f &&
           m_hExtents.y >= 0.0f &&
           m_hExtents.z >= 0.0f;
  }

  /**
   * @brief Returns local X axis in world space.
   */
  Vector3f
  getRightAxis() const {
    return m_orientation.getMatrix4Rotation().getRightVector();
  }

  /**
   * @brief Returns local Y axis in world space.
   */
  Vector3f
  getUpAxis() const {
    return m_orientation.getMatrix4Rotation().getUpVector();
  }

  /**
   * @brief Returns local Z axis in world space.
   */
  Vector3f
  getForwardAxis() const {
    return m_orientation.getMatrix4Rotation().getForwardVector();
  }

  /**
   * @brief Volume = (2ex)(2ey)(2ez).
   */
  float
  getVolume() const {
    const Vector3f d = getDimensions();
    return d.x * d.y * d.z;
  }

  /**
   * @brief Surface area = 2(xy + xz + yz) using full dimensions.
   */
  float
  getSurfaceArea() const {
    const Vector3f d = getDimensions();
    return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
  }

  /**
   * @brief Closest point on/in OBB to world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const {
    const Vector3f d = point - m_center;
    const Vector3f right = getRightAxis();
    const Vector3f up = getUpAxis();
    const Vector3f forward = getForwardAxis();

    float x = Math::clamp(Vector3f::dot(d, right), -m_hExtents.x, m_hExtents.x);
    float y = Math::clamp(Vector3f::dot(d, up), -m_hExtents.y, m_hExtents.y);
    float z = Math::clamp(Vector3f::dot(d, forward), -m_hExtents.z, m_hExtents.z);

    return m_center + right * x + up * y + forward * z;
  }

  /**
   * @brief Checks if world point lies inside (or on boundary).
   */
  bool
  contains(const Vector3f& point) const {
    const Vector3f d = point - m_center;
    const Vector3f right = getRightAxis();
    const Vector3f up = getUpAxis();
    const Vector3f forward = getForwardAxis();

    const float x = Vector3f::dot(d, right);
    const float y = Vector3f::dot(d, up);
    const float z = Vector3f::dot(d, forward);

    return Math::abs(x) <= m_hExtents.x &&
           Math::abs(y) <= m_hExtents.y &&
           Math::abs(z) <= m_hExtents.z;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  void
  expandTo(const Vector3f& pos);

  void
  expandTo(const OBB& other);

  float
  projectOntoAxis(const Vector3f& axis) const;

public:

  Vector3f m_center;
  Vector3f m_hExtents;
  Quaternion m_orientation;

};

}

