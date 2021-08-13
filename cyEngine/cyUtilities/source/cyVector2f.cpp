
#include "cyMath.h"
#include "cyUtilities.h"

#include "cyVector2i.h"
#include "cyVector2f.h"

#include "cyVector3f.h"

#include "cyVector4f.h"

namespace CYLLENE_SDK {

  Vector2f::Vector2f(const float& nx, const float& ny)
    : m_x(nx),
      m_y(ny) {}

  Vector2f::Vector2f(const float& values)
    : m_x(values), 
      m_y(values) {}

  Vector2f::Vector2f(const Vector2i& other) 
    : m_x(static_cast<float>(other[0])), 
      m_y(static_cast<float>(other[1])) {}

  Vector2f::Vector2f(const Vector2f& other) 
    : m_x(other[0]), 
      m_y(other[1]) {}

  Vector2f::Vector2f(const Vector3f& other) 
    : m_x(other[0]),
      m_y(other[1]) {}

  Vector2f::Vector2f(const Vector4f& other) 
    : m_x(other[0]), 
      m_y(other[1]) {}

  float&
  Vector2f::operator()(const uint32& index) {
    return (&m_x)[index];
  }

  const float&
  Vector2f::operator()(const uint32& index) const {
    return (&m_x)[index];
  }

  float&
  Vector2f::operator[](const uint32& index) {
    return (&m_x)[index];
  }

  const float
  Vector2f::operator[](const uint32& index) const {
    return (&m_x)[index];
  }


  Vector2f
  Vector2f::operator+(const Vector2f& v) const {
    return Vector2f(m_x + v.m_x, m_y + v.m_y);
  }

  Vector2f
  Vector2f::operator-(const Vector2f& v) const {
    return Vector2f(m_x - v.m_x, m_y - v.m_y);
  }

  Vector2f
  Vector2f::operator*(const Vector2f& v) const {
    return Vector2f(m_x * v.m_x, m_y * v.m_y);
  }

  Vector2f
  Vector2f::operator/(const Vector2f& v) const {
    return Vector2f(m_x / v.m_x, m_y / v.m_y);
  }

  Vector2f
  Vector2f::operator+(const float& plus) const {
    return Vector2f(m_x + plus, m_y + plus);
  }

  Vector2f
  Vector2f::operator-(const float& minus) const {
    return Vector2f(m_x - minus, m_y - minus);
  }

  Vector2f
  Vector2f::operator*(const float& times) const {
    return Vector2f(m_x * times, m_y * times);
  }

  Vector2f
  Vector2f::operator/(const float& under) const {
    return Vector2f(m_x / under, m_y / under);
  }

  float
  Vector2f::operator|(const Vector2f& v) const {
    return m_x * v.m_x + m_y * v.m_y;
  }

  float
  Vector2f::operator^(const Vector2f& v) const {
    return m_x * v.m_x - m_y * v.m_y;
  }

  bool
  Vector2f::operator==(const Vector2f& v) const {
    return m_x == v.m_x && m_y == v.m_y;
  }

  bool
  Vector2f::operator!=(const Vector2f& v) const {
    return m_x != v.m_x || m_y != v.m_y;
  }

  bool
  Vector2f::operator<(const Vector2f& v) const {
    return m_x < v.m_x&& m_y < v.m_y;
  }

  bool
  Vector2f::operator>(const Vector2f& v) const {
    return m_x > v.m_x && m_y > v.m_y;
  }

  bool
  Vector2f::operator<=(const Vector2f& v) const {
    return m_x <= v.m_x && m_y <= v.m_y;
  }

  bool
  Vector2f::operator>=(const Vector2f& v) const {
    return m_x >= v.m_x && m_y >= v.m_y;
  }

  Vector2f
  Vector2f::operator-() const {
    return Vector2f(-m_x, -m_y);
  }

  Vector2f&
  Vector2f::operator+=(const Vector2f& v) {
    m_x += v.m_x;
    m_y += v.m_y;
    return *this;
  }

  Vector2f&
  Vector2f::operator-=(const Vector2f& v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    return *this;
  }

  Vector2f&
  Vector2f::operator*=(const Vector2f& v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    return *this;
  }

  Vector2f&
  Vector2f::operator/=(const Vector2f& v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    return *this;
  }

  Vector2f&
  Vector2f::operator*=(const float& scale) {
    m_x *= scale;
    m_y *= scale;
    return *this;
  }

  Vector2f&
  Vector2f::operator/=(const float& scale) {
    m_x /= scale;
    m_y /= scale;
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
    return Math::pow(a.m_x - b.m_x, 2.0f) + Math::pow(a.m_y - b.m_y, 2.0f);
  }

  float
  Vector2f::distance(const Vector2f& a, const Vector2f& b) {
    return Math::sqrt(Math::sqr(a.m_x + b.m_x) + Math::sqr(a.m_y + b.m_y));
  }

  void
  Vector2f::setValues(const float& newX, const float& newY) {
    m_x = newX;
    m_y = newY;
  }

  void
  Vector2f::min(const Vector2f& v) {
    if (v.m_x < m_x) { m_x = v.m_x; }
    if (v.m_y < m_y) { m_y = v.m_y; }
  }

  void
  Vector2f::max(const Vector2f& v) {
    if (v.m_x > m_x) { m_x = v.m_x; }
    if (v.m_y > m_y) { m_y = v.m_y; }
  }

  float
  Vector2f::getHighest() const {
    return Math::max(m_x, m_y);
  }

  float
  Vector2f::getLowest() const {
    return Math::min(m_x, m_y);
  }

  float
  Vector2f::magnitude() const {
    return Math::sqrt(m_x * m_x + m_y * m_y);
  }

  float
  Vector2f::sqrMagnitude() const {
    return (m_x * m_x + m_y * m_y);
  }

  Vector2f
  Vector2f::normalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(m_x), 2.0f) +
                Math::pow(static_cast<float>(m_y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);
    return Vector2f((m_x * static_cast<float>(unit)), (m_y * static_cast<float>(unit)));
  }

  Vector2f
  Vector2f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(m_x), 2.0f) +
                Math::pow(static_cast<float>(m_y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);
    return Vector2f((m_x * static_cast<float>(unit)), (m_y * static_cast<float>(unit)));
  }

  void
  Vector2f::normalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(m_x), 2.0f) +
                Math::pow(static_cast<float>(m_y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);
    m_x *= static_cast<float>(unit);
    m_y *= static_cast<float>(unit);
  }

  void
  Vector2f::qNormalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
               Utils::format("Value X or Y are either infinite or NAN").c_str());

    float sqr = Math::pow(static_cast<float>(m_x), 2.0f) +
                Math::pow(static_cast<float>(m_y), 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);
    m_x *= static_cast<float>(unit);
    m_y *= static_cast<float>(unit);
  }

  bool
  Vector2f::isZero() const {
    return 0 == m_x && 0 == m_y;
  }

  bool
  Vector2f::isSame(const Vector2f& a, const Vector2f& b) {
    return a == b;
  }

  bool
  Vector2f::isNearlySame(const Vector2f& a, const Vector2f& b, const float& threshold) {
    return Math::abs(a.m_x - b.m_x) <= threshold &&
           Math::abs(a.m_y - b.m_y) <= threshold ;
  }

  String
  Vector2f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", m_x);
    output += ", ";
    output += Utils::format("%2.2f", m_y);
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