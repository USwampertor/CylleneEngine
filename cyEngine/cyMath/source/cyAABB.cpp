#include "cyAABB.h"

#include "cyMath.h"

#include "cyCapsule.h"
#include "cyCone.h"
#include "cyCylinder.h"
#include "cyFrustrum.h"
#include "cyLine.h"
#include "cyOBB.h"
#include "cyPlane.h"
#include "cyPoint.h"
#include "cyRay.h"
#include "cyRect.h"
#include "cySphere.h"

#include <cyUtilities.h>


namespace CYLLENE_SDK {

const Vector3f
AABB::getDimensions() const {
  return m_max - m_min;

}

void
AABB::setBoundaries(const Vector3f& min, const Vector3f& max) {
  m_min = min;
  m_max = max;
}

const Vector3f
AABB::getCenter() const {
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
  else if (type == +PRIMITIVE_TYPE::E::CAPSULE) {
    // Check if the capsule intersects with the AABB
    const Capsule& capsule = static_cast<const Capsule&>(other);
    Vector3f closestPoint = Vector3f::ZERO;
    closestPoint.x = Math::max(m_min.x, Math::min(capsule.getPointA().x, m_max.x));
    closestPoint.y = Math::max(m_min.y, Math::min(capsule.getPointA().y, m_max.y));
    closestPoint.z = Math::max(m_min.z, Math::min(capsule.getPointA().z, m_max.z));
    Vector3f difference = closestPoint - capsule.getPointA();
    float distanceSquared = Vector3f::dot(difference, difference);
    return distanceSquared < (capsule.m_radius * capsule.m_radius);
  }
  else if (type == +PRIMITIVE_TYPE::E::CONE) {
    // Check if the cone intersects with the AABB
    const Cone& cone = static_cast<const Cone&>(other);
    Vector3f closestPoint = Vector3f::ZERO;
    closestPoint.x = Math::max(m_min.x, Math::min(cone.getHeight(), m_max.x));
    closestPoint.y = Math::max(m_min.y, Math::min(cone.getHeight(), m_max.y));
    closestPoint.z = Math::max(m_min.z, Math::min(cone.getHeight(), m_max.z));
    Vector3f difference = closestPoint - Vector3f(0.0f, 0.0f, cone.getHeight());
    float distanceSquared = Vector3f::dot(difference, difference);
    return distanceSquared < (cone.getRadius() * cone.getRadius());
  }
  else if (type == +PRIMITIVE_TYPE::E::CYLINDER) {
        // Check if the cylinder intersects with the AABB
    const Cylinder& cylinder = static_cast<const Cylinder&>(other);
    Vector3f closestPoint = Vector3f::ZERO;
    closestPoint.x = Math::max(m_min.x, Math::min(cylinder.getHeight(), m_max.x));
    closestPoint.y = Math::max(m_min.y, Math::min(cylinder.getHeight(), m_max.y));
    closestPoint.z = Math::max(m_min.z, Math::min(cylinder.getHeight(), m_max.z));
    Vector3f difference = closestPoint - Vector3f(0.0f, 0.0f, cylinder.getHeight());
    float distanceSquared = Vector3f::dot(difference, difference);
    return distanceSquared < (cylinder.getRadius() * cylinder.getRadius());
  }
  else if (type == +PRIMITIVE_TYPE::E::FRUSTUM) {
    // Check if the frustum intersects with the AABB
    const Frustrum& frustum = static_cast<const Frustrum&>(other);
    // For simplicity, we can check if the center of the frustum is inside the AABB
    return contains(frustum.getCenter());
  }
  else if (type == +PRIMITIVE_TYPE::E::LINE) {
    // Check if the line intersects with the AABB
    const Line& line = static_cast<const Line&>(other);
    Vector3f direction = line.getPointB()  - line.getPointA();
    Vector3f invDirection = Vector3f(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);
    Vector3f t1 = (m_min - line.getPointA()) * invDirection;
    Vector3f t2 = (m_max - line.getPointA()) * invDirection;
    
    float tmin = Math::max(Math::max(Math::min(t1.x, t2.x), 
                                     Math::min(t1.y, t2.y)), 
                           Math::min(t1.z, t2.z));
    
    float tmax = Math::min(Math::min(Math::max(t1.x, t2.x), 
                                     Math::max(t1.y, t2.y)), 
                           Math::max(t1.z, t2.z));

    return tmax >= 0.0f && tmin <= 1.0f && tmin <= tmax;
  }
  else if (type == +PRIMITIVE_TYPE::E::OBB) {
    // Check if the OBB intersects with the AABB
    const OBB& obb = static_cast<const OBB&>(other);
    // For simplicity, we can check if the center of the OBB is inside the AABB
    return contains(obb.getCenter());
  }
  else if (type == +PRIMITIVE_TYPE::E::PLANE) {
    // Check if the plane intersects with the AABB
    const Plane& plane = static_cast<const Plane&>(other);
    // For simplicity, we can check if the center of the AABB is on the positive side of the plane
    return (plane.getNormal() | getCenter()) >= plane.getDistance();
  }
  else if (type == +PRIMITIVE_TYPE::E::POINT) {
    // Check if the point is inside the AABB
    const Point& p = static_cast<const Point&>(other);
    const Vector3f& point = static_cast<const Vector3f&>(p);
    return contains(point);
  }
  else if (type == +PRIMITIVE_TYPE::E::RAY) {
    // Check if the ray intersects with the AABB
    const Ray& ray = static_cast<const Ray&>(other);
    Vector3f invDirection = Vector3f(1.0f / ray.m_direction.x, 1.0f / ray.m_direction.y, 1.0f / ray.m_direction.z);
    Vector3f t1 = (m_min - ray.m_origin) * invDirection;
    Vector3f t2 = (m_max - ray.m_origin) * invDirection;
    
    float tmin = Math::max(Math::max(Math::min(t1.x, t2.x), 
                                     Math::min(t1.y, t2.y)), 
                           Math::min(t1.z, t2.z));
    
    float tmax = Math::min(Math::min(Math::max(t1.x, t2.x), 
                                     Math::max(t1.y, t2.y)), 
                           Math::max(t1.z, t2.z));
    return tmax >= 0.0f && tmin <= tmax;
  }
  else if (type == +PRIMITIVE_TYPE::E::RECT) {
    // Check if the rectangle intersects with the AABB
    const Rect& rect = static_cast<const Rect&>(other);
    // For simplicity, we can check if the center of the rectangle is inside the AABB
    return contains(rect.getCenter());
  }
  else if (type == +PRIMITIVE_TYPE::E::SPHERE) {
    // Check if the sphere intersects with the AABB
    const Sphere& sphere = static_cast<const Sphere&>(other);
    Vector3f closestPoint = Vector3f::ZERO;
    closestPoint.x = Math::max(m_min.x, Math::min(sphere.m_center.x, m_max.x));
    closestPoint.y = Math::max(m_min.y, Math::min(sphere.m_center.y, m_max.y));
    closestPoint.z = Math::max(m_min.z, Math::min(sphere.m_center.z, m_max.z));
    Vector3f difference = closestPoint - sphere.m_center;
    float distanceSquared = Vector3f::dot(difference, difference);
    return distanceSquared < (sphere.m_radius * sphere.m_radius);
  }

  return false;
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