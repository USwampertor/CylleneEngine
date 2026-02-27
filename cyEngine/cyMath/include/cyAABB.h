/**
 * @file cyAABB.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for AABB.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {


/**
 * @class AABB
 * @brief Axis-Aligned Bounding Box defined by minimum and maximum corners. 
 *        Used for simple collision detection and spatial queries.
 */
class CY_MATH_EXPORT AABB : public Primitive
{
public:

  /**
   * @brief Default constructor initializes to unit cube from (0,0,0) to (1,1,1).
   */
  AABB() 
    : Primitive(AABB::staticType()),
      m_min(Vector3f::ZERO),
      m_max(Vector3f::ONE) {}

  /**
   * @brief Constructor with specified min and max boundaries.
   */
  AABB(const Vector3f& min, const Vector3f& max)
    : Primitive(AABB::staticType()),
      m_min(min),
      m_max(max) {}

  /**
   * @brief Copy constructor.
   */
  AABB(const AABB& other)
    : Primitive(AABB::staticType()),
      m_min(other.m_min),
      m_max(other.m_max) {}

  static PRIMITIVE_TYPE::E 
  staticType() {
    return PRIMITIVE_TYPE::E::eAABB;
  }

  /**
   * @brief Converts AABB to string representation.
   * @return Formatted AABB string.
   */
  virtual String
  toString() override;

  /**
   * @brief Checks if this AABB intersects another primitive.
   * @param other Primitive to test.
   * @return True if intersecting.
   */
  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Gets the dimensions of the box (max - min).
   * @return Dimensions of the box as a Vector3f.
   */
  const Vector3f
  getDimensions() const;

  /**
   * @brief Gets the center point of the box ((min + max) / 2).
   * @return Center point of the box as a Vector3f.
   */
  const Vector3f
  getCenter() const;

  /**
   * @brief Sets both box corners.
   * @param min Minimum corner of the box.
   * @param max Maximum corner of the box.
   */
  void
  setDimensions(const Vector3f& min, const Vector3f& max);

  /**
   * @brief Sets minimum corner.
   * @param min Minimum corner of the box.
   */
  void
  setMin(const Vector3f& min) {
    m_min = min;
  }

  /**
   * @brief Sets maximum corner.
   * @param max Maximum corner of the box.
   */
  void
  setMax(const Vector3f& max) {
    m_max = max;
  }

  /**
   * @brief Gets minimum corner.
   * @return Minimum corner of the box as a Vector3f.
   */
  const Vector3f&
  getMin() const {
    return m_min;
  }

  /**
   * @brief Gets maximum corner.
   * @return Maximum corner of the box as a Vector3f.
   */
  const Vector3f&
  getMax() const {
    return m_max;
  }

  /**
   * @brief Returns true if min <= max for all axes.
   * @return True if the box is valid, false otherwise.
   */
  bool
  isValid() const;
  /**
   * @brief Half extents from center.
   * @return Half of the dimensions of the box as a Vector3f.
   */
  Vector3f
  getHalfExtents() const;

  /**
   * @brief Surface area for a valid AABB.
   * @return Surface area of the box, or 0 if the box is invalid.
   */
  float
  getSurfaceArea() const;

  /**
   * @brief Volume for a valid AABB.
   * @return Volume of the box, or 0 if the box is invalid.
   */
  float
  getVolume() const;

  /**
   * @brief Checks if a point is inside or on boundaries.
   * @param point The point to check.
   * @return True if the point is inside or on the boundaries of the box, false otherwise.
   */
  bool
  contains(const Vector3f& point) const;

  /**
   * @brief Checks if this box fully contains another AABB.
   * @param other The other AABB to check.
   * @return True if this box fully contains the other AABB, false otherwise.
   */
  bool
  contains(const AABB& other) const;

  /**
   * @brief Expands the box to include a point, adjusting min and max as needed.
   * @param pos The point to include in the box.
   */
  void
  expandTo(const Vector3f& pos);

  /**
   * @brief Expands the box to include a point, adjusting min and max as needed.
   * @param other The other AABB to include in this box.
   */
  void
  expandTo(const AABB& other);

  /**
   * @brief Expands the box to include a point, adjusting min and max as needed.
   * @param min The min point to include in the box.
   * @param max The max point to include in the box.
   */
  void
  expandTo(const Vector3f& min, const Vector3f& max);

  /**
   * @brief Expands the box to include a point, adjusting min and max as needed.
   * @param x The x position to include in the box.
   * @param y The y position to include in the box.
   * @param z The z position to include in the box.
   */
  void
  expandTo(const float& x, const float& y, const float& z);

public:

  /**
   * @brief Minimum corner of the box (smallest x, y, z).
   */
  Vector3f m_min;

  /**
   * @brief Maximum corner of the box (largest x, y, z).
   */
  Vector3f m_max;
};

}


