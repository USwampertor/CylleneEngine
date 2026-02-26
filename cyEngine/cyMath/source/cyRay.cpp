#include "cyRay.h"
#include "cyIntersections.h"
#include <cyUtilities.h>


namespace CYLLENE_SDK
{
String
Ray::toString() {
  return Utils::format("Origin: %s, Direction: %s", m_origin.toString().c_str(), m_direction.toString().c_str());
}

bool
Ray::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Ray::setDimensions(const Vector3f& origin, const Vector3f& direction) {
  m_origin = origin;
  m_direction = direction;
}

bool
Ray::isValid() const {
  return m_direction.sqrMagnitude() > Math::EPSILONF;
}

Vector3f
Ray::getUnitDirection() const {
  if (!isValid()) {
    return Vector3f::ZERO;
  }
  return m_direction.normalized();
}

void
Ray::normalizeDirection() {
  if (isValid()) {
    m_direction.normalize();
  }
}

Vector3f
Ray::getPointAt(const float& t) const {
  return m_origin + m_direction * t;
}

float
Ray::projectParameter(const Vector3f& point) const {
  const float denom = m_direction.sqrMagnitude();
  if (denom <= Math::EPSILONF) {
    return 0.0f;
  }
  return Math::max(0.0f, Vector3f::dot(point - m_origin, m_direction) / denom);
}

Vector3f
Ray::closestPoint(const Vector3f& point) const {
  return getPointAt(projectParameter(point));
}

float
Ray::sqrDistanceToPoint(const Vector3f& point) const {
  const Vector3f c = closestPoint(point);
  return Vector3f::sqrDistance(c, point);
}

float
Ray::distanceToPoint(const Vector3f& point) const {
  return Vector3f::distance(closestPoint(point), point);
}

}
