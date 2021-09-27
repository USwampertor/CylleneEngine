
#include "cyUtilities.h"

#include "cyVector2f.h"
#include "cyVector2i.h"

#include "cyVector3f.h"

#include "cyVector4f.h"

#include "cyMath.h"

namespace CYLLENE_SDK {
  
  
  Vector3f::Vector3f(const Vector3f& other)
    : x(other.x),
      y(other.y),
      z(other.z) {}

  Vector3f::Vector3f(const Vector2f& other)
    : x(other[0]),
      y(other[1]),
      z(0.0f) {}

  Vector3f::Vector3f(const Vector2i& other)
    : x(static_cast<float>(other[0])),
      y(static_cast<float>(other[1])),
      z(0.0f) {}

  Vector3f::Vector3f(const Vector4f& other)
    : x(other[0]),
      y(other[1]),
      z(other[2]) {}


  Vector3f::Vector3f(const float& nx, const float& ny, const float& nz) 
    : x(nx),
      y(ny),
      z(nz) {}

  float&
  Vector3f::operator()(const uint32& index) {
    return (&x)[index];
  }

  const float&
  Vector3f::operator()(const uint32& index) const {
    return (&x)[index];
  }

  float&
  Vector3f::operator[](const uint32& index) {
    return (&x)[index];
  }

  const float
  Vector3f::operator[](const uint32& index) const {
    return (&x)[index];
  }


  Vector3f
  Vector3f::operator+(const Vector3f& v) const {
    return Vector3f(x + v.x, y + v.y, z + v.z);
  }

  Vector3f
  Vector3f::operator-(const Vector3f& v) const {
    return Vector3f(x - v.x, y - v.y, z - v.z);
  }

  Vector3f
  Vector3f::operator*(const Vector3f& v) const {
    return Vector3f(x * v.x, y * v.y, z * v.z);
  }

  Vector3f
  Vector3f::operator/(const Vector3f& v) const {
    return Vector3f(x / v.x, y / v.y, z / v.z);
  }

  Vector3f
  Vector3f::operator+(const float& plus) const {
    return Vector3f(x + plus, y + plus, z + plus);
  }

  Vector3f
  Vector3f::operator-(const float& minus) const {
    return Vector3f(x - minus, y - minus, z - minus);
  }

  Vector3f
  Vector3f::operator*(const float& times) const {
    return Vector3f(x * times, y * times, z * times);
  }

  Vector3f
  Vector3f::operator/(const float& under) const {
    return Vector3f(x / under, y / under, z / under);
  }

  float
  Vector3f::operator|(const Vector3f& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  Vector3f
  Vector3f::operator^(const Vector3f& v) const {
    return Vector3f((y * v.z) - (z * v.y),
                    (z * v.x) - (x * v.z),
                    (x * v.y) - (y * v.x));
  }

  bool
  Vector3f::operator==(const Vector3f& v) const {
    return x == v.x && y == v.y && z == v.z;
  }

  bool
  Vector3f::operator!=(const Vector3f& v) const {
    return x != v.x || y != v.y || z != v.z;
  }

  bool
  Vector3f::operator<(const Vector3f& v) const {
    return x < v.x && y < v.y && z < v.z;
  }

  bool
  Vector3f::operator>(const Vector3f& v) const {
    return x > v.x && y > v.y && z > v.z;
  }

  bool
  Vector3f::operator<=(const Vector3f& v) const {
    return x <= v.x && y <= v.y && z <= v.z;
  }

  bool
  Vector3f::operator>=(const Vector3f& v) const {
    return x >= v.x && y >= v.y && z >= v.z;
  }

  Vector3f
  Vector3f::operator-() const {
    return Vector3f(-x, -y, -z);
  }

  Vector3f&
  Vector3f::operator+=(const Vector3f& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector3f&
  Vector3f::operator-=(const Vector3f& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  Vector3f&
  Vector3f::operator*=(const Vector3f& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  Vector3f&
  Vector3f::operator/=(const Vector3f& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  Vector3f&
  Vector3f::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }

  Vector3f&
  Vector3f::operator/=(const float& scale) {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
  }

  float
  Vector3f::dot(const Vector3f& a, const Vector3f& b) {
    return a | b;
  }

  Vector3f
  Vector3f::cross(const Vector3f& a, const Vector3f& b) {
    return a ^ b;
  }

  float
  Vector3f::projection(const Vector3f& a, const Vector3f& b) {
    return (a | b) / Math::sqr(a.magnitude());
  }

  float
  Vector3f::sqrDistance(const Vector3f& a, const Vector3f& b) {
    return Math::sqr(a.x - b.x) +
           Math::sqr(a.y - b.y) +
           Math::sqr(a.z - b.z);
  }

  float
  Vector3f::distance(const Vector3f& a, const Vector3f& b) {
    return 
      Math::sqrt(Math::sqr(a.x - b.x) +
                 Math::sqr(a.y - b.y) +
                 Math::sqr(a.z - b.z));
  }

  void
  Vector3f::setValues(const float& newX, const float& newY, const float& newZ) {
    x = newX;
    y = newY;
    z = newZ;
  }

  void
  Vector3f::min(const Vector3f& v) {
    if (v.x < x) x = v.x;
    if (v.y < y) y = v.y;
    if (v.z < z) z = v.z;
  }

  void
  Vector3f::max(const Vector3f& v) {
    if (v.x > x) x = v.x;
    if (v.y > y) y = v.y;
    if (v.z > z) z = v.z;
  }

  void
  Vector3f::floor() {
    x = Math::floor(x);
    y = Math::floor(y);
    z = Math::floor(z);
  }

  void
  Vector3f::ceiling() {
    x = Math::ceil(x);
    y = Math::ceil(y);
    z = Math::ceil(z);
  }

  void
  Vector3f::round() {
    x = Math::round(x);
    y = Math::round(y);
    z = Math::round(z);
  }

  void
  Vector3f::roundHalf() {
    x = Math::roundHalf(x);
    y = Math::roundHalf(y);
    z = Math::roundHalf(z);
  }

  float
  Vector3f::getHighest() const {
    return Math::max3(x, y, z);
  }

  float
  Vector3f::getLowest() const {
    return Math::min3(x, y, z);
  }

  float
  Vector3f::magnitude() const {
    return Math::sqrt(x * x + y * y + z * z);
  }

  float
  Vector3f::sqrMagnitude() const {
    return (x * x + y * y + z * z);
  }

  Vector3f
  Vector3f::normalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f);

    CY_ASSERT(sqr > Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    return Vector3f((x * unit), (y * unit), (z * unit));
  }

  Vector3f
  Vector3f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    return Vector3f((x * unit), (y * unit), (z * unit));
  }

  void
  Vector3f::normalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    x *= unit;
    y *= unit; 
    z *= unit;
  }

  void
  Vector3f::qNormalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    x *= unit;
    y *= unit; 
    z *= unit;

  }

  bool
  Vector3f::isZero() const {
    return 0.0f == x && 0.0f == y && 0.0f == z;
  }

  bool
  Vector3f::isNearlySame(const Vector3f& a, const Vector3f& b, const float& error = Math::SMALLNUMBER) {
    return Math::abs(a.x - b.x) <= error &&
           Math::abs(a.y - b.y) <= error &&
           Math::abs(a.z - b.z) <= error;
  }

  String
  Vector3f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", x);
    output += ", ";
    output += Utils::format("%2.2f", y);
    output += ", ";
    output += Utils::format("%2.2f", z);
    output += " )";

    return output;
  }
  
  const Vector3f Vector3f::ZERO   = Vector3f(0.0f, 0.0f, 0.0f);

  const Vector3f Vector3f::ONE    = Vector3f(1.0f, 1.0f, 1.0f);

  const Vector3f Vector3f::ONEX   = Vector3f(1.0f, 0.0f, 0.0f);

  const Vector3f Vector3f::ONEY   = Vector3f(0.0f, 1.0f, 0.0f);

  const Vector3f Vector3f::ONEZ   = Vector3f(0.0f, 0.0f, 1.0f);

  const Vector3f Vector3f::RIGHT  = Vector3f(1.0f, 0.0f, 0.0f);

  const Vector3f Vector3f::UP     = Vector3f(0.0f, 1.0f, 0.0f);

  const Vector3f Vector3f::FRONT  = Vector3f(0.0f, 0.0f, 1.0f);

}