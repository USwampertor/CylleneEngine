/**
 * @file cyPlane.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Plane.
 */
#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

/**
 * @class Plane
 * @brief Represents an infinite plane in 3D space defined by a point (origin) and a normal vector.
 */
class CY_MATH_EXPORT Plane : public Primitive
{
public:
  /**
   * @brief Default constructor.
   */
  Plane()
    : Primitive(Plane::staticType()),
      m_origin(Vector3f::ZERO),
      m_normal(Vector3f::ONEZ) {}

  /**
   * @brief Constructs a plane from origin and normal.
   * @param origin A point on the plane.
   * @param normal Plane normal.
   */
  Plane(const Vector3f& origin, const Vector3f& normal)
    : Primitive(Plane::staticType()),
      m_origin(origin),
      m_normal(normal) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::ePLANE;
  }

  /**
   * @brief Checks if this plane intersects another primitive.
   * @param other Primitive to test.
   * @return True if intersecting.
   */
  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Converts plane to string representation.
   * @return Formatted plane string.
   */
  virtual String
  toString() override;

  /**
   * @brief Sets plane by origin and normal.
   * @param origin A point on the plane.
   * @param normal The normal vector of the plane (does not need to be normalized).
   */
  void
  setDimensions(const Vector3f& origin, const Vector3f& normal);

  /**
   * @brief Sets plane origin.
   * @param origin A point on the plane.
   */
  void
  setOrigin(const Vector3f& origin) {
    m_origin = origin;
  }

  /**
   * @brief Sets plane normal.
   * @param normal The normal vector of the plane (does not need to be normalized).
   *               The plane is considered valid if the normal has non-zero magnitude.
   */
  void
  setNormal(const Vector3f& normal) {
    m_normal = normal;
  }

  /**
   * @brief Gets plane origin.
   * @return A point on the plane.
   */
  const Vector3f&
  getOrigin() const {
    return m_origin;
  }

  /**
   * @brief Gets plane normal.
   * @return The normal vector of the plane (may not be normalized). The plane is considered valid if the normal has non-zero magnitude.
   *         Normalizing the normal vector can be done with getUnitNormal() or normalize().
   */
  const Vector3f&
  getNormal() const {
    return m_normal;
  }

  /**
   * @brief Returns true when normal has non-zero magnitude.
   * @return True if the normal vector has non-zero magnitude, indicating a valid plane; 
   *         false if the normal is zero, indicating an invalid plane.
   *
   *         A plane with a zero normal vector is considered invalid because it does not 
   *         define a unique plane in 3D space. The normal vector is essential for 
   *         defining the orientation of the plane and for performing calculations such 
   *         as distance and projection. If the normal is zero, these calculations would 
   *         be undefined or meaningless.
   */
  bool
  isValid() const;

  /**
   * @brief Normalized copy of the normal vector.
   * @return A normalized version of the plane's normal vector. 
   *         If the plane is invalid (zero normal), returns a zero vector.
   */
  Vector3f
  getUnitNormal() const;

  /**
   * @brief Normalizes the stored normal in place.
   */
  void
  normalize();

  /**
   * @brief Plane constant d in equation n.x + d = 0.
   * @return The plane constant d, which can be calculated as -n | origin, 
   *         where n is the unit normal vector.
   */
  float
  getConstantD() const;

  /**
   * @brief Plane distance term for equation n.x = d.
   * @return The plane distance term d, which can be calculated as n | origin,
   */
  float
  getDistance() const;

  /**
   * @brief Signed distance from point to plane.
   * @param point The point to measure distance from.
   * @return The signed distance from the point to the plane. Positive if in front of the plane, negative if behind, and zero if on the plane.
   *         The distance is calculated as (n | (point - origin)), where n is the unit normal vector.
   */
  float
  signedDistanceTo(const Vector3f& point) const;

  /**
   * @brief Absolute distance from point to plane.
   * @param point The point to measure distance from.
   * @return The absolute distance from the point to the plane, which is the absolute 
   *         value of the signed distance.
   */
  float
  distanceTo(const Vector3f& point) const;

  /**
   * @brief Projects a point onto the plane.
   * @param point The point to project onto the plane.
   * @return The projection of the point onto the plane, which is calculated as 
   *         point - n * signedDistanceTo(point), where n is the unit normal vector.
   */
  Vector3f
  projectPoint(const Vector3f& point) const;

  /**
   * @brief Returns true if point is on plane within tolerance.
   * @param point The point to check for containment.
   * @param tolerance The distance tolerance for considering the point to be on the plane. 
   *        Default is a small number.
   * @return True if the absolute value of the signed distance from the point to the 
   *         plane is less than or equal to the specified tolerance, indicating that 
   *         the point is on or very close to the plane; false otherwise.
   */
  bool
  contains(const Vector3f& point, const float& tolerance = Math::SMALLNUMBER) const;

  /**
   * @brief Returns +1 if in front, -1 if behind, 0 if on plane.
   * @param point The point to check which side of the plane it is on.
   * @param tolerance The distance tolerance for considering the point to be on the plane.
   * @return 1 if the point is in front of the plane, -1 if behind, and 0 if within tolerance.
   */
  int32
  sideOf(const Vector3f& point, const float& tolerance = Math::SMALLNUMBER) const;

private:

  /**
   * @brief A point on the plane, which can be used as the plane's origin for calculations.
   */
  Vector3f m_origin;

  /**
   * @brief The normal vector of the plane, which defines the plane's orientation. 
   *        The plane is considered valid if this normal vector has non-zero magnitude.
   */
  Vector3f m_normal;
};


}

