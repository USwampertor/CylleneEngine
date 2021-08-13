
#include "cyUtilities.h"

#include "cyVector2f.h"
#include "cyVector2i.h"

#include "cyVector3f.h"

namespace CYLLENE_SDK {
  
  
  Vector3f::Vector3f(const Vector3f& other)
    : m_x(other.m_x),
      m_y(other.m_y),
      m_z(other.m_z) {}

  Vector3f::Vector3f(const Vector2f& other)
    : m_x(other[0]),
      m_y(other[1]),
      m_z(0.0f) {}

  Vector3f::Vector3f(const Vector2i& other)
    : m_x(static_cast<float>(other[0])),
      m_y(static_cast<float>(other[1])),
      m_z(0.0f) {}

  Vector3f::Vector3f(const Vector4f& other)
    : m_x(other[0]),
      m_y(other[1]),
      m_z(other[2]) {}


  Vector3f::Vector3f(const float& nx, const float& ny, const float& nz) 
    : m_x(nx),
      m_y(ny),
      m_z(nz) {}

  float&
  Vector3f::operator()(const uint32& index) {
    return (&m_x)[index];
  }

  const float&
  Vector3f::operator()(const uint32& index) const {
    return (&m_x)[index];
  }

  float&
  Vector3f::operator[](const uint32& index) {
    return (&m_x)[index];
  }

  const float
  Vector3f::operator[](const uint32& index) const {
    return (&m_x)[index];
  }


  Vector3f
  Vector3f::operator+(const Vector3f& v) const {
    return Vector3f(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
  }

  Vector3f
  Vector3f::operator-(const Vector3f& v) const {
    return Vector3f(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
  }

  Vector3f
  Vector3f::operator*(const Vector3f& v) const {
    return Vector3f(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
  }

  Vector3f
  Vector3f::operator/(const Vector3f& v) const {
    return Vector3f(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
  }

  Vector3f
  Vector3f::operator+(const float& plus) const {
    return Vector3f(m_x + plus, m_y + plus, m_z + plus);
  }

  Vector3f
  Vector3f::operator-(const float& minus) const {
    return Vector3f(m_x - minus, m_y - minus, m_z - minus);
  }

  Vector3f
  Vector3f::operator*(const float& times) const {
    return Vector3f(m_x * times, m_y * times, m_z * times);
  }

  Vector3f
  Vector3f::operator/(const float& under) const {
    return Vector3f(m_x / under, m_y / under, m_z / under);
  }

  float
  Vector3f::operator|(const Vector3f& v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
  }

  Vector3f
  Vector3f::operator^(const Vector3f& v) const {
    return Vector3f((m_y * v.m_z) - (m_z * v.m_y),
                    (m_z * v.m_x) - (m_x * v.m_z),
                    (m_x * v.m_y) - (m_y * v.m_x));
  }

  bool
  Vector3f::operator==(const Vector3f& v) const {
    return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;
  }

  bool
  Vector3f::operator!=(const Vector3f& v) const {
    return m_x != v.m_x || m_y != v.m_y || m_z != v.m_z;
  }

  bool
  Vector3f::operator<(const Vector3f& v) const {
    return m_x < v.m_x && m_y < v.m_y && m_z < v.m_z;
  }

  bool
  Vector3f::operator>(const Vector3f& v) const {
    return m_x > v.m_x && m_y > v.m_y && m_z > v.m_z;
  }

  bool
  Vector3f::operator<=(const Vector3f& v) const {
    return m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z;
  }

  bool
  Vector3f::operator>=(const Vector3f& v) const {
    return m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z;
  }

  Vector3f
  Vector3f::operator-() const {
    return Vector3f(-m_x, -m_y, -m_z);
  }

  Vector3f&
  Vector3f::operator+=(const Vector3f& v) {
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    return *this;
  }

  Vector3f&
  Vector3f::operator-=(const Vector3f& v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
    return *this;
  }

  Vector3f&
  Vector3f::operator*=(const Vector3f& v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
    return *this;
  }

  Vector3f&
  Vector3f::operator/=(const Vector3f& v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    m_z /= v.m_z;
    return *this;
  }

  Vector3f&
  Vector3f::operator*=(const float& scale) {
    m_x *= scale;
    m_y *= scale;
    m_z *= scale;
    return *this;
  }

  Vector3f&
  Vector3f::operator/=(const float& scale) {
    m_x /= scale;
    m_y /= scale;
    m_z /= scale;
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
    return Math::sqr(a.m_x - b.m_x) +
           Math::sqr(a.m_y - b.m_y) +
           Math::sqr(a.m_z - b.m_z);
  }

  float
  Vector3f::distance(const Vector3f& a, const Vector3f& b) {
    return 
      Math::sqrt(Math::sqr(a.m_x - b.m_x) +
                 Math::sqr(a.m_y - b.m_y) +
                 Math::sqr(a.m_z - b.m_z));
  }

  void
  Vector3f::setValues(const float& newX, const float& newY, const float& newZ) {
    m_x = newX;
    m_y = newY;
    m_z = newZ;
  }

  void
  Vector3f::min(const Vector3f& v) {
    if (v.m_x < m_x) m_x = v.m_x;
    if (v.m_y < m_y) m_y = v.m_y;
    if (v.m_z < m_z) m_z = v.m_z;
  }

  void
  Vector3f::max(const Vector3f& v) {
    if (v.m_x > m_x) m_x = v.m_x;
    if (v.m_y > m_y) m_y = v.m_y;
    if (v.m_z > m_z) m_z = v.m_z;
  }

  void
  Vector3f::floor() {
    m_x = Math::floor(m_x);
    m_y = Math::floor(m_y);
    m_z = Math::floor(m_z);
  }

  void
  Vector3f::ceiling() {
    m_x = Math::ceil(m_x);
    m_y = Math::ceil(m_y);
    m_z = Math::ceil(m_z);
  }

  void
  Vector3f::round() {
    m_x = Math::round(m_x);
    m_y = Math::round(m_y);
    m_z = Math::round(m_z);
  }

  void
  Vector3f::roundHalf() {
    m_x = Math::roundHalf(m_x);
    m_y = Math::roundHalf(m_y);
    m_z = Math::roundHalf(m_z);
  }

  float
  Vector3f::getHighest() const {
    return Math::max3(m_x, m_y, m_z);
  }

  float
  Vector3f::getLowest() const {
    return Math::min3(m_x, m_y, m_z);
  }

  float
  Vector3f::magnitude() const {
    return Math::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
  }

  float
  Vector3f::sqrMagnitude() const {
    return (m_x * m_x + m_y * m_y + m_z * m_z);
  }

  Vector3f
  Vector3f::normalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    return Vector3f((m_x * unit), (m_y * unit), (m_z * unit));
  }

  Vector3f
  Vector3f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    return Vector3f((m_x * unit), (m_y * unit), (m_z * unit));
  }

  void
  Vector3f::normalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    m_x *= unit;
    m_y *= unit; 
    m_z *= unit;
  }

  void
  Vector3f::qNormalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    m_x *= unit;
    m_y *= unit; 
    m_z *= unit;

  }

  bool
  Vector3f::isZero() const {
    return 0.0f == m_x && 0.0f == m_y && 0.0f == m_z;
  }

  bool
  Vector3f::isNearlySame(const Vector3f& a, const Vector3f& b, float error) {
    return Math::abs(a.m_x - b.m_x) <= error &&
           Math::abs(a.m_y - b.m_y) <= error &&
           Math::abs(a.m_z - b.m_z) <= error;
  }

  String
  Vector3f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", m_x);
    output += ", ";
    output += Utils::format("%2.2f", m_y);
    output += ", ";
    output += Utils::format("%2.2f", m_z);
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