#include "cySphere.h"
#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

  String
  Sphere::toString() {
    return Utils::format("Center: %s, radius: %2.f", m_center.toString().c_str(), m_radius);
  }

  bool
  Sphere::intersects(const Primitive& other) {
    const PRIMITIVE_TYPE::E type = other.getType();
    if (type == +PRIMITIVE_TYPE::E::SPHERE) {
      const Sphere& otherSphere = static_cast<const Sphere&>(other);
      
    }

    return false;
  }



}