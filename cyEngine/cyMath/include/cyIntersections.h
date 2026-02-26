#pragma once
#include "cyMathPrerequisites.h"

#include "cyPrimitive.h"


namespace CYLLENE_SDK {

  namespace COLLISIONS {

  /**
  * @brief Check if an Primitive intersects with another Primitive. Internally it will check the type
  * of the Primitive and call the appropriate intersection function.
  * @param aabb The first Primitive to check
  * @param plane The second Primitive to check against
  * @return True if the Primitive intersects with the other Primitive, false otherwise
  */
  CY_MATH_EXPORT bool
  intersects(const Primitive& aabb, const Primitive& plane);

  }
}


