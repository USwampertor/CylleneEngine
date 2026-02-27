#include "cyAABB.h"

#include "cyMath.h"

#include "cyCapsule.h"
#include "cyCone.h"
#include "cyCylinder.h"
#include "cyFrustum.h"
#include "cyLine.h"
#include "cyOBB.h"
#include "cyPlane.h"
#include "cyPoint.h"
#include "cyRay.h"
#include "cyRect.h"
#include "cySphere.h"
#include "cyIntersections.h"

#include <cyUtilities.h>


namespace CYLLENE_SDK {

  
String
AABB::toString() {
  return Utils::format("Min: %s, Max: %s", m_min.toString(), m_max.toString());
}

bool
AABB::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}


const Vector3f
AABB::getDimensions() const {
  return m_max - m_min;

}

void
AABB::setDimensions(const Vector3f& min, const Vector3f& max) {
  m_min = min;
  m_max = max;
}

const Vector3f
AABB::getCenter() const {
  return (m_min + m_max) * 0.5f;
}

bool
AABB::isValid() const {
  return m_min.x <= m_max.x &&
    m_min.y <= m_max.y &&
    m_min.z <= m_max.z;
}

Vector3f
AABB::getHalfExtents() const {
  return getDimensions() * 0.5f;
}

float
AABB::getSurfaceArea() const {
  if (!isValid()) {
    return 0.0f;
  }

  const Vector3f d = getDimensions();
  return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

float
AABB::getVolume() const {
  if (!isValid()) {
    return 0.0f;
  }

  const Vector3f d = getDimensions();
  return d.x * d.y * d.z;
}

bool
AABB::contains(const Vector3f& point) const {
  return point.x >= m_min.x && point.x <= m_max.x &&
    point.y >= m_min.y && point.y <= m_max.y &&
    point.z >= m_min.z && point.z <= m_max.z;
}

bool
AABB::contains(const AABB& other) const {
  return contains(other.m_min) && contains(other.m_max);
}

void
AABB::expandTo(const Vector3f& pos) {
  m_min.x = Math::min(m_min.x, pos.x);
  m_min.y = Math::min(m_min.y, pos.y);
  m_min.z = Math::min(m_min.z, pos.z);
  m_max.x = Math::max(m_max.x, pos.x);
  m_max.y = Math::max(m_max.y, pos.y);
  m_max.z = Math::max(m_max.z, pos.z);
}

void
AABB::expandTo(const AABB& other) {
  m_min.x = Math::min(m_min.x, other.m_min.x);
  m_min.y = Math::min(m_min.y, other.m_min.y);
  m_min.z = Math::min(m_min.z, other.m_min.z);
  m_max.x = Math::max(m_max.x, other.m_max.x);
  m_max.y = Math::max(m_max.y, other.m_max.y);
  m_max.z = Math::max(m_max.z, other.m_max.z);
}


void
AABB::expandTo(const Vector3f& min, const Vector3f& max) {
  expandTo(min);
  expandTo(max);
}

void
AABB::expandTo(const float& x, const float& y, const float& z) {
  expandTo(Vector3f(x, y, z));
}

}
