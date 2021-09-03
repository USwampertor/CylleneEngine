
#include "cyUtilities.h"

#include "cyVector2i.h"
#include "cyVector2f.h"

#include "cyVector3f.h"

#include "cyVector4f.h"

#include "cyMath.h"

namespace CYLLENE_SDK {

  Vector2f::Vector2f(const float& nx, const float& ny)
    : x(nx),
      y(ny) {}

  Vector2f::Vector2f(const float& values)
    : x(values), 
      y(values) {}

  Vector2f::Vector2f(const Vector2i& other) 
    : x(static_cast<float>(other[0])), 
      y(static_cast<float>(other[1])) {}

  Vector2f::Vector2f(const Vector2f& other) 
    : x(other[0]), 
      y(other[1]) {}

  Vector2f::Vector2f(const Vector3f& other) 
    : x(other[0]),
      y(other[1]) {}

  Vector2f::Vector2f(const Vector4f& other) 
    : x(other[0]), 
      y(other[1]) {}

  float&
  Vector2f::operator()(const uint32& index) {
    return (&x)[index];
  }

  const float&
  Vector2f::operator()(const uint32& index) const {
    return (&x)[index];
  }

  float&
  Vector2f::operator[](const uint32& index) {
    return (&x)[index];
  }

  const float
  Vector2f::operator[](const uint32& index) const {
    return (&x)[index];
  }


  Vector2f
  Vector2f::operator+(const Vector2f& v) const {
    return Vector2f(x + v.x, y + v.y);
  }

  Vector2f
  Vector2f::operator-(const Vector2f& v) const {
    return Vector2f(x - v.x, y - v.y);
  }

  Vector2f
  Vector2f::operator*(const Vector2f& v) const {
    return Vector2f(x * v.x, y * v.y);
  }

  Vector2f
  Vector2f::operator/(const Vector2f& v) const {
    return Vector2f(x / v.x, y / v.y);
  }

  Vector2f
  Vector2f::operator+(const float& plus) const {
    return Vector2f(x + plus, y + plus);
  }

  Vector2f
  Vector2f::operator-(const float& minus) const {
    return Vector2f(x - minus, y - minus);
  }

  Vector2f
  Vector2f::operator*(const float& times) const {
    return Vector2f(x * times, y * times);
  }

  Vector2f
  Vector2f::operator/(const float& under) const {
    return Vector2f(x / under, y / under);
  }

  float
  Vector2f::operator|(const Vector2f& v) const {
    return x * v.x + y * v.y;
  }

  float
  Vector2f::operator^(const Vector2f& v) const {
    return x * v.x - y * v.y;
  }

  bool
  Vector2f::operator==(const Vector2f& v) const {
    return x == v.x && y == v.y;
  }

  bool
  Vector2f::operator!=(const Vector2f& v) const {
    return x != v.x || y != v.y;
  }

  bool
  Vector2f::operator<(const Vector2f& v) const {
    return x < v.x&& y < v.y;
  }

  bool
  Vector2f::operator>(const Vector2f& v) const {
    return x > v.x && y > v.y;
  }

  bool
  Vector2f::operator<=(const Vector2f& v) const {
    return x <= v.x && y <= v.y;
  }

  bool
  Vector2f::operator>=(const Vector2f& v) const {
    return x >= v.x && y >= v.y;
  }

  Vector2f
  Vector2f::operator-() const {
    return Vector2f(-x, -y);
  }

  Vector2f&
  Vector2f::operator+=(const Vector2f& v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector2f&
  Vector2f::operator-=(const Vector2f& v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vector2f&
  Vector2f::operator*=(const Vector2f& v) {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  Vector2f&
  Vector2f::operator/=(const Vector2f& v) {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  Vector2f&
  Vector2f::operator*=(const float& scale) {
    x *= scale;
    y *= scale;
    return *this;
  }

  Vector2f&
  Vector2f::operator/=(const float& scale) {
    x /= scale;
    y /= scale;
    return *this;
  }

  float
  Vector2f::dot(const Vector2f& a, const Vector2f& b) {
    return a | b;
  }

  float
  Vector2f::cross(const Vector2f& a, const Vector2f& b) {
    return a ^ b;
  }

  float
  Vector2f::projection(const Vector2f& a, const Vector2f& b) {
    return (a | b) / a.magnitude();
  }

  float
  Vector2f::sqrDistance(const Vector2f& a, const Vector2f& b) {
    return Math::pow(a.x - b.x, 2.0f) + Math::pow(a.y - b.y, 2.0f);
  }

  float
  Vector2f::distance(const Vector2f& a, const Vector2f& b) {
    return Math::sqrt(Math::sqr(a.x + b.x) + Math::sqr(a.y + b.y));
  }

  void
  Vector2f::setValues(const float& newX, const float& newY) {
    x = newX;
    y = newY;
  }

  void
  Vector2f::min(const Vector2f& v) {
    if (v.x < x) { x = v.x; }
    if (v.y < y) { y = v.y; }
  }

  void
  Vector2f::max(const Vector2f& v) {
    if (v.x > x) { x = v.x; }
    if (v.y > y) { y = v.y; }
  }

  float
  Vector2f::getHighest() const {
    return Math::max(x, y);
  }

  float
  Vector2f::getLowest() const {
    return Math::min(x, y);
  }

  float
  Vector2f::magnitude() const {
    return Math::sqrt(x * x + y * y);
  }

  float
  Vector2f::sqrMagnitude() const {
    return (x * x + y * y);
  }

  Vector2f
  Vector2f::normalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(x), 2.0f) +
                Math::pow(static_cast<float>(y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);
    return Vector2f((x * static_cast<float>(unit)), (y * static_cast<float>(unit)));
  }

  Vector2f
  Vector2f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(x), 2.0f) +
                Math::pow(static_cast<float>(y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);
    return Vector2f((x * static_cast<float>(unit)), (y * static_cast<float>(unit)));
  }

  void
  Vector2f::normalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(x), 2.0f) +
                Math::pow(static_cast<float>(y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);
    x *= static_cast<float>(unit);
    y *= static_cast<float>(unit);
  }

  void
  Vector2f::qNormalize() {

    CY_ASSERT(!Math::isNaN(x) &&
              !Math::isNaN(y) &&
              !Math::isInfinite(x) &&
              !Math::isInfinite(y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(x), 2.0f) +
                Math::pow(static_cast<float>(y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);
    x *= static_cast<float>(unit);
    y *= static_cast<float>(unit);
  }

  bool
  Vector2f::isZero() const {
    return 0 == x && 0 == y;
  }

  bool
  Vector2f::isSame(const Vector2f& a, const Vector2f& b) {
    return a == b;
  }

  bool
  Vector2f::isNearlySame(const Vector2f& a, const Vector2f& b, const float& error = Math::SMALLNUMBER) {
    return Math::abs(a.x - b.x) <= error &&
           Math::abs(a.y - b.y) <= error ;
  }

  String
  Vector2f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", x);
    output += ", ";
    output += Utils::format("%2.2f", y);
    output += " )";

    return output;
  }

  const Vector2f Vector2f::ZERO   = Vector2f(0, 0);

  const Vector2f Vector2f::ONE    = Vector2f(1, 1);

  const Vector2f Vector2f::ONEX   = Vector2f(1, 0);

  const Vector2f Vector2f::ONEY   = Vector2f(0, 1);

  const Vector2f Vector2f::UP     = Vector2f(0, 1);

  const Vector2f Vector2f::RIGHT  = Vector2f(1, 0);
}