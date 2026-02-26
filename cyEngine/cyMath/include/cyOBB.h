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
    return PRIMITIVE_TYPE::E::eOBB;
  }
  
  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Gets the full dimensions of the OBB (width, height, depth).
   * @return full dimensions (2 * half extents).
   */
  Vector3f
  getDimensions() const;

  /**
   * @brief Sets all OBB properties at once.
   * @param center The center of the OBB in world space.
   * @param halfExtents The half extents (half width, half height, half depth) of the OBB.
   * @param orientation The orientation of the OBB as a quaternion.
   */
  void
  setDimensions(const Vector3f& center, 
                const Vector3f& halfExtents, 
                const Quaternion& orientation);

  /**
   * @brief Sets center.
   * @param center The center of the OBB in world space.
   */
  void
  setCenter(const Vector3f& center) {
    m_center = center;
  }

  /**
   * @brief Sets half extents.
   * @param halfExtents The half extents (half width, half height, half depth) of the OBB.
   */
  void
  setHalfExtents(const Vector3f& halfExtents) {
    m_hExtents = halfExtents;
  }

  /**
   * @brief Sets orientation.
   * @param orientation The orientation of the OBB as a quaternion.
   */
  void
  setOrientation(const Quaternion& orientation) {
    m_orientation = orientation;
  }

  /**
   * @brief Gets center.
   * @return The center of the OBB in world space.
   */
  const Vector3f&
  getCenter() const {
    return m_center;
  }

  /**
   * @brief Gets half extents.
   * @return The half extents (half width, half height, half depth) of the OBB.
   */
  const Vector3f&
  getHalfExtents() const {
    return m_hExtents;
  }

  /**
   * @brief Gets orientation.
   * @return The orientation of the OBB as a quaternion.
   */
  const Quaternion&
  getOrientation() const {
    return m_orientation;
  }

  /**
   * @brief Returns true when extents are non-negative.
   * @return True if all half extents are >= 0, false otherwise.
   */
  bool
  isValid() const;

  /**
   * @brief Returns local X axis in world space.
   * @return Local X axis (right) of the OBB in world space.
   */
  Vector3f
  getRightAxis() const;

  /**
   * @brief Returns local Y axis in world space.
   * @return Local Y axis (up) of the OBB in world space.
   */
  Vector3f
  getUpAxis() const;

  /**
   * @brief Returns local Z axis in world space.
   * @return Local Z axis (forward) of the OBB in world space.
   */
  Vector3f
  getForwardAxis() const;

  /**
   * @brief Volume = (2ex)(2ey)(2ez).
   * @return The volume of the OBB using full dimensions.
   */
  float
  getVolume() const;

  /**
   * @brief Surface area = 2(xy + xz + yz) using full dimensions.
   * @return The surface area of the OBB using full dimensions.
   */
  float
  getSurfaceArea() const;

  /**
   * @brief Closest point on/in OBB to world point.
   * @param point The world point to find the closest point to.
   * @return The closest point on or inside the OBB to the given world point.
   */
  Vector3f
  closestPoint(const Vector3f& point) const;

  /**
   * @brief Checks if world point lies inside (or on boundary).
   * @param point The world point to check for containment.
   * @return True if the point is inside or on the boundary of the OBB, false otherwise.
   *
   *         This method works by transforming the point into the OBB's local space and checking
   *         if it lies within the half extents along each local axis.
   */
  bool
  contains(const Vector3f& point) const;

  /**
   * @brief Expands the OBB to include the given world point.
   * @param pos The world point to expand the OBB to include.
   *
   *        This method works by finding the closest point on the OBB to the given point and then
   *        expanding the half extents if necessary to include that point. The center may also be
   *        adjusted if the new point lies outside the current bounds of the OBB.
   */
  void
  expandTo(const Vector3f& pos);


  /**
   * @brief Expands the OBB to include another OBB.
   * @param other The other OBB to expand this OBB to include.
   */
  void
  expandTo(const OBB& other);

  /**
   * @brief Projects the OBB onto the given axis and returns the radius of the projection.
   * @param axis The axis to project the OBB onto (should be normalized).
   * @return The radius of the OBB's projection onto the given axis.
   */
  float
  projectOntoAxis(const Vector3f& axis) const;

public:

  /**
   * @brief The center of the OBB in world space.
   */
  Vector3f m_center;

  /**
   * @brief The half extents (half width, half height, half depth) of the OBB.
   */
  Vector3f m_hExtents;

  /**
   * @brief The orientation of the OBB as a quaternion.
   */
  Quaternion m_orientation;

};

}

