#include "cySphere.h"
#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

  String
  Sphere::toString() {
    return Utils::format("A: %s, B: %s, Height: %2.f");
  }

  bool
  Sphere::intersects(const Primitive& other) {
    const PRIMITIVE_TYPE::E type = other.getType();
    if (type == +PRIMITIVE_TYPE::E::SPHERE) {
      const Sphere& otherCapsule = static_cast<const Sphere&>(other);

    }

    return false;
  }



}