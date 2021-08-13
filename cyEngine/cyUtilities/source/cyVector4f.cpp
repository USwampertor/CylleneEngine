
#include "cyVector4f.h"
#include "cyUtilities.h"

#include "cyVector3f.h"

#include "cyMath.h"

namespace CYLLENE_SDK {
  
  
  Vector4f::Vector4f(const Vector4f& other)
    : m_x(other.m_x),
      m_y(other.m_y),
      m_z(other.m_z),
      m_w(other.m_w) {}

  Vector4f::Vector4f(const Vector3f& other)
    : m_x(other[0]),
      m_y(other[1]),
      m_z(other[2]),
      m_w(0) {}


  Vector4f::Vector4f(const float& nx, const float& ny, const float& nz, const float& nw)
    : m_x(nx),
      m_y(ny),
      m_z(nz),
      m_w(nw) {}

  float&
  Vector4f::operator()(const uint32& index) {
    return (&m_x)[index];
  }

  const float&
  Vector4f::operator()(const uint32& index) const {
    return (&m_x)[index];
  }

  float&
  Vector4f::operator[](const uint32& index) {
    return (&m_x)[index];
  }

  const float
  Vector4f::operator[](const uint32& index) const {
    return (&m_x)[index];
  }


  Vector4f
  Vector4f::operator+(const Vector4f& v) const {
    return Vector4f(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z, m_w + v.m_w);
  }

  Vector4f
  Vector4f::operator-(const Vector4f& v) const {
    return Vector4f(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z, m_w - v.m_w);
  }

  Vector4f
  Vector4f::operator*(const Vector4f& v) const {
    return Vector4f(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z, m_w * v.m_w);
  }

  Vector4f
  Vector4f::operator/(const Vector4f& v) const {
    return Vector4f(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z, m_w / v.m_w);
  }

  Vector4f
  Vector4f::operator+(const float& plus) const {
    return Vector4f(m_x + plus, m_y + plus, m_z + plus, m_w + plus);
  }

  Vector4f
  Vector4f::operator-(const float& minus) const {
    return Vector4f(m_x - minus, m_y - minus, m_z - minus, m_w - minus);
  }

  Vector4f
  Vector4f::operator*(const float& times) const {
    return Vector4f(m_x * times, m_y * times, m_z * times, m_w * times);
  }

  Vector4f
  Vector4f::operator/(const float& under) const {
    return Vector4f(m_x / under, m_y / under, m_z / under, m_w / under);
  }

  float
  Vector4f::operator|(const Vector4f& v) const {
    return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z + m_w * v.m_w;
  }

  Vector4f
  Vector4f::operator^(const Vector4f& v) const {
    return Vector4f((m_y * v.m_z) - (m_z * v.m_y),
                    (m_z * v.m_x) - (m_x * v.m_z),
                    (m_x * v.m_y) - (m_y * v.m_x),
                    0);
  }

  bool
  Vector4f::operator==(const Vector4f& v) const {
    return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z && m_w == v.m_w;
  }

  bool
  Vector4f::operator!=(const Vector4f& v) const {
    return m_x != v.m_x || m_y != v.m_y || m_z != v.m_z || m_w != v.m_w;
  }

  bool
  Vector4f::operator<(const Vector4f& v) const {
    return m_x < v.m_x && m_y < v.m_y && m_z < v.m_z && m_w < v.m_w;
  }

  bool
  Vector4f::operator>(const Vector4f& v) const {
    return m_x > v.m_x && m_y > v.m_y && m_z > v.m_z && m_w > v.m_w;
  }

  bool
  Vector4f::operator<=(const Vector4f& v) const {
    return m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z && m_w <= v.m_w;
  }

  bool
  Vector4f::operator>=(const Vector4f& v) const {
    return m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z && m_w >= v.m_w;
  }

  Vector4f
  Vector4f::operator-() const {
    return Vector4f(-m_x, -m_y, -m_z, -m_w);
  }

  Vector4f&
  Vector4f::operator+=(const Vector4f& v) {
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
    m_w += v.m_w;
    return *this;
  }

  Vector4f&
  Vector4f::operator-=(const Vector4f& v) {
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
    m_w -= v.m_w;
    return *this;
  }

  Vector4f&
  Vector4f::operator*=(const Vector4f& v) {
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
    m_w *= v.m_w;
    return *this;
  }

  Vector4f&
  Vector4f::operator/=(const Vector4f& v) {
    m_x /= v.m_x;
    m_y /= v.m_y;
    m_z /= v.m_z;
    m_w /= v.m_w;
    return *this;
  }

  Vector4f&
  Vector4f::operator*=(const float& scale) {
    m_x *= scale;
    m_y *= scale;
    m_z *= scale;
    m_w *= scale;
    return *this;
  }

  Vector4f&
  Vector4f::operator/=(const float& scale) {
    m_x /= scale;
    m_y /= scale;
    m_z /= scale;
    m_w /= scale;
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
    return Math::sqr(a.m_x - b.m_x) +
           Math::sqr(a.m_y - b.m_y) +
           Math::sqr(a.m_z - b.m_z) +
           Math::sqr(a.m_w - b.m_w);
  }

  float
  Vector4f::distance(const Vector4f& a, const Vector4f& b) {
    return 
      Math::sqrt(Math::sqr(a.m_x - b.m_x) +
                 Math::sqr(a.m_y - b.m_y) +
                 Math::sqr(a.m_z - b.m_z) +
                 Math::sqr(a.m_w - b.m_w));
  }

  void
  Vector4f::setValues(const float& newX, const float& newY, const float& newZ, const float& newW) {
    m_x = newX;
    m_y = newY;
    m_z = newZ;
    m_w = newW;
  }

  void
  Vector4f::min(const Vector4f& v) {
    if (v.m_x < m_x) m_x = v.m_x;
    if (v.m_y < m_y) m_y = v.m_y;
    if (v.m_z < m_z) m_z = v.m_z;
    if (v.m_w < m_w) m_w = v.m_w;
  }

  void
  Vector4f::max(const Vector4f& v) {
    if (v.m_x > m_x) m_x = v.m_x;
    if (v.m_y > m_y) m_y = v.m_y;
    if (v.m_z > m_z) m_z = v.m_z;
    if (v.m_w > m_w) m_w = v.m_w;
  }

  void
  Vector4f::floor() {
    m_x = Math::floor(m_x);
    m_y = Math::floor(m_y);
    m_z = Math::floor(m_z);
    m_w = Math::floor(m_w);
  }

  void
  Vector4f::ceiling() {
    m_x = Math::ceil(m_x);
    m_y = Math::ceil(m_y);
    m_z = Math::ceil(m_z);
    m_w = Math::ceil(m_w);
  }

  void
  Vector4f::round() {
    m_x = Math::round(m_x);
    m_y = Math::round(m_y);
    m_z = Math::round(m_z);
    m_w = Math::round(m_w);
  }

  void
  Vector4f::roundHalf() {
    m_x = Math::roundHalf(m_x);
    m_y = Math::roundHalf(m_y);
    m_z = Math::roundHalf(m_z);
    m_w = Math::roundHalf(m_w);
  }

  float
  Vector4f::getHighest() const {
    return Math::max3(m_x, m_y, Math::max(m_z, m_w));
  }

  float
  Vector4f::getLowest() const {
    return Math::min3(m_x, m_y, Math::min(m_z, m_w));
  }

  float
  Vector4f::magnitude() const {
    return Math::sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
  }

  float
  Vector4f::sqrMagnitude() const {
    return (m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
  }

  Vector4f
  Vector4f::normalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isNaN(m_w) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
              !Math::isInfinite(m_w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f) + Math::pow(m_w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    return Vector4f((m_x * unit), (m_y * unit), (m_z * unit), (m_w * unit));
  }

  Vector4f
  Vector4f::qNormalized() const {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isNaN(m_w) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
              !Math::isInfinite(m_w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f) + Math::pow(m_w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
      Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    return Vector4f((m_x * unit), (m_y * unit), (m_z * unit), (m_w * unit));
  }

  void
  Vector4f::normalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isNaN(m_w) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
              !Math::isInfinite(m_w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f) + Math::pow(m_w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::invSqrt(sqr);

    m_x *= unit;
    m_y *= unit; 
    m_z *= unit;
    m_w *= unit;
  }

  void
  Vector4f::qNormalize() {

    CY_ASSERT(!Math::isNaN(m_x) &&
              !Math::isNaN(m_y) &&
              !Math::isNaN(m_z) &&
              !Math::isNaN(m_w) &&
              !Math::isInfinite(m_x) &&
              !Math::isInfinite(m_y) &&
              !Math::isInfinite(m_z) &&
              !Math::isInfinite(m_w) &&
               Utils::format("Value X Y or Z are either infinite or NAN").c_str());

    float sqr = Math::pow(m_x, 2.0f) + Math::pow(m_y, 2.0f) + Math::pow(m_z, 2.0f) + Math::pow(m_w, 2.0f);

    CY_ASSERT(sqr <= Math::EPSILONF &&
              Utils::format("Invalid normalization: value inside square root is %f", sqr).c_str());

    CY_DEBUG_ONLY(sqrMagnitude());

    float unit = Math::qInvSqrt(sqr);

    m_x *= unit;
    m_y *= unit; 
    m_z *= unit;
    m_w *= unit;

  }

  bool
  Vector4f::isZero() const {
    return 0.0f == m_x && 0.0f == m_y && 0.0f == m_z && 0.0f == m_w;
  }

  bool
  Vector4f::isNearlySame(const Vector4f& a, const Vector4f& b, const float& error = Math::SMALLNUMBER) {
    return Math::abs(a.m_x - b.m_x) <= error &&
           Math::abs(a.m_y - b.m_y) <= error &&
           Math::abs(a.m_z - b.m_z) <= error &&
           Math::abs(a.m_w - b.m_w) <= error;
  }

  String
  Vector4f::toString() {

    String output;

    output += "( ";
    output += Utils::format("%2.2f", m_x);
    output += ", ";
    output += Utils::format("%2.2f", m_y);
    output += ", ";
    output += Utils::format("%2.2f", m_z);
    output += ", ";
    output += Utils::format("%2.2f", m_w);
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