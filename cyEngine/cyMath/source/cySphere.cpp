#include "cySphere.h"
#include "cyMath.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Sphere::toString() {
  return Utils::format("Center: %s, radius: %2.f", m_center.toString().c_str(), m_radius);
}

bool
Sphere::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

float
Sphere::getDiameter() const {
  return m_radius * 2.0f;
}

bool
Sphere::isValid() const {
  return m_radius >= 0.0f;
}

float
Sphere::getCircumference() const {
  return 2.0f * Math::PI * m_radius;
}

float
Sphere::getSurfaceArea() const {
  return 4.0f * Math::PI * Math::sqr(m_radius);
}

float
Sphere::getVolume() const {
  return (4.0f / 3.0f) * Math::PI * Math::pow(m_radius, 3.0f);
}

bool
Sphere::contains(const Vector3f& point) const {
  return Vector3f::sqrDistance(point, m_center) <= Math::sqr(m_radius);
}

Vector3f
Sphere::closestPoint(const Vector3f& point) const {
  const Vector3f toPoint = point - m_center;
  if (toPoint.sqrMagnitude() <= Math::EPSILONF) {
    return m_center + Vector3f::RIGHT * m_radius;
  }
  return m_center + toPoint.normalized() * m_radius;
}

void
Sphere::expandTo(const Vector3f& pos) {
  const float distSqr = Vector3f::sqrDistance(pos, m_center);
  if (distSqr > Math::sqr(m_radius)) {
    const float dist = Math::sqrt(distSqr);
    const float newRadius = (m_radius + dist) * 0.5f;
    const Vector3f dir = (pos - m_center) / dist;
    m_center += dir * (newRadius - m_radius);
    m_radius = newRadius;
  }

}

}
