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
#include "cyMatrix3.h"
#include "cyMath.h"
#include "cyUtilities.h"
#include "cyMatrix4.h"

namespace CYLLENE_SDK {

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Euler ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Euler::Euler(const int32& norder)
: x(0),
  y(0),
  z(0),
  order(EulOrdXYZs) {}

Euler::Euler(const float& nx, const float& ny, const float& nz)
: x(nx),
  y(ny),
  z(nz),
  order(EulOrdXYZs) {}

Euler::Euler(const float& nx, const float& ny, const float& nz, const int32& norder)
: x(nx),
  y(ny),
  z(nz),
  order(norder) {}

Euler::Euler(const Vector3f& vector)
: x(vector.x),
  y(vector.y),
  z(vector.z),
  order(EulOrdXYZs) {}

Euler::Euler(const Vector4f& vector)
: x(vector.x),
  y(vector.y),
  z(vector.z),
  order(static_cast<int32>(vector.w)) {}

Euler::Euler(const Matrix3& rotationMatrix, const int32& norder) {
  Matrix4 mat4(rotationMatrix);
  mat4.m[3][3] = 1.0f;

  EulerAngles eulerAngles = Eul_FromHMatrix(mat4.m, norder);

  x = eulerAngles.x;
  y = eulerAngles.y;
  z = eulerAngles.z;
  order = static_cast<float>(norder);
}

Euler::Euler(const Matrix4& rotationMatrix, const int32& norder) {
  EulerAngles eulerAngles = Eul_FromHMatrix(rotationMatrix.m, norder);

  x = eulerAngles.x;
  y = eulerAngles.y;
  z = eulerAngles.z;
  order = static_cast<float>(norder);
}

Euler::Euler(const Quaternion& rotationQuat, const int32& norder) {
  Quat q;
  q.x = rotationQuat.x;
  q.y = rotationQuat.y;
  q.z = rotationQuat.z;
  q.w = rotationQuat.w;

  EulerAngles eulerAngles = Eul_FromQuat(q, norder);

  x = eulerAngles.x;
  y = eulerAngles.y;
  z = eulerAngles.z;
  order = static_cast<float>(norder);
}

Matrix4
Euler::getMatrix4Rotation() const {
  EulerAngles ea;
  ea.x = x;
  ea.y = y;
  ea.z = z;
  ea.w = static_cast<float>(order);

  HMatrix m;
  Eul_ToHMatrix(ea, m);

  Matrix4 mat;
  mat.m[0][0] = m[0][0];
  mat.m[0][1] = m[0][1];
  mat.m[0][2] = m[0][2];
  mat.m[0][3] = 0.0f;

  mat.m[1][0] = m[1][0];
  mat.m[1][1] = m[1][1];
  mat.m[1][2] = m[1][2];
  mat.m[1][3] = 0.0f;

  mat.m[2][0] = m[2][0];
  mat.m[2][1] = m[2][1];
  mat.m[2][2] = m[2][2];
  mat.m[2][3] = 0.0f;

  mat.m[3][0] = 0.0f;
  mat.m[3][1] = 0.0f;
  mat.m[3][2] = 0.0f;
  mat.m[3][3] = 1.0f;

  return mat;
}

Matrix3
Euler::getMatrix3Rotation() const {
  Matrix4 mat = getMatrix4Rotation();

  return Matrix3(mat.m[0][0], mat.m[0][1], mat.m[0][2],
                 mat.m[1][0], mat.m[1][1], mat.m[1][2],
                 mat.m[2][0], mat.m[2][1], mat.m[2][2]);
}

Quaternion
Euler::getQuatRotation() const {
  EulerAngles ea;
  ea.x = x;
  ea.y = y;
  ea.z = z;
  ea.w = static_cast<float>(order);

  Quat q = Eul_ToQuat(ea);

  Quaternion quat;
  quat.x = q.x;
  quat.y = q.y;
  quat.z = q.z;
  quat.w = q.w;

  return quat;
}

String
Euler::toString() {
  return Utils::format("( %2.2f, %2.2f, %2.2f, %2.2f)", x, y, z, order);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Quaternion //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Quaternion::Quaternion(const float& nx, const float& ny, const float& nz, const float& nw)
: x(nx),
  y(ny),
  z(nz),
  w(nw) {}

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

Quaternion::Quaternion(const Euler& euler) {
  *this = euler.getQuatRotation();
}

Quaternion::Quaternion(const Matrix3& matrix) {
  Matrix3 tmp = matrix.transposed();

  float sum = tmp._m.m00 + tmp._m.m11 + tmp._m.m22;

  if (sum > 0.0f) {
    w = sqrt(sum + 1.0f) * 0.5f;
    float f = 0.25f / w;
    x = (tmp.m[2][1] - tmp.m[1][2]) * f;
    y = (tmp.m[0][2] - tmp.m[2][0]) * f;
    z = (tmp.m[1][0] - tmp.m[0][1]) * f;
  }
  else if ((tmp._m.m00 > tmp._m.m11) && (tmp._m.m00 > tmp._m.m22)) {
    x = sqrt(tmp._m.m00 - tmp._m.m11 - tmp._m.m22 + 1.0f) * 0.5f;
    float f = 0.25f / x;
    y = (tmp.m[1][0] + tmp.m[0][1]) * f;
    z = (tmp.m[0][2] + tmp.m[2][0]) * f;
    w = (tmp.m[2][1] - tmp.m[1][2]) * f;
  }
  else if (tmp._m.m11 > tmp._m.m22) {
    y = sqrt(tmp._m.m11 - tmp._m.m00 - tmp._m.m22 + 1.0f) * 0.5f;
    float f = 0.25f / y;
    x = (tmp.m[1][0] + tmp.m[0][1]) * f;
    z = (tmp.m[2][1] + tmp.m[1][2]) * f;
    w = (tmp.m[0][2] - tmp.m[2][0]) * f;
  }
  else {
    z = sqrt(tmp._m.m22 - tmp._m.m00 - tmp._m.m11 + 1.0f) * 0.5f;
    float f = 0.25f / z;
    x = (tmp.m[0][2] + tmp.m[2][0]) * f;
    y = (tmp.m[2][1] + tmp.m[1][2]) * f;
    w = (tmp.m[1][0] - tmp.m[0][1]) * f;
  }
}

Quaternion::Quaternion(const Matrix4& matrix) {
  *this = Quaternion(Matrix3(matrix));
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
  // TODO: Check if this shit is correctly implemented

  // Quaternion result;
  // 
  // float dotproduct = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
  // float theta, st, sut, sout, coeff1, coeff2;
  // 
  // // algorithm adapted from Shoemake's paper
  // t = t / 2.0;
  // 
  // theta = (float)acos(dotproduct);
  // if (theta < 0.0) theta = -theta;
  // 
  // st = (float)sin(theta);
  // sut = (float)sin(t * theta);
  // sout = (float)sin((1 - t) * theta);
  // coeff1 = sout / st;
  // coeff2 = sut / st;
  // 
  // result.x = coeff1 * q1.x + coeff2 * q2.x;
  // result.y = coeff1 * q1.y + coeff2 * q2.y;
  // result.z = coeff1 * q1.z + coeff2 * q2.z;
  // result.w = coeff1 * q1.w + coeff2 * q2.w;
  // 
  // result.norm();


  float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
  // Quaternion q2New;
  float theta, mult1, mult2;

  w1 = q1.w; x1 = q1.x; y1 = q1.y; z1 = q1.z;
  w2 = q2.w; x2 = q2.x; y2 = q2.y; z2 = q2.z;

  // Reverse the sign of q2 if q1.q2 < 0.
  if (w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2 < 0)
  {
    w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
  }

  theta = acos(w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2);

  if (theta > 0.000001)
  {
    mult1 = sin((1 - t) * theta) / sin(theta);
    mult2 = sin(t * theta) / sin(theta);
  }

  // To avoid division by 0 and by very small numbers the approximation of sin(angle)
  // by angle is used when theta is small (0.000001 is chosen arbitrarily).
  else
  {
    mult1 = 1 - t;
    mult2 = t;
  }

  w3 = mult1 * w1 + mult2 * w2;
  x3 = mult1 * x1 + mult2 * x2;
  y3 = mult1 * y1 + mult2 * y2;
  z3 = mult1 * z1 + mult2 * z2;

  // TODO: WTF I DONT LIKE THIS
  return *new Quaternion(w3, x3, y3, z3);


  // return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

void
Quaternion::setRotation(const Euler& euler) {
  *this = Quaternion(euler);
}

void
Quaternion::setRotation(const Matrix3& m) {
  *this = Quaternion(m);
}

void
Quaternion::setRotation(const Matrix4& m) {
  *this = Quaternion(m);
}

void
Quaternion::setValues(const float& nx, const float& ny, const float& nz, const float& nw) {
  x = nx;
  y = ny;
  z = nz;
  w = nw;
}

void
Quaternion::setValues(const Vector3f& vector, const float& scalar) {
  x = vector.x;
  y = vector.y;
  z = vector.z;
  w = scalar;
}

const Vector3f
Quaternion::getVectorPart() const {
  return Vector3f(x, y, z);
}

const float&
Quaternion::getScalarPart() const {
  return w;
}

Euler
Quaternion::getEulerRotation(const int32& order) const {
  return Euler(*this, order);
}

const Matrix3
Quaternion::getMatrix3Rotation() const {
  Quaternion tmp = this->normalized();

  float x2 = tmp.x * tmp.x;
  float y2 = tmp.y * tmp.y;
  float z2 = tmp.z * tmp.z;
  float xy = tmp.x * tmp.y;
  float xz = tmp.x * tmp.z;
  float yz = tmp.y * tmp.z;
  float wx = tmp.w * tmp.x;
  float wy = tmp.w * tmp.y;
  float wz = tmp.w * tmp.z;

  return Matrix3(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz),         2.0f * (xz + wy),
                 2.0f * (xy + wz),        1.0f - 2.0f * (x2 + z2),  2.0f * (yz - wx),
                 2.0f * (xz - wy),        2.0f * (yz + wx),         1.0f - 2.0f * (x2 + y2));
}

const Matrix4
Quaternion::getMatrix4Rotation() const {
  Matrix3 tmp = this->getMatrix3Rotation();
  
  Matrix4 mat(tmp);
  mat.m[3][3] = 1.0f;

  return mat;
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
Quaternion::normalized() const {
  return Quaternion(x / norm(), y / norm(), z / norm(), w / norm());
}

void
Quaternion::normalize() {
  *this = normalized();
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

Vector4f 
Quaternion::toVector4() const {
  return Vector4f(x, y, z, w);
}

String
Quaternion::toString() const {
  return Utils::format("(%2.2f, %2.2f, %2.2f, %2.2f)", x, y, z, w);
}

const Quaternion
Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

}
