
#include "cyMath.h"
#include "cyUtilities.h"

#include "cyVector2i.h"
#include "cyVector2f.h"

#include "cyVector3f.h"

#include "cyVector4f.h"

namespace CYLLENE_SDK {

  Vector2i::Vector2i(const int32& nx, const int32& ny)
    : m_x(nx),
      m_y(ny) {}

  Vector2i::Vector2i(const int32& values)
    : m_x(values), 
      m_y(values) {}

  Vector2i::Vector2i(const Vector2f& other) 
    : m_x(static_cast<int>(other[0])), 
      m_y(static_cast<int>(other[1])) {}

  Vector2i::Vector2i(const Vector2i& other) 
    : m_x(static_cast<int>(other[0])),
      m_y(static_cast<int>(other[1])) {}

  Vector2i::Vector2i(const Vector3f& other) 
    : m_x(static_cast<int>(other[0])),
      m_y(static_cast<int>(other[1])) {}

  Vector2i::Vector2i(const Vector4f& other) 
    : m_x(static_cast<int>(other[0])),
      m_y(static_cast<int>(other[1])) {}

  int32&
  Vector2i::operator()(const int32& index) {
    return (&m_x)[index];
  }

  const int32&
  Vector2i::operator()(const int32& index) const {
    return (&m_x)[index];
  }

  int32&
  Vector2i::operator[](const int32& index) {
    return (&m_x)[index];
  }
  
  const int32
  Vector2i::operator[](const int32& index) const {
    return (&m_x)[index];
  }


  Vector2i
  Vector2i::operator+(const Vector2i& v) const {
    return Vector2i(m_x + v.m_x, m_y + v.m_y);
  }

  Vector2i
  Vector2i::operator-(const Vector2i& v) const {
    return Vector2i(m_x - v.m_x, m_y - v.m_y);
  }

  Vector2i
  Vector2i::operator*(const Vector2i& v) const {
    return Vector2i(m_x * v.m_x, m_y * v.m_y);
  }

  Vector2i
  Vector2i::operator/(const Vector2i& v) const {
    return Vector2i(m_x / v.m_x, m_y / v.m_y);
  }

  Vector2i
  Vector2i::operator+(const int32& plus) const {
    return Vector2i(m_x + plus, m_y + plus);
  }

  Vector2i
  Vector2i::operator-(const int32& minus) const {
    return Vector2i(m_x - minus, m_y - minus);
  }

  Vector2i
  Vector2i::operator*(const int32& times) const {
    return Vector2i(m_x * times, m_y * times);
  }

  Vector2i
  Vector2i::operator/(const int32& under) const {
    return Vector2i(m_x / under, m_y / under);
  }

  int32
  Vector2i::operator|(const Vector2i& v) const {
    return m_x * v.m_x + m_y * v.m_y;
  }

  int32
  Vector2i::operator^(const Vector2i& v) const {
    return m_x * v.m_x - m_y * v.m_y;
  }

  bool
  Vector2i::operator==(const Vector2i& v) const {
    return m_x == v.m_x && m_y == v.m_y;
  }

  bool
  Vector2i::operator!=(const Vector2i& v) const {
    return m_x != v.m_x || m_y != v.m_y;
  }

  bool
  Vector2i::operator<(const Vector2i& v) const {
    return m_x < v.m_x && m_y < v.m_y;
  }

  bool
  Vector2i::operator>(const Vector2i& v) const {
    return m_x > v.m_x && m_y > v.m_y;
  }

  bool
  Vector2i::operator<=(const Vector2i& v) const {
    return m_x <= v.m_x && m_y <= v.m_y;
  }

  bool
  Vector2i::operator>=(const Vector2i& v) const {
    return m_x >= v.m_x && m_y >= v.m_y;
  }

  Vector2i
  Vector2i::operator-() const {
    return Vector2i(-m_x, -m_y);
  }

  Vector2i&
  Vector2i::operator+=(const Vector2i& v) {
    m_x += v.m_x;
    m_y += v.m_y;
    return *this;
  }

  Vector2i&
  Vector2i::operator-=(const Vector2i& v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    return *this;
  }

  Vector2i&
  Vector2i::operator*=(const Vector2i& v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    return *this;
  }

  Vector2i&
  Vector2i::operator/=(const Vector2i& v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    return *this;
  }

  Vector2i&
  Vector2i::operator*=(const int32& scale) {
    m_x *= scale;
    m_y *= scale;
    return *this;
  }

  Vector2i&
  Vector2i::operator/=(const int32& scale) {
    m_x /= scale;
    m_y /= scale;
    return *this;
  }

  int32
  Vector2i::dot(const Vector2i& a, const Vector2i& b) {
    return a | b;
  }

  int32
  Vector2i::cross(const Vector2i& a, const Vector2i& b) {
    return a ^ b;
  }

  int32
  Vector2i::projection(const Vector2i& a, const Vector2i& b) {
    return (a | b) / a.magnitude();
  }

  int32
  Vector2i::sqrDistance(const Vector2i& a, const Vector2i& b) {
    return Math::pow(a.m_x - b.m_x, 2) + Math::pow(a.m_y - b.m_y, 2);
  }

  int32
  Vector2i::distance(const Vector2i& a, const Vector2i& b) {
    return Math::sqrt(Math::sqr(a.m_x + b.m_x) + Math::sqr(a.m_y + b.m_y));
  }

  void
  Vector2i::setValues(const int32& newX, const int32& newY) {
    m_x = newX;
    m_y = newY;
  }

  void
  Vector2i::min(const Vector2i& v) {
    if (v.m_x < m_x) { m_x = v.m_x; }
    if (v.m_y < m_y) { m_y = v.m_y; }
  }

  void
  Vector2i::max(const Vector2i& v) {
    if (v.m_x > m_x) { m_x = v.m_x; }
    if (v.m_y > m_y) { m_y = v.m_y; }
  }

  int32
  Vector2i::getHighest() const {
    return Math::max(m_x, m_y);
  }

  int32
  Vector2i::getLowest() const {
    return Math::min(m_x, m_y);
  }

  int32
  Vector2i::magnitude() const {
    return Math::sqrt(m_x * m_x + m_y * m_y);
  }

  int32
  Vector2i::sqrMagnitude() const {
    return (m_x * m_x + m_y * m_y);
  }

  Vector2i
  Vector2i::normalized() const {

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
    return Vector2i((m_x * static_cast<int32>(unit)), (m_y * static_cast<int32>(unit)));
  }

  Vector2i
  Vector2i::qNormalized() const {

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
    return Vector2i((m_x * static_cast<int32>(unit)), (m_y * static_cast<int32>(unit)));
  }

  void
  Vector2i::normalize() {

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
    m_x *= static_cast<int32>(unit);
    m_y *= static_cast<int32>(unit);
  }

  void
  Vector2i::qNormalize() {

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
    m_x *= static_cast<int32>(unit);
    m_y *= static_cast<int32>(unit);
  }

  bool
  Vector2i::isZero() const {
    return 0 == m_x && 0 == m_y;
  }

  bool
  Vector2i::areSame(const Vector2i& a, const Vector2i& b) {
    return a == b;
  }

  bool
  Vector2i::areNearlySame(const Vector2i& a, const Vector2i& b, const int32& threshold) {
    return Math::abs(a.m_x - b.m_x) <= threshold &&
           Math::abs(a.m_y - b.m_y) <= threshold ;
  }

  String
  Vector2i::toString() {
    
    String output;

    output += "( ";
    output += Utils::format("%2d", m_x);
    output += ", ";
    output += Utils::format("%2d", m_y);
    output += " )";

    return output;
  }

  const Vector2i Vector2i::ZERO   = Vector2i(0, 0);

  const Vector2i Vector2i::ONE    = Vector2i(1, 1);

  const Vector2i Vector2i::ONEX   = Vector2i(1, 0);

  const Vector2i Vector2i::ONEY   = Vector2i(0, 1);

  const Vector2i Vector2i::UP     = Vector2i(0, 1);
  
  const Vector2i Vector2i::RIGHT  = Vector2i(1, 0);
}