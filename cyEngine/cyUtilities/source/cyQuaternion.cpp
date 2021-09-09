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

#include "cyEulerHelpers.h"
#include "cyVector3f.h"
#include "cyVector4f.h"
#include "cyMatrix3x3.h"
#include "cyMath.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {

  Euler::Euler(const float& nx, const float& ny, const float& nz)
    : x(nx),
      y(ny),
      z(nz),
      order(0) {}

  Euler::Euler(const float& nx, const float& ny, const float& nz, const int32& norder)
    : x(nx),
      y(ny),
      z(nz),
      order(norder) {}

  Euler::Euler(const Vector3f& vector)
    : x(vector.x),
      y(vector.y),
      z(vector.z),
      order(0) {}

  Euler::Euler(const Vector4f& vector)
    : x(vector.x),
      y(vector.y),
      z(vector.z),
      order(static_cast<int32>(vector.w)) {}

  Quaternion::Quaternion(const Vector3f& other) 
  : x(other.x),
    y(other.y),
    z(other.z),
    w(0.0f) {}

  Quaternion::Quaternion(const Vector4f other)
    : x(other.x),
      y(other.y),
      z(other.z),
      w(other.w) {}

  Quaternion::Quaternion(const Euler& euler, const int32& order) {

    EulerAngles ea;
    
    ea.x = euler.x;
    ea.y = euler.y;
    ea.z = euler.z;
    ea.w = order;

    Quat q = Eul_ToQuat(ea);

    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
  }

  float&
  Quaternion::operator[](uint32 index) {
    return (&x)[index];
  }

  float
  Quaternion::operator[](uint32 index) const {
    return (&x)[index];
  }

  Quaternion
  Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
  }
  
  Quaternion
  Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
  }
  

  Quaternion
  Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(w* other.x + x * other.w + y * other.z - z * other.y,
                      w* other.y - x * other.z + y * other.w + z * other.x,
                      w* other.z + x * other.y - y * other.x + z * other.w,
                      w* other.w - x * other.x - y * other.y - z * other.z);
  }

  Quaternion
  Quaternion::operator/(const Quaternion& other) const {
    
    return *this * other.inversed();
    // float divider = Math::sqr(other.x) + 
    //                 Math::sqr(other.y) + 
    //                 Math::sqr(other.z) +
    //                 Math::sqr(other.w);
    // 
    // return Quaternion((other.w * x - other.x * w - other.y * z + other.z * y) / divider,
    //                   (other.w * y + other.x * z - other.y * w - other.z * x) / divider,
    //                   (other.w * z - other.x * y + other.y * x - other.z * w) / divider,
    //                   (other.w * w + other.x * x + other.y * y + other.z * z) / divider);
  }

  Quaternion
  Quaternion::operator/(const float& scale) const {
    return Quaternion(x / scale, y / scale, z / scale, w / scale);
  }

  Quaternion
  Quaternion::operator*(const float& scale) const {
    return Quaternion(x * scale, y * scale, z * scale, w * scale);
  }

  Quaternion&
  Quaternion::operator=(const Quaternion& other) {
    x = other.x;
    y = other.y;
    z = other.z;

    w = other.w;

    return *this;
  }

  bool
  Quaternion::operator==(const Quaternion& other) {
    return x == other.x && y == other.y && z == other.z && w == other.w;
  }

  bool
  Quaternion::operator!=(const Quaternion& other) {
    return x != other.x && y != other.y && z != other.z && w != other.w;
  }

  bool
  Quaternion::operator<(const Quaternion& other) {
    return x < other.x && y < other.y && z < other.z && w < other.w;
  }

  bool
  Quaternion::operator>(const Quaternion& other) {
    return x > other.x && y > other.y && z > other.z && w > other.w;
  }

  Quaternion
  Quaternion::operator-() const {
    return Quaternion(-x, -y, -z, -w);
  }

  bool
  Quaternion::operator<=(const Quaternion& other) {
    return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
  }

  bool
  Quaternion::operator>=(const Quaternion& other) {
    return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
  }

  Quaternion&
  Quaternion::operator+=(const Quaternion& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }
  
  Quaternion&
  Quaternion::operator-=(const Quaternion& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }
  
  Quaternion&
  Quaternion::operator*=(const Quaternion& other) {
    
    Quaternion newQuat;
    newQuat.w = w * other.w - x * other.x - y * other.y - z * other.z;
    newQuat.x = w * other.x + x * other.w + y * other.z - z * other.y;
    newQuat.y = w * other.y - x * other.z + y * other.w + z * other.x;
    newQuat.z = w * other.z + x * other.y - y * other.x + z * other.w;

    *this = newQuat;

    return *this;
  }
  
  Quaternion&
  Quaternion::operator/=(const Quaternion& other) {


    *this = *this * other.inversed();
    // float divider = Math::sqr(other.x) +
    //                 Math::sqr(other.y) +
    //                 Math::sqr(other.z) +
    //                 Math::sqr(other.w);
    // 
    // *this = Quaternion((other.w * x - other.x * w - other.y * z + other.z * y) / divider,
    //                    (other.w * y + other.x * z - other.y * w - other.z * x) / divider,
    //                    (other.w * z - other.x * y + other.y * x - other.z * w) / divider,
    //                    (other.w * w + other.x * x + other.y * y + other.z * z) / divider);
    return *this;
  }

  Quaternion&
  Quaternion::operator*=(float scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
  }

  Quaternion&
  Quaternion::operator/=(float scale) {
    x /= scale;
    y /= scale;
    z /= scale;
    w /= scale;
    return *this;
  }

  float
  Quaternion::operator|(const Quaternion& v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  const Vector3f
  Quaternion::transform(const Vector3f& vector, const Quaternion& quaternion) {
    auto tmp = quaternion.getVectorPart();
    float b2 = tmp.sqrMagnitude();
    return (vector * (quaternion.w * quaternion.w - b2) + 
            tmp * (Vector3f::dot(vector, tmp) * 2.0f) + 
            Vector3f::cross(tmp, vector) * (quaternion.w * 2.0f));
  }

  const Quaternion
  Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t) {
  
  }

  void
  Quaternion::fromEuler(const Euler& euler, int32 order) {

  }

  void 
  Quaternion::fromEuler(const Vector3f& vector, int32 order) {

  }

  void
  Quaternion::setValues(const float& nx, const float& ny, const float& nz, const float& nw) {

  }

  void
  Quaternion::setValues(const Vector3f& vector, const float& scalar) {

  }

  void
  Quaternion::setRotationMatrix(const Matrix3x3& m) {
    float sum = m._m.m00 + m._m.m11 + m._m.m22;

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

  float
  Quaternion::norm() const {
    return Math::sqr(x) + Math::sqr(y) + Math::sqr(z) + Math::sqr(w);
  }

  float
  Quaternion::magnitude() const {
    return Math::sqrt(this->norm());
  }

  Quaternion
  Quaternion::scaled(const float& s) const {
    return Quaternion(x * s, y * s, z * s, w * s);
  }

  void
  Quaternion::scale(const float& s) {
    *this = scaled(s);
  }

  Quaternion
  Quaternion::conjugated() const {
    return Quaternion(-x, -y, -z, w);
  }

  void
  Quaternion::conjugate() {
    *this = conjugated();
  }

  Quaternion
  Quaternion::inversed() const {
    return conjugated().scaled(1/norm());
  }

  void
  Quaternion::inverse() {
    *this = inversed();
  }
  Quaternion
  Quaternion::unitQuaternion() const {
    return scaled(Math::invSqrt(norm()));
  }

  void
  Quaternion::unit() {
    *this = unitQuaternion();
  }

  Vector3f 
  Quaternion::rotate(const Vector3f& v) const {
    Quaternion q(v);
    Quaternion r = *this * q * this->inversed();
    return Vector3f(r.x, r.y, r.z);
  }

  bool
  Quaternion::isPure() const {
    return w == 0;
  }

  bool
  Quaternion::isReal() const {
    return x == 0 && y == 0 && z == 0;
  }

  Euler
  Quaternion::toEuler(const int32& order) const {
    Quat q;

    q.x = x;
    q.y = y;
    q.z = z;
    q.w = w;

    EulerAngles ea = Eul_FromQuat(q, order);

    return Euler(ea.x, ea.y, ea.z, ea.w);
  }

  Vector4f 
  Quaternion::toVector4() const {
    return Vector4f(x, y, z, w);
  }

  String
  Quaternion::toString() {
    String toReturn = Utils::format("(%2.2f, %2.2f, %2.2f, %2.2f)", x, y, z, w);
  }


}
