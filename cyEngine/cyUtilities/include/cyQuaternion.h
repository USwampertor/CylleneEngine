/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyQuaternion.h
 * @author Marco "Swampy" Millan
 * @date 9/1/2021
 * @brief Ah yes, quaternions my old nemesis, now I can call you
 *        my old friend. Quaternions are special vectors used for
 *        rotation regardless of orientation
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector4;
  class Vector3;

  class CY_UTILITY_EXPORT Quaternion {
    
  public:
    
    Quaternion() = default;

    Quaternion(const float& nx, const float& ny, const float& nz, const float& nw)
      : x(nx),
        y(ny),
        z(nz),
        w(nw) {}

  public:

    float x;
    float y;
    float z;
    float w;

  };

}
