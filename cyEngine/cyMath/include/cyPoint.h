/**
 * @file cyPoint.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Point.
 */
#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"


namespace CYLLENE_SDK
{

/**
 * @class Point
 * @brief Represents a point in 3D space.
 */
class CY_MATH_EXPORT Point : public Primitive, public Vector3f
{
public:
  Point() : Primitive(Point::staticType()) {}
  
  Point(float nx, float ny, float nz) 
    : Primitive(Point::staticType()), 
      Vector3f(nx, ny, nz) {}

  Point(const Vector3f& v) 
    : Primitive(Point::staticType()), 
      Vector3f(v) {}

  static PRIMITIVE_TYPE::E 
  staticType() { return PRIMITIVE_TYPE::E::ePOINT; }

  /**
   * @brief Checks if this point intersects another primitive.
   * @param other Primitive to test.
   * @return True if intersecting.
   */
  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Converts point to string representation.
   * @return Formatted point string.
   */
  virtual String
  toString() override;

  /**
   * @brief Sets point coordinates.
   * @param nx New x coordinate.
   * @param ny New y coordinate.
   * @param nz New z coordinate.
   */
  void
  setDimensions(const float& nx, const float& ny, const float& nz);

  /**
   * @brief Sets from a Vector3f.
   * @param v Vector3f containing new coordinates.
   */
  void
  setDimensions(const Vector3f& v);

  /**
   * @brief Returns point as Vector3f value.
   * @return A Vector3f with the same coordinates as this point.
   */
  Vector3f
  toVector3f() const;

  /**
   * @brief Squared distance to another point.
   * @param other The other point to which the distance is calculated.
   * @return The squared distance from this point to the other point.
   */
  float
  sqrDistanceTo(const Point& other) const;

  /**
   * @brief Distance to another point.
   * @param other The other point to which the distance is calculated.
   * @return The distance from this point to the other point.
   */
  float
  distanceTo(const Point& other) const;

  /**
   * @brief Squared distance to a world position.
   * @param other The world position to which the distance is calculated.
   * @return The squared distance from this point to the world position.
   */
  float
  sqrDistanceTo(const Vector3f& other) const;

  /**
   * @brief Distance to a world position.
   * @param other The world position to which the distance is calculated.
   * @return The distance from this point to the world position.
   */
  float
  distanceTo(const Vector3f& other) const;

  /**
   * @brief Midpoint between this and another point.
   * @param other The other point to which the midpoint is calculated.
   * @return A new Point that is the midpoint between this point and the other point.
   */
  Point
  midpoint(const Point& other) const;

  /**
   * @brief Linear interpolation from this point to another.
   * @param other The other point to which the interpolation is calculated.
   * @param t The interpolation factor (0.0f = this point, 1.0f = other point).
   * @return A new Point that is the result of linearly interpolating between this point
   *         and the other point by factor t.
   */
  Point
  lerp(const Point& other, const float& t) const;

};

}


