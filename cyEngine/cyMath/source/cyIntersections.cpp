#include "cyIntersections.h"

#include "cyAABB.h"
#include "cyCapsule.h"
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
#include <limits>

namespace CYLLENE_SDK {

  namespace COLLISIONS {

  float
  pointSegmentSqrDistance(const Vector3f& p, const Vector3f& a, const Vector3f& b) {
    const Vector3f ab = b - a;
    const float denom = ab.sqrMagnitude();
    if (denom <= Math::EPSILONF) {
      return Vector3f::sqrDistance(p, a);
    }
    const float t = Math::clamp(Vector3f::dot(p - a, ab) / denom, 0.0f, 1.0f);
    return Vector3f::sqrDistance(p, a + ab * t);
  }

  float
  pointRaySqrDistance(const Vector3f& p, const Ray& ray) {
    const Vector3f d = ray.getDirection();
    const float denom = d.sqrMagnitude();
    if (denom <= Math::EPSILONF) {
      return Vector3f::sqrDistance(p, ray.getOrigin());
    }

    const float t = Math::max(0.0f, Vector3f::dot(p - ray.getOrigin(), d) / denom);
    return Vector3f::sqrDistance(p, ray.getOrigin() + d * t);
  }

  float
  segmentSegmentSqrDistance(const Vector3f& p1,
                            const Vector3f& q1,
                            const Vector3f& p2,
                            const Vector3f& q2) {
    const Vector3f d1 = q1 - p1;
    const Vector3f d2 = q2 - p2;
    const Vector3f r = p1 - p2;
    const float a = Vector3f::dot(d1, d1);
    const float e = Vector3f::dot(d2, d2);
    const float f = Vector3f::dot(d2, r);

    float s = 0.0f;
    float t = 0.0f;

    if (a <= Math::EPSILONF && e <= Math::EPSILONF) {
      return Vector3f::sqrDistance(p1, p2);
    }

    if (a <= Math::EPSILONF) {
      t = Math::clamp(f / e, 0.0f, 1.0f);
    }
    else {
      const float c = Vector3f::dot(d1, r);
      if (e <= Math::EPSILONF) {
        s = Math::clamp(-c / a, 0.0f, 1.0f);
      }
      else {
        const float b = Vector3f::dot(d1, d2);
        const float denom = a * e - b * b;
        if (denom > Math::EPSILONF) {
          s = Math::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
        }
        t = (b * s + f) / e;
        if (t < 0.0f) {
          t = 0.0f;
          s = Math::clamp(-c / a, 0.0f, 1.0f);
        }
        else if (t > 1.0f) {
          t = 1.0f;
          s = Math::clamp((b - c) / a, 0.0f, 1.0f);
        }
      }
    }

    const Vector3f c1 = p1 + d1 * s;
    const Vector3f c2 = p2 + d2 * t;
    return Vector3f::sqrDistance(c1, c2);
  }

  bool
  aabbIntersectsAabb(const AABB& a, const AABB& b) {
    return a.getMin().x <= b.getMax().x && a.getMax().x >= b.getMin().x &&
           a.getMin().y <= b.getMax().y && a.getMax().y >= b.getMin().y &&
           a.getMin().z <= b.getMax().z && a.getMax().z >= b.getMin().z;
  }

  bool
  aabbContainsPoint(const AABB& box, const Vector3f& p) {
    return p.x >= box.getMin().x && p.x <= box.getMax().x &&
           p.y >= box.getMin().y && p.y <= box.getMax().y &&
           p.z >= box.getMin().z && p.z <= box.getMax().z;
  }

  Vector3f
  closestPointAabb(const AABB& box, const Vector3f& p) {
    return Vector3f(
      Math::clamp(p.x, box.getMin().x, box.getMax().x),
      Math::clamp(p.y, box.getMin().y, box.getMax().y),
      Math::clamp(p.z, box.getMin().z, box.getMax().z));
  }

  bool
  aabbIntersectsSphere(const AABB& box, const Sphere& sphere) {
    const Vector3f c = closestPointAabb(box, sphere.getCenter());
    return Vector3f::sqrDistance(c, sphere.getCenter()) <= Math::sqr(sphere.getRadius());
  }

  bool
  aabbIntersectsRay(const AABB& box, const Ray& ray) {
    const Vector3f origin = ray.getOrigin();
    const Vector3f dir = ray.getDirection();

    float tmin = -std::numeric_limits<float>::infinity();
    float tmax = std::numeric_limits<float>::infinity();

    for (uint32 axis = 0; axis < 3; ++axis) {
      const float o = origin[axis];
      const float d = dir[axis];
      const float bmin = box.getMin()[axis];
      const float bmax = box.getMax()[axis];

      if (Math::abs(d) <= Math::EPSILONF) {
        if (o < bmin || o > bmax) {
          return false;
        }
        continue;
      }

      const float invD = 1.0f / d;
      float t1 = (bmin - o) * invD;
      float t2 = (bmax - o) * invD;
      if (t1 > t2) {
        const float tmp = t1;
        t1 = t2;
        t2 = tmp;
      }

      tmin = Math::max(tmin, t1);
      tmax = Math::min(tmax, t2);

      if (tmin > tmax) {
        return false;
      }
    }

    return tmax >= 0.0f;
  }

  bool
  aabbIntersectsSegment(const AABB& box, const Vector3f& p0, const Vector3f& p1) {
    const Vector3f dir = p1 - p0;
    float tmin = 0.0f;
    float tmax = 1.0f;

    for (uint32 axis = 0; axis < 3; ++axis) {
      const float o = p0[axis];
      const float d = dir[axis];
      const float bmin = box.getMin()[axis];
      const float bmax = box.getMax()[axis];

      if (Math::abs(d) <= Math::EPSILONF) {
        if (o < bmin || o > bmax) {
          return false;
        }
        continue;
      }

      const float invD = 1.0f / d;
      float t1 = (bmin - o) * invD;
      float t2 = (bmax - o) * invD;
      if (t1 > t2) {
        const float tmp = t1;
        t1 = t2;
        t2 = tmp;
      }

      tmin = Math::max(tmin, t1);
      tmax = Math::min(tmax, t2);
      if (tmin > tmax) {
        return false;
      }
    }

    return true;
  }

  bool
  rectIntersectsRect(const Rect& a, const Rect& b) {
    return a.getLeft() < b.getRight() &&
           a.getRight() > b.getLeft() &&
           a.getTop() < b.getBottom() &&
           a.getBottom() > b.getTop();
  }

  bool
  rectContainsPoint(const Rect& r, const Point& p) {
    if (Math::abs(p.z) > Math::SMALLNUMBER || p.x < 0.0f || p.y < 0.0f) {
      return false;
    }
    return r.contains(static_cast<uint32>(p.x), static_cast<uint32>(p.y));
  }

  bool
  toBoundingSphere(const Primitive& prim, Vector3f& center, float& radius) {
    const PRIMITIVE_TYPE::E type = prim.getType();

    if (type == +PRIMITIVE_TYPE::E::eAABB) {
      const AABB& aabb = static_cast<const AABB&>(prim);
      center = aabb.getCenter();
      radius = Vector3f::distance(center, aabb.getMax());
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eCAPSULE) {
      const Capsule& capsule = static_cast<const Capsule&>(prim);
      center = capsule.getCenter();
      radius = capsule.getSegmentLength() * 0.5f + capsule.getRadius();
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eCONE) {
      const Cone& cone = static_cast<const Cone&>(prim);
      center = Vector3f(0.0f, 0.0f, cone.getHeight() * 0.5f);
      radius = Math::sqrt(Math::sqr(cone.getRadius()) + Math::sqr(cone.getHeight() * 0.5f));
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eCYLINDER) {
      const Cylinder& cylinder = static_cast<const Cylinder&>(prim);
      center = cylinder.getCenter();
      radius = Math::sqrt(Math::sqr(cylinder.getRadius()) + Math::sqr(cylinder.getHalfHeight()));
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eFRUSTUM) {
      const Frustum& frustum = static_cast<const Frustum&>(prim);
      center = frustum.getCenter();
      const float halfDepth = frustum.getDepth() * 0.5f;
      radius = Math::sqrt(Math::sqr(frustum.getFarHalfWidth()) +
                          Math::sqr(frustum.getFarHalfHeight()) +
                          Math::sqr(halfDepth));
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eLINE) {
      const Line& line = static_cast<const Line&>(prim);
      center = line.getCenter();
      radius = line.getLength() * 0.5f;
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eOBB) {
      const OBB& obb = static_cast<const OBB&>(prim);
      center = obb.getCenter();
      radius = obb.getHalfExtents().magnitude();
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::ePOINT) {
      const Point& p = static_cast<const Point&>(prim);
      center = p.toVector3f();
      radius = 0.0f;
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eRECT) {
      const Rect& rect = static_cast<const Rect&>(prim);
      center = Vector3f(rect.getCenter().x, rect.getCenter().y, 0.0f);
      radius = Math::sqrt(Math::sqr(static_cast<float>(rect.m_width) * 0.5f) +
                          Math::sqr(static_cast<float>(rect.m_height) * 0.5f));
      return true;
    }
    if (type == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Sphere& sphere = static_cast<const Sphere&>(prim);
      center = sphere.getCenter();
      radius = sphere.getRadius();
      return true;
    }

    return false;
  }

  bool
  sphereSphere(const Vector3f& c0, const float r0, const Vector3f& c1, const float r1) {
    return Vector3f::sqrDistance(c0, c1) <= Math::sqr(r0 + r1);
  }


  bool
  intersects(const Primitive& primA, const Primitive& primB) {
    PRIMITIVE_TYPE::E typeA = primA.getType();
    PRIMITIVE_TYPE::E typeB = primB.getType();
    const Primitive* a = &primA;
    const Primitive* b = &primB;

    if (+typeB < +typeA) {
      const PRIMITIVE_TYPE::E tmpType = typeA;
      typeA = typeB;
      typeB = tmpType;

      const Primitive* tmp = a;
      a = b;
      b = tmp;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::eAABB) {
      return aabbIntersectsAabb(static_cast<const AABB&>(*a), static_cast<const AABB&>(*b));
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::eLINE) {
      const AABB& box = static_cast<const AABB&>(*a);
      const Line& line = static_cast<const Line&>(*b);
      return aabbIntersectsSegment(box, line.getPointA(), line.getPointB());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::eCAPSULE) {
      const AABB& box = static_cast<const AABB&>(*a);
      const Capsule& capsule = static_cast<const Capsule&>(*b);
      AABB expanded(box.getMin() - Vector3f::ONE * capsule.getRadius(),
                    box.getMax() + Vector3f::ONE * capsule.getRadius());
      return aabbIntersectsSegment(expanded, capsule.getPointA(), capsule.getPointB());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::ePLANE) {
      const AABB& box = static_cast<const AABB&>(*a);
      const Plane& plane = static_cast<const Plane&>(*b);
      const Vector3f n = plane.getUnitNormal();
      const Vector3f c = box.getCenter();
      const Vector3f e = box.getHalfExtents();
      const float r = e.x * Math::abs(n.x) + e.y * Math::abs(n.y) + e.z * Math::abs(n.z);
      const float s = Vector3f::dot(n, c - plane.getOrigin());
      return Math::abs(s) <= r;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::ePOINT) {
      return aabbContainsPoint(static_cast<const AABB&>(*a), static_cast<const Point&>(*b).toVector3f());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::eRAY) {
      return aabbIntersectsRay(static_cast<const AABB&>(*a), static_cast<const Ray&>(*b));
    }

    if (typeA == +PRIMITIVE_TYPE::E::eAABB && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      return aabbIntersectsSphere(static_cast<const AABB&>(*a), static_cast<const Sphere&>(*b));
    }

    if (typeA == +PRIMITIVE_TYPE::E::eCAPSULE && typeB == +PRIMITIVE_TYPE::E::eCAPSULE) {
      const Capsule& c0 = static_cast<const Capsule&>(*a);
      const Capsule& c1 = static_cast<const Capsule&>(*b);
      const float d2 = segmentSegmentSqrDistance(c0.getPointA(), c0.getPointB(), c1.getPointA(), c1.getPointB());
      return d2 <= Math::sqr(c0.getRadius() + c1.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eCAPSULE && typeB == +PRIMITIVE_TYPE::E::eLINE) {
      const Capsule& c = static_cast<const Capsule&>(*a);
      const Line& l = static_cast<const Line&>(*b);
      const float d2 = segmentSegmentSqrDistance(c.getPointA(), c.getPointB(), l.getPointA(), l.getPointB());
      return d2 <= Math::sqr(c.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eCAPSULE && typeB == +PRIMITIVE_TYPE::E::ePOINT) {
      const Capsule& capsule = static_cast<const Capsule&>(*a);
      const Point& point = static_cast<const Point&>(*b);
      return pointSegmentSqrDistance(point.toVector3f(), capsule.getPointA(), capsule.getPointB()) <=
             Math::sqr(capsule.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eCAPSULE && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Capsule& capsule = static_cast<const Capsule&>(*a);
      const Sphere& sphere = static_cast<const Sphere&>(*b);
      const float d2 = pointSegmentSqrDistance(sphere.getCenter(), capsule.getPointA(), capsule.getPointB());
      return d2 <= Math::sqr(capsule.getRadius() + sphere.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eLINE && typeB == +PRIMITIVE_TYPE::E::eLINE) {
      const Line& l0 = static_cast<const Line&>(*a);
      const Line& l1 = static_cast<const Line&>(*b);
      return segmentSegmentSqrDistance(l0.getPointA(), l0.getPointB(), l1.getPointA(), l1.getPointB()) <=
             Math::SMALLNUMBER;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eLINE && typeB == +PRIMITIVE_TYPE::E::ePOINT) {
      const Line& l = static_cast<const Line&>(*a);
      const Point& p = static_cast<const Point&>(*b);
      return pointSegmentSqrDistance(p.toVector3f(), l.getPointA(), l.getPointB()) <= Math::SMALLNUMBER;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eLINE && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Line& l = static_cast<const Line&>(*a);
      const Sphere& s = static_cast<const Sphere&>(*b);
      return pointSegmentSqrDistance(s.getCenter(), l.getPointA(), l.getPointB()) <= Math::sqr(s.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eLINE && typeB == +PRIMITIVE_TYPE::E::ePLANE) {
      const Line& line = static_cast<const Line&>(*a);
      const Plane& plane = static_cast<const Plane&>(*b);
      const float d0 = plane.signedDistanceTo(line.getPointA());
      const float d1 = plane.signedDistanceTo(line.getPointB());
      return Math::abs(d0) <= Math::SMALLNUMBER ||
             Math::abs(d1) <= Math::SMALLNUMBER ||
             (d0 * d1 < 0.0f);
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePLANE && typeB == +PRIMITIVE_TYPE::E::ePLANE) {
      const Plane& p0 = static_cast<const Plane&>(*a);
      const Plane& p1 = static_cast<const Plane&>(*b);
      const Vector3f n0 = p0.getUnitNormal();
      const Vector3f n1 = p1.getUnitNormal();
      const Vector3f cross = Vector3f::cross(n0, n1);
      if (cross.sqrMagnitude() > Math::SMALLNUMBER) {
        return true;
      }
      return Math::abs(p0.signedDistanceTo(p1.getOrigin())) <= Math::SMALLNUMBER;
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePLANE && typeB == +PRIMITIVE_TYPE::E::ePOINT) {
      const Plane& plane = static_cast<const Plane&>(*a);
      const Point& point = static_cast<const Point&>(*b);
      return plane.contains(point.toVector3f());
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePLANE && typeB == +PRIMITIVE_TYPE::E::eRAY) {
      const Plane& plane = static_cast<const Plane&>(*a);
      const Ray& ray = static_cast<const Ray&>(*b);
      const Vector3f n = plane.getUnitNormal();
      const float denom = Vector3f::dot(n, ray.getDirection());
      if (Math::abs(denom) <= Math::EPSILONF) {
        return plane.contains(ray.getOrigin());
      }
      const float t = Vector3f::dot(n, plane.getOrigin() - ray.getOrigin()) / denom;
      return t >= 0.0f;
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePLANE && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Plane& plane = static_cast<const Plane&>(*a);
      const Sphere& sphere = static_cast<const Sphere&>(*b);
      return plane.distanceTo(sphere.getCenter()) <= sphere.getRadius();
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePOINT && typeB == +PRIMITIVE_TYPE::E::ePOINT) {
      const Point& p0 = static_cast<const Point&>(*a);
      const Point& p1 = static_cast<const Point&>(*b);
      return Vector3f::sqrDistance(p0.toVector3f(), p1.toVector3f()) <= Math::SMALLNUMBER;
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePOINT && typeB == +PRIMITIVE_TYPE::E::eRAY) {
      const Point& point = static_cast<const Point&>(*a);
      const Ray& ray = static_cast<const Ray&>(*b);
      return pointRaySqrDistance(point.toVector3f(), ray) <= Math::SMALLNUMBER;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eRAY && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Ray& ray = static_cast<const Ray&>(*a);
      const Sphere& sphere = static_cast<const Sphere&>(*b);
      return pointRaySqrDistance(sphere.getCenter(), ray) <= Math::sqr(sphere.getRadius());
    }

    if (typeA == +PRIMITIVE_TYPE::E::eRECT && typeB == +PRIMITIVE_TYPE::E::eRECT) {
      return rectIntersectsRect(static_cast<const Rect&>(*a), static_cast<const Rect&>(*b));
    }

    if (typeA == +PRIMITIVE_TYPE::E::ePOINT && typeB == +PRIMITIVE_TYPE::E::eRECT) {
      return rectContainsPoint(static_cast<const Rect&>(*b), static_cast<const Point&>(*a));
    }

    if (typeA == +PRIMITIVE_TYPE::E::eSPHERE && typeB == +PRIMITIVE_TYPE::E::eSPHERE) {
      const Sphere& s0 = static_cast<const Sphere&>(*a);
      const Sphere& s1 = static_cast<const Sphere&>(*b);
      return sphereSphere(s0.getCenter(), s0.getRadius(), s1.getCenter(), s1.getRadius());
    }

    // Generic fallback for pairs not yet implemented explicitly: bounded-shape test.
    Vector3f c0 = Vector3f::ZERO;
    Vector3f c1 = Vector3f::ZERO;
    float r0 = 0.0f;
    float r1 = 0.0f;

    if (typeA == +PRIMITIVE_TYPE::E::ePLANE && toBoundingSphere(*b, c1, r1)) {
      const Plane& plane = static_cast<const Plane&>(*a);
      return plane.distanceTo(c1) <= r1;
    }

    if (typeB == +PRIMITIVE_TYPE::E::ePLANE && toBoundingSphere(*a, c0, r0)) {
      const Plane& plane = static_cast<const Plane&>(*b);
      return plane.distanceTo(c0) <= r0;
    }

    if (typeA == +PRIMITIVE_TYPE::E::eRAY && toBoundingSphere(*b, c1, r1)) {
      return pointRaySqrDistance(c1, static_cast<const Ray&>(*a)) <= Math::sqr(r1);
    }

    if (typeB == +PRIMITIVE_TYPE::E::eRAY && toBoundingSphere(*a, c0, r0)) {
      return pointRaySqrDistance(c0, static_cast<const Ray&>(*b)) <= Math::sqr(r0);
    }

    if (toBoundingSphere(*a, c0, r0) && toBoundingSphere(*b, c1, r1)) {
      return sphereSphere(c0, r0, c1, r1);
    }

    return false;
  }

  } // namespace COLLISIONS


} // namespace CYLLENE_SDK
