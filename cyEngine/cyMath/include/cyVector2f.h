/**
 * @file cyVector2f.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Vector2f.
 */
#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

// Forward declaration
class Vector2i;
class Vector3f;
class Vector4f;

/**
 * @class Vector2f
 * @brief 2D vector with float x and y components.
 */
class CY_MATH_EXPORT Vector2f
{
public:

  Vector2f() = default;

  Vector2f(const float& nx, const float& ny = 0);

  Vector2f(const float& values);

  Vector2f(const Vector2i& other);

  Vector2f(const Vector2f& other);

  Vector2f(const Vector3f& other);

  Vector2f(const Vector4f& other);

  ~Vector2f() = default;

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
   * @param b the other matrix to add
   * @return a vector sum of *this and v
   */
  Vector2f
  operator+(const Vector2f& v) const;

  /**
   * @brief - operator overload
   * @param b the other matrix to substract
   * @return a vector difference of *this and v
   */
  Vector2f
  operator-(const Vector2f& v) const;

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return a vector multiplication of *this times v
   */
  Vector2f
  operator*(const Vector2f& v) const;

  /**
   * @brief / operator overload
   * @param b the other matrix to divide
   * @return a vector division of *this divided by v
   */
  Vector2f
  operator/(const Vector2f& v) const;

  /**
   * @brief + operator overload
   * @param plus the float to add to all vector
   * @return a vector sum of *this + plus
   */
  Vector2f
  operator+(const float& plus) const;

  /**
   * @brief - operator overload
   * @param minus the float to subtract to all vector
   * @return a vector difference of *this - minus
   */
  Vector2f
  operator-(const float& minus) const;

  /**
   * @brief * operator overload
   * @param times the float to multiply all vector
   * @return a vector multiplication of *this times times
   */
  Vector2f
  operator*(const float& times) const;

  /**
   * @brief / operator overload
   * @param under the float to divide all vector
   * @return a vector divided of *this divided by times
   */
  Vector2f
  operator/(const float& under) const;

  /**
   * @brief | operator overload for dot product
   * @param v Other vector.
   * @return dot product
   */
  float
  operator|(const Vector2f& v) const;

  /**
   * @brief ^ operator overload for cross product
   * @param v Other vector.
   * @return cross product
   */
  float
  operator^(const Vector2f& v) const;

  /**
   * @brief == operator overload
   * @param v Vector to compare.
   * @return true if this components are == to v components
   */
  bool
  operator==(const Vector2f& v) const;

  /**
   * @brief != operator overload
   * @param v Vector to compare.
   * @return true if this components are != to v components
   */
  bool
  operator!=(const Vector2f& v) const;

  /**
   * @brief < operator overload
   * @param v Vector to compare.
   * @return true if this components are < to v components
   */
  bool
  operator<(const Vector2f& v) const;

  /**
   * @brief > operator overload
   * @param v Vector to compare.
   * @return true if this components are > to v components
   */
  bool
  operator>(const Vector2f& v) const;

  /**
   * @brief <= operator overload
   * @param v Vector to compare.
   * @return true if this components are <= to v components
   */
  bool
  operator<=(const Vector2f& v) const;

  /**
   * @brief >= operator overload
   * @param v Vector to compare.
   * @return true if this components are >= to v components
   */
  bool
  operator>=(const Vector2f& v) const;

  /**
   * @brief - operator overload
   * @return the negative of the vector
   */
  Vector2f
  operator-() const;

  /**
   * @brief += operator overload
   * @param v Vector to add.
   * @return *this + v components
   */
  Vector2f&
  operator+=(const Vector2f& v);

  /**
   * @brief -= operator overload
   * @param v Vector to subtract.
   * @return *this - v components
   */
  Vector2f&
  operator-=(const Vector2f& v);

  /**
   * @brief *= operator overload
   * @param v Vector to multiply component-wise.
   * @return *this * v components
   */
  Vector2f&
  operator*=(const Vector2f& v);

  /**
   * @brief /= operator overload
   * @param v Vector to divide component-wise.
   * @return *this / v components
   */
  Vector2f&
  operator/=(const Vector2f& v);

  /**
   * @brief *= operator overload
   * @param scale Scalar multiplier.
   * @return *this * float
   */
  Vector2f&
  operator*=(const float& scale);

  /**
   * @brief /= operator overload
   * @param scale Scalar divisor.
   * @return *this / float
   */
  Vector2f&
  operator/=(const float& scale);

  /**
   * @brief the dot product
   * @param a First vector.
   * @param b Second vector.
   * @return the dot product between a and b
   */
  static float
  dot(const Vector2f& a, const Vector2f& b);

  /**
   * @brief the cross product
   * @param a First vector.
   * @param b Second vector.
   * @return the cross product between a and b
   */
  static float
  cross(const Vector2f& a, const Vector2f& b);

  /**
   * @brief the scale of b over a
   * @param a Projection axis.
   * @param b Vector to project.
   * @return scale of b in a
   */
  static float
  projection(const Vector2f& a, const Vector2f& b);

  /**
   * @brief the square distance
   * @param a First vector.
   * @param b Second vector.
   * @return the square distance bewteen the two points
   */
  static float
  sqrDistance(const Vector2f& a, const Vector2f& b);

  /**
   * @brief the distance between two points in 2d space
   * @param a First vector.
   * @param b Second vector.
   * @return the distance bewteen the two points
   */
  static float
  distance(const Vector2f& a, const Vector2f& b);

  /**
   * @brief sets the value of a nauVector2
   * @param nx New x component.
   * @param ny New y component.
   */
  void
  setValues(const float& nx, const float& ny);

  /**
   * @brief gets the min between two vectors
   * @param v Other vector to compare.
   */
  void
  min(const Vector2f& v);

  /**
   * @brief gets the max between two vectors
   * @param v Other vector to compare.
   */
  void
  max(const Vector2f& v);

  /**
   * @brief gets the highest value of the vector
   * @return Highest component value.
   */
  float
  getHighest() const;

  /**
   * @brief gets the least value of the vector
   * @return Lowest component value.
   */
  float
  getLowest() const;

  /**
   * @brief gets the magnitude value of the vector
   * @return Vector magnitude.
   */
  float
  magnitude() const;

  /**
   * @brief gets the squared magnitude value of the vector
   * @return Squared vector magnitude.
   */
  float
  sqrMagnitude() const;

  /**
   * @brief gets the normalized version of the vector
   * @return the normalized vector
   */
  Vector2f
  normalized() const;

  /**
   * @brief gets the QUICK normalized version of the vector using the Carmack Equation
   * Use this if you don't have problems having a small error difference from the real value
   * @return the normalized vector
   */
  Vector2f
  qNormalized() const;

  /**
   * @brief normalizes the vector
   */
  void
  normalize();

  /**
   * @brief QUICK normalizes the vector using the Carmack Equation
   *        Use this if you don't have problems having a small error difference from the real value
   */
  void
  qNormalize();

  /**
   * @brief Checks if the vector is empty (0s)
   * @return true if it is empty
   */
  bool
  isZero() const;

  /**
   * @brief Checks if one vector is the same as another vector
   * @param a First vector to compare.
   * @param b Second vector to compare.
   * @return true if based on the error is the same
   */
  static bool
  isSame(const Vector2f& a, const Vector2f& b);

  /**
   * @brief Checks if one vector is near the same as another vector based in an error
   * @param a First vector to compare.
   * @param b Second vector to compare.
   * @param error Tolerance threshold for comparison.
   * @return true if based on the error is the same
   */
  static bool
  isNearlySame(const Vector2f& a, const Vector2f& b, const float& error = Math::SMALLNUMBER);

  Vector2f
  toNearestOctant() const;


  /**
   * @brief Returns the vector as a printable string
   * @return String representation of this vector.
   */
  String
  toString();

  /**
   * static const
   */

public:

  /**
   * @brief Vector2 with all values 0
   */
  static const Vector2f ZERO;

  /**
   * @brief Vector2 with all values 1
   */
  static const Vector2f ONE;

  /**
   * @brief Vector2 with x value 1
   */
  static const Vector2f ONEX;

  /**
   * @brief Vector2 with y value 1
   */
  static const Vector2f ONEY;

  /**
   * @brief Vector2 with y value 1. This is our right in our world
   */
  static const Vector2f UP;

  /**
   * @brief Vector2 with x value 1. This is our up in our world
   */
  static const Vector2f RIGHT;

  /**
   * @brief Vector2 with y value -1. This is our down in our world
   */
  static const Vector2f DOWN;

  /**
   * @brief Vector2 with x value -1. This is our left in our world
   */
  static const Vector2f LEFT;

  /**
   * Member declaration
   */

public:

  /**
   * @brief x component
   */
  float x;

  /**
   * @brief y component
   */
  float y;

};

}


