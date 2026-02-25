#include "cyIntersections.h"


#include "cyAABB.h"
#include "cyCapsule.h"
#include "cyCone.h"
#include "cyLine.h"
#include "cyOBB.h"
#include "cyPlane.h"
#include "cySphere.h"
#include "cyVector4f.h"


namespace CYLLENE_SDK {
    
bool
intersects(const Primitive& prim1, const Primitive& prim2) {
  const PRIMITIVE_TYPE::E typeA = prim1.getType();
  const PRIMITIVE_TYPE::E typeB = prim2.getType();
    
  SPtr<Primitive> primA = nullptr;
  SPtr<Primitive> primB = nullptr;

  if (typeA == +PRIMITIVE_TYPE::E::AABB) {
    primA = std::make_shared<AABB>(static_cast<const AABB&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::CAPSULE) {
    primA = std::make_shared<Capsule>(static_cast<const Capsule&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::CONE) {
    primA = std::make_shared<Cone>(static_cast<const Cone&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::PLANE) {
    primA = std::make_shared<Plane>(static_cast<const Plane&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::SPHERE) {
    primA = std::make_shared<Sphere>(static_cast<const Sphere&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::LINE) {
    primA = std::make_shared<Line>(static_cast<const Line&>(prim1));
  }
  else if (typeA == +PRIMITIVE_TYPE::E::OBB) {
    primA = std::make_shared<OBB>(static_cast<const OBB&>(prim1));
  }



  return false;
}
  

}