#include "cyPoint.h"

#include "cyMath.h"

#include "cyLine.h"
#include "cyPlane.h"
#include "cySphere.h"
#include "cyCapsule.h"
#include "cyAABB.h"
#include "cyOBB.h"

namespace CYLLENE_SDK {

bool
Point::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();

  if (type == +PRIMITIVE_TYPE::E::POINT) {
    const Point& otherPoint = static_cast<const Point&>(other);
    return (Math::isNearSame(this->x, otherPoint.x) && 
            Math::isNearSame(this->y, otherPoint.y) && 
            Math::isNearSame(this->z, otherPoint.z));
  }
  else if (type == +PRIMITIVE_TYPE::E::LINE) {
    const Line& otherLine = static_cast<const Line&>(other);
  }
  else if (type == +PRIMITIVE_TYPE::E::PLANE) {
    const Plane& otherPlane = static_cast<const Plane&>(other);
  }
  else if (type == +PRIMITIVE_TYPE::E::SPHERE) {
    const Sphere& otherSphere = static_cast<const Sphere&>(other);
  }
  else if (type == +PRIMITIVE_TYPE::E::CAPSULE) {
    const Capsule& otherCapsule = static_cast<const Capsule&>(other);
  }
  else if (type == +PRIMITIVE_TYPE::E::AABB) {
    const AABB& otherAABB = static_cast<const AABB&>(other);
  }
  else if (type == +PRIMITIVE_TYPE::E::OBB) {
    const OBB& otherOBB = static_cast<const OBB&>(other);
  }


  return false;
}

}
