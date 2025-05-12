#include "cyAABB.h"

#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

Vector3f
AABB::getDimensions() {
  return m_max - m_min;

}

Vector3f
AABB::getCenter() {
  return (m_min + m_max) * 0.5f;
}

String
AABB::toString() {
  return Utils::format("Min: %s, Max: %s", m_min.toString(), m_max.toString());
}

bool
AABB::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::AABB) {
    const AABB& otherAABB = static_cast<const AABB&>(other);
  return (m_min.x <= otherAABB.m_max.x && m_max.x >= otherAABB.m_min.x &&
          m_min.y <= otherAABB.m_max.y && m_max.y >= otherAABB.m_min.y &&
          m_min.z <= otherAABB.m_max.z && m_max.z >= otherAABB.m_min.z);
  }

  return false;
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

}