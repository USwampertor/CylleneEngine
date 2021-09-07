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

  class Vector4f;
  class Vector3f;
  class Matrix3x3;
  class Matrix4x4;

  class CY_UTILITY_EXPORT Quaternion {
    
  public:
    
    Quaternion() = default;

    Quaternion(const float& nx, const float& ny, const float& nz, const float& nw)
      : x(nx),
        y(ny),
        z(nz),
        w(nw) {}

    Quaternion(const Vector3f& other);

    Quaternion(const Vector3f& other, const float& nw);

    Quaternion(const Vector4f other);

    Quaternion
    operator*(const Quaternion& other) const;

    Vector4f 
    toVector4();

    const Vector3f&
    getVectorPart() const;

    const float&
    getScalarPart() const;

    const Matrix3x3&
    getRotationMatrix() const;

    static const Vector3f
    transform(const Vector3f& vector, const Quaternion& quaternion);

    void
    setRotationMatrix(const Matrix3x3& m);

  public:

    float x;
    float y;
    float z;
    float w;

  };

}
