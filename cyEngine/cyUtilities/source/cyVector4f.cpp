
#include "cyVector4f.h"
#include "cyUtilities.h"

#include "cyVector3f.h"

#include "cyMath.h"

namespace CYLLENE_SDK {
  
  
  Vector4f::Vector4f(const Vector4f& other)
    : x(other.x),
      y(other.y),
      z(other.z),
      w(other.w) {}

  Vector4f::Vector4f(const Vector3f& other)
    : x(other[0]),
      y(other[1]),
      z(other[2]),
      w(0) {}


  Vector4f::Vector4f(const float& nx, const float& ny, const float& nz, const float& nw)
    : x(nx),
      y(ny),
      z(nz),
      w(nw) {}

  float&
  Vector4f::operator()(const uint32& index) {
    return (&x)[index];
  }

  const float&
  Vector4f::operator()(const uint32& index) const {
    return (&x)[index];
  }

  float&
  Vector4f::operator[](const uint32& index) {
    return (&x)[index];
  }

  const float
  Vector4f::operator[](const uint32& index) const {
    return (&x)[index];
  }


  Vector4f
  Vector4f::operator+(const Vector4f& v) const {
    return Vector4f(x + v.x, y + v.y, z + v.z, w + v.w);
  }

  Vector4f
  Vector4f::operator-(const Vector4f& v) const {
    return Vector4f(x - v.x, y - v.y, z - v.z, w - v.w);
  }

  Vector4f
  Vector4f::operator*(const Vector4f& v) const {
    return Vector4f(x * v.x, y * v.y, z * v.z, w * v.w);
  }

  Vector4f
  Vector4f::operator/(const Vector4f& v) const {
    return Vector4f(x / v.x, y / v.y, z / v.z, w / v.w);
  }

  Vector4f
  Vector4f::operator+(const float& plus) const {
    return Vector4f(x + plus, y + plus, z + plus, w + plus);
  }

  Vector4f
  Vector4f::operator-(const float& minus) const {
    return Vector4f(x - minus, y - minus, z - minus, w - minus);
  }

  Vector4f
  Vector4f::operator*(const float& times) const {
    return Vector4f(x * times, y * times, z * times, w * times);
  }

  Vector4f
  Vector4f::operator/(const float& under) const {
    return Vector4f(x / under, y / under, z / under, w / under);
  }

  float
  Vector4f::operator|(const Vector4f& v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }

  Vector4f
  Vector4f::operator^(const Vector4f& v) const {
    return Vector4f((y * v.z) - (z * v.y),
                    (z * v.x) - (x * v.z),
                    (x * v.y) - (y * v.x),
                    0);
  }

  bool
  Vector4f::operator==(const Vector4f& v) const {
    return x == v.x && y == v.y && z == v.z && w == v.w;
  }

  bool
  Vector4f::operator!=(const Vector4f& v) const {
    return x != v.x || y != v.y || z != v.z || w != v.w;
  }

  bool
  Vector4f::operator<(const Vector4f& v) const {
    return x < v.x && y < v.y && z < v.z && w < v.w;
  }

  bool
  Vector4f::operator>(const Vector4f& v) const {
    return x > v.x && y > v.y && z > v.z && w > v.w;
  }

  bool
  Vector4f::operator<=(const Vector4f& v) const {
    return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
  }

  bool
  Vector4f::operator>=(const Vector4f& v) const {
    return x >= v.x && y >= v.y && z >= v.z && w >= v.w;
  }

  Vector4f
  Vector4f::operator-() const {
    return Vector4f(-x, -y, -z, -w);
  }

  Vector4f&
  Vector4f::operator+=(const Vector4f& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  Vector4f&
  Vector4f::operator-=(const Vector4f& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  Vector4f&
  Vector4f::operator*=(const Vector4f& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  Vector4f&
  Vector4f::operator/=(const Vector4f& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  Vector4f&
  Vector4f::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
    return *this;
  }

  Vector4f&
  Vector4f::operator/=(const float& scale) {
    x /= scale;
    y /= scale;
    z /= scale;
    w /= scale;
    return *this;
  }

  float
  Vector4f::dot(const Vector4f& a, const Vector4f& b) {
    return a | b;
  }

  Vector4f
  Vector4f::cross(const Vector4f& a, const Vector4f& b) {
    return a ^ b;
  }

  float
  Vector4f::projection(const Vector4f& a, const Vector4f& b) {
    return (a | b) / Math::sqr(a.magnitude());
  }

  float
  Vector4f::sqrDistance(const Vector4f& a, const Vector4f& b) {
    return Math::sqr(a.x - b.x) +
           Math::sqr(a.y - b.y) +
           Math::sqr(a.z - b.z) +
           Math::sqr(a.w - b.w);
  }

  float
  Vector4f::distance(const Vector4f& a, const Vector4f& b) {
    return 
      Math::sqrt(Math::sqr(a.x - b.x) +
                 Math::sqr(a.y - b.y) +
                 Math::sqr(a.z - b.z) +
                 Math::sqr(a.w - b.w));
  }

  void
  Vector4f::setValues(const float& newX, const float& newY, const float& newZ, const float& newW) {
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
  }

  void
  Vector4f::min(const Vector4f& v) {
    if (v.x < x) x = v.x;
    if (v.y < y) y = v.y;
    if (v.z < z) z = v.z;
    if (v.w < w) w = v.w;
  }

  void
  Vector4f::max(const Vector4f& v) {
    if (v.x > x) x = v.x;
    if (v.y > y) y = v.y;
    if (v.z > z) z = v.z;
    if (v.w > w) w = v.w;
  }

  void
  Vector4f::floor() {
    x = Math::floor(x);
    y = Math::floor(y);
    z = Math::floor(z);
    w = Math::floor(w);
  }

  void
  Vector4f::ceiling() {
    x = Math::ceil(x);
    y = Math::ceil(y);
    z = Math::ceil(z);
    w = Math::ceil(w);
  }

  void
  Vector4f::round() {
    x = Math::round(x);
    y = Math::round(y);
    z = Math::round(z);
    w = Math::round(w);
  }

  void
  Vector4f::roundHalf() {
    x = Math::roundHalf(x);
    y = Math::roundHalf(y);
    z = Math::roundHalf(z);
    w = Math::roundHalf(w);
  }

  float
  Vector4f::getHighest() const {
    return Math::max3(x, y, Math::max(z, w));
  }

  float
  Vector4f::getLowest() const {
    return Math::min3(x, y, Math::min(z, w));
  }

  float
  Vector4f::magnitude() const {
    return Math::sqrt(x * x + y * y + z * z + w * w);
  }

  float
  Vector4f::sqrMagnitude() const {
    return (x * x + y * y + z * z + w * w);
  }

  Vector4f
  Vector4f::normalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isNaN(w) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
              !Math::isInfinite(w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f) + Math::pow(w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    return Vector4f((x * unit), (y * unit), (z * unit), (w * unit));
  }

  Vector4f
  Vector4f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isNaN(w) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
              !Math::isInfinite(w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f) + Math::pow(w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    return Vector4f((x * unit), (y * unit), (z * unit), (w * unit));
  }

  void
  Vector4f::normalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isNaN(w) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
              !Math::isInfinite(w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f) + Math::pow(w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    x *= unit;
    y *= unit; 
    z *= unit;
    w *= unit;
  }

  void
  Vector4f::qNormalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isNaN(z) &&
              !Math::isNaN(w) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
              !Math::isInfinite(z) &&
              !Math::isInfinite(w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(x, 2.0f) + Math::pow(y, 2.0f) + Math::pow(z, 2.0f) + Math::pow(w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    x *= unit;
    y *= unit; 
    z *= unit;
    w *= unit;

  }

  bool
  Vector4f::isZero() const {
    return 0.0f == x && 0.0f == y && 0.0f == z && 0.0f == w;
  }

  bool
  Vector4f::isNearlySame(const Vector4f& a, const Vector4f& b, const float& error = Math::SMALLNUMBER) {
    return Math::abs(a.x - b.x) <= error &&
           Math::abs(a.y - b.y) <= error &&
           Math::abs(a.z - b.z) <= error &&
           Math::abs(a.w - b.w) <= error;
  }

  String
  Vector4f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", x);
    output += ", ";
    output += Utils::format("%2.2f", y);
    output += ", ";
    output += Utils::format("%2.2f", z);
    output += ", ";
    output += Utils::format("%2.2f", w);
    output += " )";

    return output;
  }

  Vector3f
  Vector4f::toVector3f() const {
    return Vector3f(*this);
  }
  
  const Vector4f Vector4f::ZERO   = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  
  const Vector4f Vector4f::ONE    = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  
  const Vector4f Vector4f::ONEX   = Vector4f(1.0f, 1.0f, 0.0f, 0.0f);
  
  const Vector4f Vector4f::ONEY   = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
  
  const Vector4f Vector4f::ONEZ   = Vector4f(0.0f, 0.0f, 1.0f, 0.0f);
  
  const Vector4f Vector4f::ONEW   = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
  
  const Vector4f Vector4f::RIGHT  = Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
  
  const Vector4f Vector4f::UP     = Vector4f(0.0f, 1.0f, 0.1f, 1.0f);
  
  const Vector4f Vector4f::FRONT  = Vector4f(0.0f, 0.0F, 1.0f, 1.0f);

}