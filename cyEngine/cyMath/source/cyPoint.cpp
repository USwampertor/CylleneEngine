#include "cyPoint.h"

#include "cyMath.h"

#include "cyLine.h"
#include "cyPlane.h"
#include "cySphere.h"
#include "cyCapsule.h"
#include "cyAABB.h"
#include "cyOBB.h"
#include "cyIntersections.h"

namespace CYLLENE_SDK {

bool
Point::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}


String
Point::toString() {
  return static_cast<Vector3f*>(this)->toString();
}

void
Point::setDimensions(const float& nx, const float& ny, const float& nz) {
  x = nx;
  y = ny;
  z = nz;
}

void
Point::setDimensions(const Vector3f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3f
Point::toVector3f() const {
  return Vector3f(x, y, z);
}

float
Point::sqrDistanceTo(const Point& other) const {
  return Vector3f::sqrDistance(*this, other);
}

float
Point::distanceTo(const Point& other) const {
  return Vector3f::distance(*this, other);
}

float
Point::sqrDistanceTo(const Vector3f& other) const {
  return Vector3f::sqrDistance(*this, other);
}

float
Point::distanceTo(const Vector3f& other) const {
  return Vector3f::distance(*this, other);
}

Point
Point::midpoint(const Point& other) const {
  return Point((x + other.x) * 0.5f,
    (y + other.y) * 0.5f,
    (z + other.z) * 0.5f);
}

Point
Point::lerp(const Point& other, const float& t) const {
  const float clampedT = Math::clamp(t, 0.0f, 1.0f);
  return Point(x + (other.x - x) * clampedT,
    y + (other.y - y) * clampedT,
    z + (other.z - z) * clampedT);
}


}
