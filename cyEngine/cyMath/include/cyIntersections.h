/**
 * @file cyIntersections.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Intersections.
 */
#pragma once
#include "cyMathPrerequisites.h"

#include "cyPrimitive.h"


namespace CYLLENE_SDK {

  namespace COLLISIONS {

  /**
   * @brief Checks whether one primitive intersects another primitive.
   * @param first First primitive to test.
   * @param second Second primitive to test.
   * @return True if the primitives intersect, false otherwise.
   */
  CY_MATH_EXPORT bool
  intersects(const Primitive& first, const Primitive& second);

  }
}



