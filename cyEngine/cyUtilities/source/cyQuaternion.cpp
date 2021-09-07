/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyQuaternion.cpp
 * @author Marco "Swampy" Millan
 * @date 9/6/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#include "cyQuaternion.h"

#include "cyVector3f.h"
#include "cyVector4f.h"
#include "cyMatrix3x3.h"

namespace CYLLENE_SDK {
  Quaternion::Quaternion(const Vector3f& other) 
  : x(other.x),
    y(other.y),
    z(other.z),
    w(0.0f) {}

  Quaternion::Quaternion(const Vector3f& other, const float& nw)
    : x(other.x),
      y(other.y),
      z(other.z),
      w(nw) {}

  Quaternion::Quaternion(const Vector4f other)
    : x(other.x),
      y(other.y),
      z(other.z),
      w(other.w) {}

  Quaternion
  Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(w* other.x + x * other.w + y * other.z - z * other.y,
                      w* other.y - x * other.z + y * other.w + z * other.x,
                      w* other.z + x * other.y - y * other.x + z * other.w,
                      w* other.w - x * other.x - y * other.y - z * other.z);
  }

  const Vector3f
  Quaternion::transform(const Vector3f& vector, const Quaternion& quaternion) {
    auto tmp = quaternion.getVectorPart();
    float b2 = tmp.sqrMagnitude();
    return (vector * (quaternion.w * quaternion.w - b2) + tmp * (Vector3f::dot(vector, tmp) * 2.0f)
      + Vector3f::cross(tmp, vector) * (quaternion.w * 2.0f));
  }

  Vector4f 
  Quaternion::toVector4() {
  
  }

  const Vector3f&
  Quaternion::getVectorPart() const {
    return Vector3f(x, y, z);
  }

  const float&
  Quaternion::getScalarPart() const {
    return w;
  }

  const Matrix3x3&
  Quaternion::getRotationMatrix() const {
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // Don't know if this is row major or column major
    return Matrix3x3(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy),
                     2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx),
                     2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2));
    
  }

  void
  Quaternion::setRotationMatrix(const Matrix3x3& m) {

  }

}
