/**
 * @file cyVector2i.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Vector2i.
 */
#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

// Forward declarations
class Vector2f;
class Vector3f;
class Vector4f;

/**
 * @class Vector2i
 * @brief a Vector with int32 x y components 
 */
class CY_MATH_EXPORT Vector2i
{
public:

  Vector2i() = default;

  Vector2i(const int32& nx, const int32& ny = 0);

  Vector2i(const int32& values);

  Vector2i(const Vector2f& other);

  Vector2i(const Vector2i& other);

  Vector2i(const Vector3f& other);

  Vector2i(const Vector4f& other);

  ~Vector2i() = default;

  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   *
   */
  const int32&
  operator()(const int32& index) const;
    
  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   *
   */
  int32&
  operator()(const int32& index);
    
  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   *
   */
  const int32
  operator[](const int32& index) const;

  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   *
   */
  int32&
  operator[](const int32& index);

  /**
   * @brief + operator overload
   * @param b the other matrix to add
   * @return a vector sum of *this and v
   *
   */
  Vector2i
  operator+(const Vector2i& v) const;

  /**
   * @brief - operator overload
   * @param b the other matrix to substract
   * @return a vector difference of *this and v
   *
   */
  Vector2i
  operator-(const Vector2i& v) const;

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return a vector multiplication of *this times v
   *
   */
  Vector2i
  operator*(const Vector2i& v) const;

  /**
   * @brief / operator overload
   * @param b the other matrix to divide
   * @return a vector division of *this divided by v
   *
   */
  Vector2i
  operator/(const Vector2i& v) const;

  /**
   * @brief + operator overload
   * @param plus the float to add to all vector
   * @return a vector sum of *this + plus
   *
   */
  Vector2i
  operator+(const int32& plus) const;

  /**
   * @brief - operator overload
   * @param minus the float to subtract to all vector
   * @return a vector difference of *this - minus
   *
   */
  Vector2i
  operator-(const int32& minus) const;

  /**
   * @brief * operator overload
   * @param times the float to multiply all vector
   * @return a vector multiplication of *this times times
   *
   */
  Vector2i
  operator*(const int32& times) const;

  /**
   * @brief / operator overload
   * @param under the float to divide all vector
   * @return a vector divided of *this divided by times
   *
   */
  Vector2i
  operator/(const int32& under) const;

  /**
   * @brief | operator overload for dot product
   * @param v Other vector.
   * @return dot product
   *
   */
  int32
  operator|(const Vector2i& v) const;

  /**
   * @brief ^ operator overload for cross product
   * @param v Other vector.
   * @return cross product
   *
   */
  int32
  operator^(const Vector2i& v) const;

  /**
   * @brief == operator overload
   * @param v Vector to compare.
   * @return true if this components are == to v components
   *
   */
  bool
  operator==(const Vector2i& v) const;

  /**
   * @brief != operator overload
   * @param v Vector to compare.
   * @return true if this components are != to v components
   *
   */
  bool
  operator!=(const Vector2i& v) const;

  /**
   * @brief < operator overload
   * @param v Vector to compare.
   * @return true if this components are < to v components
   *
   */
  bool
  operator<(const Vector2i& v) const;

  /**
   * @brief > operator overload
   * @param v Vector to compare.
   * @return true if this components are > to v components
   *
   */
  bool
  operator>(const Vector2i& v) const;

  /**
   * @brief <= operator overload
   * @param v Vector to compare.
   * @return true if this components are <= to v components
   *
   */
  bool
  operator<=(const Vector2i& v) const;

  /**
   * @brief >= operator overload
   * @param v Vector to compare.
   * @return true if this components are >= to v components
   *
   */
  bool
  operator>=(const Vector2i& v) const;

  /**
   * @brief - operator overload
   * @return the negative of the vector
   *
   */
  Vector2i
  operator-() const;

  /**
   * @brief += operator overload
   * @param v Vector to add.
   * @return *this + v components
   *
   */
  Vector2i&
  operator+=(const Vector2i& v);

  /**
   * @brief -= operator overload
   * @param v Vector to subtract.
   * @return *this - v components
   *
   */
  Vector2i&
  operator-=(const Vector2i& v);

  /**
   * @brief *= operator overload
   * @param v Vector to multiply component-wise.
   * @return *this * v components
   *
   */
  Vector2i&
  operator*=(const Vector2i& v);

  /**
   * @brief /= operator overload
   * @param v Vector to divide component-wise.
   * @return *this / v components
   *
   */
  Vector2i&
  operator/=(const Vector2i& v);

  /**
   * @brief *= operator overload
   * @param scale Scalar multiplier.
   * @return *this * float
   *
   */
  Vector2i&
  operator*=(const int32& scale);

  /**
   * @brief /= operator overload
   * @param scale Scalar divisor.
   * @return *this / float
   *
   */
  Vector2i&
  operator/=(const int32& scale);

  /**
   * @brief the dot product
   * @param a First vector.
   * @param b Second vector.
   * @return the dot product between a and b
   *
   */
  static int32
  dot(const Vector2i& a, const Vector2i& b);

  /**
   * @brief the cross product
   * @param a First vector.
   * @param b Second vector.
   * @return the cross product between a and b
   *
   */
  static int32
  cross(const Vector2i& a, const Vector2i& b);

  /**
   * @brief the scale of b over a
   * @param a Projection axis.
   * @param b Vector to project.
   * @return scale of b in a
   *
   */
  static int32
  projection(const Vector2i& a, const Vector2i& b);

  /**
   * @brief the square distance
   * @param a First vector.
   * @param b Second vector.
   * @return the square distance bewteen the two points
   *
   */
  static int32
  sqrDistance(const Vector2i& a, const Vector2i& b);

  /**
   * @brief the distance between two points in 2d space
   * @param a First vector.
   * @param b Second vector.
   * @return the distance bewteen the two points
   *
   */
  static int32
  distance(const Vector2i& a, const Vector2i& b);

  /**
   * @brief sets the value of a nauVector2
   * @param newX New x component.
   * @param newY New y component.
   *
   */
  void
  setValues(const int32& newX, const int32& newY);

  /**
   * @brief gets the min between two vectors
   * @param v Other vector to compare.
   *
   */
  void
  min(const Vector2i& v);

  /**
   * @brief gets the max between two vectors
   * @param v Other vector to compare.
   *
   */
  void
  max(const Vector2i& v);
    
  /**
   * @brief gets the highest value of the vector
   * @return Highest component value.
   *
   */
  int32
  getHighest() const;

  /**
   * @brief gets the least value of the vector
   * @return Lowest component value.
   *
   */
  int32
  getLowest() const;

  /**
   * @brief gets the magnitude value of the vector
   * @return Vector magnitude.
   *
   */
  int32
  magnitude() const;

  /**
   * @brief gets the squared magnitude value of the vector
   * @return Squared vector magnitude.
   *
   */
  int32
  sqrMagnitude() const;

  /**
   * @brief gets the normalized version of the vector
   * @return the normalized vector
   *
   */
  Vector2i 
  normalized() const;

  /**
   * @brief gets the QUICK normalized version of the vector with the Carmack Equation
   * Use this if you don't have problems having a small error difference from the real value
   * @return the QUICK normalized vector
   *
   */
  Vector2i
  qNormalized() const;

  /**
   * @brief normalizes the vector
   *
   */
  void
  normalize();

  /**
   * @brief QUICK normalizes the vector using the Carmack Equation
   * Use this if you don't have problems having a small error difference from the real value
   *
   */
  void
  qNormalize();

  /**
   * @brief Checks if the vector is empty (0s)
   * @return true if it is empty
   *
   */
  bool
  isZero() const;

  /**
   * @brief Checks if one vector is the same as another vector
   * @param a First vector to compare.
   * @param b Second vector to compare.
   * @return true if based on the error is the same
   *
   */
  static bool
  areSame(const Vector2i& a, const Vector2i& b);

  /**
   * @brief Checks if one vector is near the same as another vector based in an error
   * @param a First vector to compare.
   * @param b Second vector to compare.
   * @param error Tolerance threshold for comparison.
   * @return true if based on the error is the same
   *
   */
  static bool
  areNearlySame(const Vector2i& a, const Vector2i& b, const int32& error = 1);

  /**
   * @brief Returns the vector as a printable string
   * @return String representation of this vector.
   *
   */
  String
  toString();

  /**
   * static const
   */

  public:

  /**
   * @brief Vector constant with all components set to 0.
   */
  static const Vector2i ZERO;

  /**
   * @brief Vector constant with all components set to 1.
   */
  static const Vector2i ONE;

  /**
   * @brief Unit vector along +X.
   */
  static const Vector2i ONEX;

  /**
   * @brief Unit vector along +Y.
   */
  static const Vector2i ONEY;

  /**
   * @brief Up direction vector.
   */
  static const Vector2i UP;

  /**
   * @brief Right direction vector.
   */
  static const Vector2i RIGHT;

  /**
   * @brief Down direction vector.
   */
  static const Vector2i DOWN;

  /**
   * @brief Left direction vector.
   */
  static const Vector2i LEFT;

  /**
   * Member declaration
   */

  public:

  /**
   * @brief X component.
   */
  int32 x;

  /**
   * @brief Y component.
   */
  int32 y;

};

}


