#include "cyOBB.h"
#include "cyMath.h"
#include "cyMatrix4.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
OBB::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f");
}

bool
OBB::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

Vector3f
OBB::getDimensions() const {
  return m_hExtents * 2.0f;
}

void
OBB::setDimensions(const Vector3f& center,
                   const Vector3f& halfExtents, 
                   const Quaternion& orientation) {
  m_center      = center;
  m_hExtents    = halfExtents;
  m_orientation = orientation;
}

bool
OBB::isValid() const {
  return m_hExtents.x >= 0.0f &&
         m_hExtents.y >= 0.0f &&
         m_hExtents.z >= 0.0f;
}

Vector3f
OBB::getRightAxis() const {
  return m_orientation.getMatrix4Rotation().getRightVector();
}

Vector3f
OBB::getUpAxis() const {
  return m_orientation.getMatrix4Rotation().getUpVector();
}

Vector3f
OBB::getForwardAxis() const {
  return m_orientation.getMatrix4Rotation().getForwardVector();
}

float
OBB::getVolume() const {
  const Vector3f d = getDimensions();
  return d.x * d.y * d.z;
}

float
OBB::getSurfaceArea() const {
  const Vector3f d = getDimensions();
  return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

Vector3f
OBB::closestPoint(const Vector3f& point) const {
  const Vector3f d = point - m_center;
  const Vector3f right = getRightAxis();
  const Vector3f up = getUpAxis();
  const Vector3f forward = getForwardAxis();

  float x = Math::clamp(Vector3f::dot(d, right), -m_hExtents.x, m_hExtents.x);
  float y = Math::clamp(Vector3f::dot(d, up), -m_hExtents.y, m_hExtents.y);
  float z = Math::clamp(Vector3f::dot(d, forward), -m_hExtents.z, m_hExtents.z);

  return m_center + right * x + up * y + forward * z;
}

bool
OBB::contains(const Vector3f& point) const {
  const Vector3f d = point - m_center;
  const Vector3f right = getRightAxis();
  const Vector3f up = getUpAxis();
  const Vector3f forward = getForwardAxis();

  const float x = Vector3f::dot(d, right);
  const float y = Vector3f::dot(d, up);
  const float z = Vector3f::dot(d, forward);

  return Math::abs(x) <= m_hExtents.x &&
    Math::abs(y) <= m_hExtents.y &&
    Math::abs(z) <= m_hExtents.z;
}

void
OBB::expandTo(const Vector3f& pos) {
  const Vector3f closest = closestPoint(pos);
  const Vector3f delta = pos - closest;
  if (delta.x > 0) {
    m_hExtents.x += delta.x;
    m_center.x += delta.x * 0.5f;
  } else if (delta.x < 0) {
    m_hExtents.x -= delta.x;
    m_center.x += delta.x * 0.5f;
  }
  if (delta.y > 0) {
    m_hExtents.y += delta.y;
    m_center.y += delta.y * 0.5f;
  } else if (delta.y < 0) {
    m_hExtents.y -= delta.y;
    m_center.y += delta.y * 0.5f;
  }
  if (delta.z > 0) {
    m_hExtents.z += delta.z;
    m_center.z += delta.z * 0.5f;
  } else if (delta.z < 0) {
    m_hExtents.z -= delta.z;
    m_center.z += delta.z * 0.5f;
  }
}

void
OBB::expandTo(const OBB& other) {
  const Vector3f corners[8] = {
    other.m_center + Vector3f( other.m_hExtents.x,  other.m_hExtents.y,  other.m_hExtents.z),
    other.m_center + Vector3f(-other.m_hExtents.x,  other.m_hExtents.y,  other.m_hExtents.z),
    other.m_center + Vector3f( other.m_hExtents.x, -other.m_hExtents.y,  other.m_hExtents.z),
    other.m_center + Vector3f(-other.m_hExtents.x, -other.m_hExtents.y,  other.m_hExtents.z),
    other.m_center + Vector3f( other.m_hExtents.x,  other.m_hExtents.y, -other.m_hExtents.z),
    other.m_center + Vector3f(-other.m_hExtents.x,  other.m_hExtents.y, -other.m_hExtents.z),
    other.m_center + Vector3f( other.m_hExtents.x, -other.m_hExtents.y, -other.m_hExtents.z),
    other.m_center + Vector3f(-other.m_hExtents.x, -other.m_hExtents.y, -other.m_hExtents.z)
  };
  for (const auto& corner : corners) {
    expandTo(corner);
  }
}


float 
OBB::projectOntoAxis(const Vector3f& axis) const {
  // Project the half-extents of the OBB onto the axis
  return std::abs(m_hExtents.x * 
                  Vector3f::dot(axis, m_orientation.getMatrix4Rotation().getRightVector())) +
         std::abs(m_hExtents.y * 
                  Vector3f::dot(axis, m_orientation.getMatrix4Rotation().getUpVector())) +
         std::abs(m_hExtents.z * 
                  Vector3f::dot(axis, m_orientation.getMatrix4Rotation().getForwardVector()));
}

}
