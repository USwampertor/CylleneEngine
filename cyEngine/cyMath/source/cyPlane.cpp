#include "cyPlane.h"
#include "cyMath.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Plane::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f");
}

bool
Plane::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Plane::setDimensions(const Vector3f& origin, const Vector3f& normal) {
  m_origin = origin;
  m_normal = normal;
}

bool
Plane::isValid() const {
  return m_normal.sqrMagnitude() > Math::EPSILONF;
}

Vector3f
Plane::getUnitNormal() const {
  if (!isValid()) {
    return Vector3f::ZERO;
  }
  return m_normal.normalized();
}

void
Plane::normalize() {
  if (isValid()) {
    m_normal.normalize();
  }
}

float
Plane::getConstantD() const {
  const Vector3f n = getUnitNormal();
  return -(n | m_origin);
}

float
Plane::getDistance() const {
  const Vector3f n = getUnitNormal();
  if (n.isZero()) {
    return 0.0f;
  }
  return n | m_origin;
}

float
Plane::signedDistanceTo(const Vector3f& point) const {
  const Vector3f n = getUnitNormal();
  if (n.isZero()) {
    return 0.0f;
  }
  return (n | (point - m_origin));
}

float
Plane::distanceTo(const Vector3f& point) const {
  return Math::abs(signedDistanceTo(point));
}

Vector3f
Plane::projectPoint(const Vector3f& point) const {
  const Vector3f n = getUnitNormal();
  if (n.isZero()) {
    return point;
  }
  const float dist = signedDistanceTo(point);
  return point - n * dist;
}

bool
Plane::contains(const Vector3f& point, const float& tolerance) const {
  return Math::abs(signedDistanceTo(point)) <= tolerance;
}

int32
Plane::sideOf(const Vector3f& point, const float& tolerance) const {
  const float s = signedDistanceTo(point);
  if (s > tolerance) {
    return 1;
  }
  if (s < -tolerance) {
    return -1;
  }
  return 0;
}

}
