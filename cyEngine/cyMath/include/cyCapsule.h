#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"


namespace CYLLENE_SDK {


/**
 * @class Capsule
 * @brief Capsule defined by two endpoints and a radius.
 */
class CY_MATH_EXPORT Capsule : public Primitive
{
public:
  Capsule()
    : Primitive(Capsule::staticType()),
      m_pointA(Vector3f::ZERO),
      m_pointB(Vector3f::ZERO),
      m_radius(0.0f) {}

  Capsule(const Capsule& other)
    : Primitive(Capsule::staticType()),
      m_pointA(other.m_pointA),
      m_pointB(other.m_pointB),
      m_radius(other.m_radius) {}

  Capsule(const Vector3f& a, const Vector3f& b, const float& radius)
    : Primitive(Capsule::staticType()),
      m_pointA(a),
      m_pointB(b),
      m_radius(radius) {}

  static PRIMITIVE_TYPE::E 
  staticType() {
    return PRIMITIVE_TYPE::E::eCAPSULE;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Sets both segment endpoints.
   * @param a First segment point.
   * @param b Second segment point.
   */
  void
  setSegment(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Sets all capsule dimensions at once.
   * @param a First segment point.
   * @param b Second segment point.
   * @param radius Capsule radius.
   */
  void
  setDimensions(const Vector3f& a, const Vector3f& b, const float& radius);

  /**
   * @brief sets point A
   * @param a First segment point.
   */
  void
  setPointA(const Vector3f& a) {
    m_pointA = a;
  }

  /**
   * @brief sets point B
   * @param b Second segment point.
   */
   void
    setPointB(const Vector3f& b) {
     m_pointB = b;
   }


  /**
   * @brief Sets capsule radius.
   * @param radius Capsule radius.
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Gets first segment point.
   * @return a First segment point.
   */
  const Vector3f&
  getPointA() const {
    return m_pointA;
  }

  /**
   * @brief Gets second segment point.
   * @return b Second segment point.
   */
  const Vector3f&
  getPointB() const {
    return m_pointB;
  }

  /**
   * @brief Gets capsule radius.
   * @return radius Capsule radius.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Returns true when radius is positive.
   * @return True if the capsule is valid, false otherwise.
   */
  bool
  isValid() const;

  /**
   * @brief Segment direction from A to B.
   * @return Normalized direction vector of the segment from point A to point B.
   */
  Vector3f
  getAxis() const;

  /**
   * @brief Segment center.
   * @return Center point of the segment, calculated as the midpoint between 
   *         point A and point B.
   */
  Vector3f
  getCenter() const;

  /**
   * @brief Segment length between endpoints.
   * @return Length of the segment defined by points A and B, 
   *         calculated as the distance between them.
   */
  float
  getSegmentLength() const;

  /**
   * @brief Returns a point along segment [A..B] from t in [0..1].
   * @param t Parameter in the range [0, 1] that specifies the position along 
   *        the segment, where 0 corresponds to point A and 1 corresponds to point B.
   * @return Point on the segment corresponding to the parameter t, 
   *         where t=0 returns point A, t=1 returns point B, and values in between 
   *         return points linearly interpolated along the segment.
   */
  Vector3f
  getPointAt(const float& t) const;

  /**
   * @brief Closest point on segment [A..B] to a world position.
   * @param point The world position to find the closest point to.
   * @return The point on the segment defined by points A and B that is closest to 
   *         the given world position. This is calculated by projecting the point onto 
   *         the line defined by A and B, clamping the result to the segment, and 
   *         returning the corresponding point on the segment.
   */
  Vector3f
  closestPointOnSegment(const Vector3f& point) const;

  /**
   * @brief Volume of the cylindrical middle section only.
   * @return Volume of the cylindrical part of the capsule, calculated as the area 
   *         of the circular cross-section (π * radius^2) multiplied by the length of 
   *         the segment between points A and B.
   */
  float
  getCylinderVolume() const;

  /**
   * @brief Volume contributed by the two hemispherical caps.
   * @return Volume of the two hemispherical caps combined, calculated as the volume 
   *         of a sphere with the same radius (4/3 * π * radius^3) since the two 
   *         hemispheres together form a full sphere.
   */
  float
  getCapsVolume() const;

  /**
   * @brief Total capsule volume.
   * @return Total volume of the capsule, calculated as the sum of the cylindrical volume
   */
  float
  getVolume() const;

  /**
   * @brief Surface area of the cylindrical middle section only.
   * @return Surface area of the cylindrical part of the capsule, calculated as the 
   *         circumference of the circular cross-section (2 * π * radius) multiplied by 
   *         the length of the segment between points A and B, representing the lateral 
   *         surface area of the cylinder.
   */
  float
  getCylinderSurfaceArea() const;

  /**
   * @brief Surface area contributed by both hemispherical caps.
   * @return Surface area of the two hemispherical caps combined, 
   *         calculated as the surface area of a sphere with the same radius 
   *         (4 * π * radius^2) since the two hemispheres together form a full sphere.
   */
  float
  getCapsSurfaceArea() const;

  /**
   * @brief Total capsule surface area.
   * @return Total surface area of the capsule, calculated as the sum of 
   *         the cylindrical surface area
   */
  float
  getSurfaceArea() const;

public:

  /**
   * One of the points of the capsule
   */
  Vector3f m_pointA;

  /**
   * The other point of the capsule
   */
  Vector3f m_pointB;
  
  /**
   * The radius of the capsule
   */
  float m_radius;

};

}

