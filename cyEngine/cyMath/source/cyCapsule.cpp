#include "cyCapsule.h"

#include <cyUtilities.h>

#include "cyAABB.h"
#include "cyCone.h"
#include "cyCylinder.h"
#include "cyFrustum.h"
#include "cyLine.h"
#include "cyMath.h"
#include "cyOBB.h"
#include "cyPlane.h"
#include "cyPoint.h"
#include "cyRay.h"
#include "cyRect.h"
#include "cySphere.h"
#include "cyIntersections.h"

namespace CYLLENE_SDK {
  
String
Capsule::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f", 
                       m_pointA.toString(), 
                       m_pointB.toString(), 
                       m_radius);
}

bool
Capsule::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}


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


}
