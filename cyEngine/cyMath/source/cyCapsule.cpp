#include "cyCapsule.h"
#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

void
Capsule::setSegment(const Vector3f& a, const Vector3f& b) {
  m_pointA = a;
  m_pointB = b;
}

void
Capsule::setDimensions(const Vector3f& a, const Vector3f& b, const float& radius) {
  m_pointA = a;
  m_pointB = b;
  m_radius = radius;
}

bool
Capsule::isValid() const {
  return m_radius > Math::EPSILONF;
}

Vector3f
Capsule::getAxis() const {
  return m_pointB - m_pointA;
}

Vector3f
Capsule::getCenter() const {
  return (m_pointA + m_pointB) * 0.5f;
}

float
Capsule::getSegmentLength() const {
  return Vector3f::distance(m_pointA, m_pointB);
}

Vector3f
Capsule::getPointAt(const float& t) const {
  const float clampedT = Math::clamp(t, 0.0f, 1.0f);
  return m_pointA + (m_pointB - m_pointA) * clampedT;
}

Vector3f
Capsule::closestPointOnSegment(const Vector3f& point) const {
  const Vector3f ab = m_pointB - m_pointA;
  const float denom = ab.sqrMagnitude();
  if (denom <= Math::EPSILONF) {
    return m_pointA;
  }

  const float t = Vector3f::dot(point - m_pointA, ab) / denom;
  return getPointAt(t);
}

float
Capsule::getCylinderVolume() const {
  return Math::PI * Math::sqr(m_radius) * getSegmentLength();
}

float
Capsule::getCapsVolume() const {
  return (4.0f / 3.0f) * Math::PI * Math::pow(m_radius, 3.0f);
}

float
Capsule::getVolume() const {
  return getCylinderVolume() + getCapsVolume();
}

float
Capsule::getCylinderSurfaceArea() const {
  return 2.0f * Math::PI * m_radius * getSegmentLength();
}

float
Capsule::getCapsSurfaceArea() const {
  return 4.0f * Math::PI * Math::sqr(m_radius);
}

float
Capsule::getSurfaceArea() const {
  return getCylinderSurfaceArea() + getCapsSurfaceArea();
}

String
Capsule::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f", 
                       m_pointA.toString(), 
                       m_pointB.toString(), 
                       m_radius);
}

bool
Capsule::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::CAPSULE) {
    const Capsule& otherCapsule = static_cast<const Capsule&>(other);
    
    Vector3f d1 = m_pointB - m_pointA;
    Vector3f d2 = otherCapsule.m_pointB - otherCapsule.m_pointA;
    Vector3f r = m_pointA - otherCapsule.m_pointA;
    float a = d1 | d1;
    float b = d1 | d2;
    float c = d2 | d2;
    float d = d1 | r;
    float e = d2 | r;

    float denom = a * c - b * b; // Denominator for parametric equations

    float s = (b * e - c * d) / denom; // Parametric point on segment 1
    float t = (a * e - b * d) / denom; // Parametric point on segment 2

    // Clamp s and t to [0, 1] to stay on the segments
    s = Math::max(0.0f, Math::min(1.0f, s));
    t = Math::max(0.0f, Math::min(1.0f, t));

    // Compute closest points
    Vector3f p1 = m_pointA + d1 * s;
    Vector3f p2 = otherCapsule.m_pointA + d2 * t;

    float distSqr = Vector3f::sqrDistance(p1, p2);
    float sumRadii = m_radius + otherCapsule.m_radius;
    return (distSqr <= sumRadii * sumRadii);

  }

  return false;
}



}