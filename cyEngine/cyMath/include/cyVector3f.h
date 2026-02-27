/**
 * @file cyVector3f.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Vector3f.
 */
#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

// Forward declaration
class Vector2f;
class Vector2i;
class Vector4f;

/**
 * @class Vector3f
 * @brief A vector object with x y z components
 */
class CY_MATH_EXPORT Vector3f {
public:
  /**
   * @brief Constructs an uninitialized vector.
   */
  Vector3f() = default;

  /**
   * @brief Constructs a copy of another vector.
   * @param other Source vector.
   */
  Vector3f(const Vector3f& other);

  /**
   * @brief Constructs from a 2D float vector.
   * @param other Source 2D vector.
   */
  Vector3f(const Vector2f& other);

  /**
   * @brief Constructs from a 2D integer vector.
   * @param other Source 2D vector.
   */
  Vector3f(const Vector2i& other);

  /**
   * @brief Constructs from a 4D vector.
   * @param other Source 4D vector.
   */
  Vector3f(const Vector4f& other);
 
  /**
   * @brief Constructs from explicit components.
   * @param nx X component.
   * @param ny Y component.
   * @param nz Z component.
   */
  Vector3f(const float& nx, const float& ny, const float& nz = 0);

  /**
   * @brief Destroys the vector.
   */
  ~Vector3f() = default;

  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  const float&
  operator()(const uint32& index) const;
    
  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  float&
  operator()(const uint32& index);
    
  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  const float
  operator[](const uint32& index) const;

  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  float&
  operator[](const uint32& index);

  /**
   * @brief + operator overload
   * @param v Vector to add.
   * @return a vector sum of *this and v
   */
  Vector3f
  operator+(const Vector3f& v) const;

  /**
   * @brief - operator overload
   * @param v Vector to subtract.
   * @return a vector difference of *this and v
   */
  Vector3f
  operator-(const Vector3f& v) const;

  /**
   * @brief * operator overload
   * @param v Vector to multiply component-wise.
   * @return a vector multiplication of *this times v
   */
  Vector3f
  operator*(const Vector3f& v) const;

  /**
   * @brief / operator overload
   * @param v Vector to divide component-wise.
   * @return a vector division of *this divided by v
   */
  Vector3f
  operator/(const Vector3f& v) const;

  /**
   * @brief + operator overload
   * @param plus the float to add to all vector
   * @return a vector sum of *this + plus
   */
  Vector3f
  operator+(const float& plus) const;

  /**
   * @brief - operator overload
   * @param minus the float to subtract to all vector
   * @return a vector difference of *this - minus
   */
  Vector3f
  operator-(const float& minus) const;

  /**
   * @brief * operator overload
   * @param times the float to multiply all vector
   * @return a vector multiplication of *this times times
   */
  Vector3f
  operator*(const float& times) const;

  /**
   * @brief / operator overload
   * @param under the float to divide all vector
   * @return a vector divided of *this divided by times
   */
  Vector3f
  operator/(const float& under) const;

  /**
   * @brief | operator overload for dot product
   * @param v Other vector.
   * @return dot product
   */
  float
  operator|(const Vector3f& v) const;

  /**
   * @brief ^ operator overload for cross product
   * @param v Other vector.
   * @return cross product
   */
  Vector3f
  operator^(const Vector3f& v) const;

  /**
   * @brief == operator overload
   * @param v Vector to compare.
   * @return true if this components are == to v components
   */
  bool
  operator==(const Vector3f& v) const;

  /**
   * @brief != operator overload
   * @param v Vector to compare.
   * @return true if this components are != to v components
   */
  bool
  operator!=(const Vector3f& v) const;

  /**
   * @brief < operator overload
   * @param v Vector to compare.
   * @return true if this components are < to v components
   */
  bool
  operator<(const Vector3f& v) const;

  /**
   * @brief > operator overload
   * @param v Vector to compare.
   * @return true if this components are > to v components
   */
  bool
  operator>(const Vector3f& v) const;

  /**
   * @brief <= operator overload
   * @param v Vector to compare.
   * @return true if this components are <= to v components
   */
  bool
  operator<=(const Vector3f& v) const;

  /**
   * @brief >= operator overload
   * @param v Vector to compare.
   * @return true if this components are >= to v components
   */
  bool
  operator>=(const Vector3f& v) const;

  /**
   * @brief - operator overload
   * @return the negative of the vector
   */
  Vector3f
  operator-() const;

  /**
   * @brief += operator overload
   * @param v Vector to add.
   * @return *this + v components
   */
  Vector3f&
  operator+=(const Vector3f& v);

  /**
   * @brief -= operator overload
   * @param v Vector to subtract.
   * @return *this - v components
   */
  Vector3f&
  operator-=(const Vector3f& v);

  /**
   * @brief *= operator overload
   * @param v Vector to multiply component-wise.
   * @return *this * v components
   */
  Vector3f&
  operator*=(const Vector3f& v);

  /**
   * @brief /= operator overload
   * @param v Vector to divide component-wise.
   * @return *this / v components
   */
  Vector3f&
  operator/=(const Vector3f& v);

  /**
   * @brief *= operator overload
   * @param scale Scalar multiplier.
   * @return *this * float
   */
  Vector3f&
  operator*=(const float& scale);

  /**
   * @brief /= operator overload
   * @param scale Scalar divisor.
   * @return *this / float
   */
  Vector3f&
  operator/=(const float& scale);

  /**
   * @brief Computes the dot product between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Dot product value.
   */
  static float
  dot(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Computes the cross product between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Cross product vector.
   */
  static Vector3f
  cross(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Computes scalar projection of b onto a.
   * @param a Projection axis.
   * @param b Vector to project.
   * @return Scalar projection value.
   */
  static float
  projection(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Computes squared distance between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Squared distance.
   */
  static float
  sqrDistance(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Computes distance between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Distance value.
   */
  static float
  distance(const Vector3f& a, const Vector3f& b);

  /**
   * @brief Sets vector components.
   * @param newX X component.
   * @param newY Y component.
   * @param newZ Z component.
   */
  void
  setValues(const float& newX, const float& newY, const float& newZ);

  /**
   * @brief Applies component-wise minimum with another vector.
   * @param v Vector to compare against.
   */
  void
  min(const Vector3f& v);

  /**
   * @brief Applies component-wise maximum with another vector.
   * @param v Vector to compare against.
   */
  void
  max(const Vector3f& v);

  /**
   * @brief Floors each vector component in place.
   */
  void
  floor();

  /**
   * @brief Ceils each vector component in place.
   */
  void
  ceiling();

  /**
   * @brief Rounds each vector component in place.
   */
  void
  round();

  /**
   * @brief Rounds each component using half-step behavior.
   */
  void
  roundHalf();

  /**
   * @brief Gets the highest component value.
   * @return Highest component value.
   */
  float
  getHighest() const;

  /**
   * @brief Gets the lowest component value.
   * @return Lowest component value.
   */
  float
  getLowest() const;

  /**
   * @brief Gets vector magnitude.
   * @return Magnitude value.
   */
  float
  magnitude() const;

  /**
   * @brief Gets squared vector magnitude.
   * @return Squared magnitude value.
   */
  float
  sqrMagnitude() const;

  /**
   * @brief Returns normalized copy of this vector.
   * @return Normalized vector.
   */
  Vector3f
  normalized() const;

  /**
   * @brief Returns normalized copy using fast-path variant.
   * @return Normalized vector.
   */
  Vector3f
  qNormalized() const;

  /**
   * @brief Normalizes this vector in place.
   */
  void
  normalize();

  /**
   * @brief Normalizes this vector in place using fast-path variant.
   */
  void
  qNormalize();

  /**
   * @brief Checks whether the vector is zero.
   * @return True if all components are zero.
   */
  bool
  isZero() const;

  /**
   * @brief Checks whether two vectors are nearly equal.
   * @param a First vector.
   * @param b Second vector.
   * @param error Allowed absolute tolerance.
   * @return True if vectors are nearly equal.
   */
  static bool
  areNearlySame(const Vector3f& a, const Vector3f& b, const float& error = Math::SMALLNUMBER);

  /**
   * @brief Returns vector as a printable string.
   * @return String representation of the vector.
   */
  String
  toString();

  /**
   * @brief Vector constant with all components set to 0.
   */
  static const Vector3f ZERO;

  /**
   * @brief Vector constant with all components set to 1.
   */
  static const Vector3f ONE;

  /**
   * @brief Unit vector along +X.
   */
  static const Vector3f ONEX;

  /**
   * @brief Unit vector along +Y.
   */
  static const Vector3f ONEY;

  /**
   * @brief Unit vector along +Z.
   */
  static const Vector3f ONEZ;

  /**
   * @brief Right direction vector.
   */
  static const Vector3f RIGHT;

  /**
   * @brief Up direction vector.
   */
  static const Vector3f UP;

  /**
   * @brief Forward direction vector.
   */
  static const Vector3f FRONT;

  /**
   * @brief Left direction vector.
   */
  static const Vector3f LEFT;

  /**
   * @brief Down direction vector.
   */
  static const Vector3f DOWN;

  /**
   * @brief Backward direction vector.
   */
  static const Vector3f BACK;

public:

  /**
   * @brief X value of the vector
   */
  float x;

  /**
   * @brief Y value of the vector
   */
  float y;

  /**
   * @brief Z value of the vector
   */
  float z;

};
}


