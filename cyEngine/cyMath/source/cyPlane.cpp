#include "cyPlane.h"
#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

  String
  Plane::toString() {
    return Utils::format("A: %s, B: %s, Height: %2.f");
  }

  bool
  Plane::intersects(const Primitive& other) {
    const PRIMITIVE_TYPE::E type = other.getType();
    if (type == +PRIMITIVE_TYPE::E::PLANE) {
      const Plane& otherPlane = static_cast<const Plane&>(other);
      Vector3f d = m_normal ^ otherPlane.m_normal;
      if ((d | d) < Math::EPSILONF) {
        return false;
      }
    
      float det = m_normal.x * otherPlane.m_normal.y - m_normal.y * otherPlane.m_normal.x;
      
    
      if (Math::abs(det) < Math::EPSILONF) {
        return false;
      }
    }


    return false;
  }



}