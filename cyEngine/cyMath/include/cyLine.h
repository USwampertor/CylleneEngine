#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

/*
 *	@class  Line 
 *	@brief  Defines two positions in space bound to each other
 *
 */
class CY_MATH_EXPORT Line : public Primitive
{
public:
  /*
   *	@brief  Default constructor
   */
  Line() 
    : Primitive(Line::staticType()),
      m_a(Vector3f::ZERO),
      m_b(Vector3f::ZERO) {}

  /*
   *	@brief  Constructor taking two vectors
   *	@param  const Point& a - Point a
   *	@param  const Point& b - Point b
   */
  Line(const Vector3f& a, const Vector3f& b)
    : Primitive(Line::staticType()),
      m_a(a),
      m_b(b) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::eLINE;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets both line endpoints.
   * @param a The new position for point A of the line segment.
   * @param b The new position for point B of the line segment.
   */
  void
  setDimensions(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Sets point A.
   * @param a The new position for point A of the line segment.
   */
  void
  setPointA(const Vector3f& a) {
    m_a = a;
  }

  /**
   * @brief Sets point B.
   * @param b The new position for point B of the line segment.
   */
  void
  setPointB(const Vector3f& b) {
    m_b = b;
  }

  /**
   * @brief Gets point A.
   * @return The current position of point A of the line segment.
   */
  const Vector3f& 
  getPointA() const { return m_a; }

  /**
   * @brief Gets point B.
   * @return The current position of point B of the line segment.
   */
  const Vector3f&
  getPointB() const { return m_b; }

  /**
   * @brief Raw direction from A to B.
   * @return The vector from point A to point B, which may not be normalized 
   *         and can have a length of zero if A and B are the same point.
   */
  Vector3f
  getDirection() const;

  /**
   * @brief Normalized direction from A to B.
   * @return The normalized vector from point A to point B, which will have a length 
   *         of 1 unless A and B are the same point, in which case it will return the zero vector.
   */
  Vector3f
  getUnitDirection() const;

  /**
   * @brief Squared segment length.
   * @return The squared distance between point A and point B, 
   *         which is more efficient to compute than the actual length and can be used 
   *         for comparisons without needing to take a square root.
   */
  float
  getSqrLength() const;

  /**
   * @brief Segment length.
   * @return The distance between point A and point B, calculated as the square root 
   *         of the squared distance
   */
  float
  getLength() const;

  /**
   * @brief Midpoint between A and B.
   * @return The midpoint between point A and point B, 
   *         calculated as the average of the two points,
   */
  Vector3f
  getCenter() const;

  /**
   * @brief Returns point along segment [A..B] from t in [0..1].
   * @param t The parameter t, which should be in the range [0, 1],
   *          where 0 corresponds to point A and 1 corresponds to point B.
   * @return The point along the line segment from A to B corresponding to the parameter t,
   */
  Vector3f
  getPointAt(const float& t) const;

  /**
   * @brief Closest point on segment [A..B] to world position.
   * @param point The world position to which we want to find 
   *        the closest point on the line segment.
   * @return The closest point on the line segment [A..B] to the given world position
   */
  Vector3f
  closestPoint(const Vector3f& point) const;

private:

  /**
   * @brief Point A of the line segment, representing one endpoint of the line.
   */
  Vector3f m_a;

  /**
   * @brief Point B of the line segment, representing the other endpoint of the line.
   */
  Vector3f m_b;
};

}


