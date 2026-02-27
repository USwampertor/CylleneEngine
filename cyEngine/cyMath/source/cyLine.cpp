#include "cyLine.h"
#include "cyMath.h"
#include "cyIntersections.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Line::toString() {
  return Utils::format("A: %s, B: %s", m_a, m_b);
}

bool
Line::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Line::setDimensions(const Vector3f& a, const Vector3f& b) {
  m_a = a;
  m_b = b;
}

Vector3f
Line::getDirection() const {
  return m_b - m_a;
}

Vector3f
Line::getUnitDirection() const {
  const Vector3f d = getDirection();
  if (d.sqrMagnitude() <= Math::EPSILONF) {
    return Vector3f::ZERO;
  }
  return d.normalized();
}

float
Line::getSqrLength() const {
  return Vector3f::sqrDistance(m_a, m_b);
}

float
Line::getLength() const {
  return Vector3f::distance(m_a, m_b);
}

Vector3f
Line::getCenter() const {
  return (m_a + m_b) * 0.5f;
}

Vector3f
Line::getPointAt(const float& t) const {
  const float clampedT = Math::clamp(t, 0.0f, 1.0f);
  return m_a + (m_b - m_a) * clampedT;
}

Vector3f
Line::closestPoint(const Vector3f& point) const {
  const Vector3f ab = m_b - m_a;
  const float denom = ab.sqrMagnitude();
  if (denom <= Math::EPSILONF) {
    return m_a;
  }

  const float t = Vector3f::dot(point - m_a, ab) / denom;
  return getPointAt(t);
}

}
