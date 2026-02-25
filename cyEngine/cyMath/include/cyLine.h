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
    return PRIMITIVE_TYPE::E::LINE;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets both line endpoints.
   */
  void
  setPoints(const Vector3f& a, const Vector3f& b) {
    m_a = a;
    m_b = b;
  }

  /**
   * @brief Sets point A.
   */
  void
  setPointA(const Vector3f& a) {
    m_a = a;
  }

  /**
   * @brief Sets point B.
   */
  void
  setPointB(const Vector3f& b) {
    m_b = b;
  }

  const Vector3f& 
  getPointA() const { return m_a; }

  const Vector3f&
  getPointB() const { return m_b; }

  /**
   * @brief Raw direction from A to B.
   */
  Vector3f
  getDirection() const {
    return m_b - m_a;
  }

  /**
   * @brief Normalized direction from A to B.
   */
  Vector3f
  getUnitDirection() const {
    const Vector3f d = getDirection();
    if (d.sqrMagnitude() <= Math::EPSILONF) {
      return Vector3f::ZERO;
    }
    return d.normalized();
  }

  /**
   * @brief Squared segment length.
   */
  float
  getSqrLength() const {
    return Vector3f::sqrDistance(m_a, m_b);
  }

  /**
   * @brief Segment length.
   */
  float
  getLength() const {
    return Vector3f::distance(m_a, m_b);
  }

  /**
   * @brief Midpoint between A and B.
   */
  Vector3f
  getCenter() const {
    return (m_a + m_b) * 0.5f;
  }

  /**
   * @brief Returns point along segment [A..B] from t in [0..1].
   */
  Vector3f
  getPointAt(const float& t) const {
    const float clampedT = Math::clamp(t, 0.0f, 1.0f);
    return m_a + (m_b - m_a) * clampedT;
  }

  /**
   * @brief Closest point on segment [A..B] to world position.
   */
  Vector3f
  closestPoint(const Vector3f& point) const {
    const Vector3f ab = m_b - m_a;
    const float denom = ab.sqrMagnitude();
    if (denom <= Math::EPSILONF) {
      return m_a;
    }

    const float t = Vector3f::dot(point - m_a, ab) / denom;
    return getPointAt(t);
  }

private:

  /**
   * 
   */
  Vector3f m_a;

  Vector3f m_b;
};

}


