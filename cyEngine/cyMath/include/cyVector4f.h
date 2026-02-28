/**
 * @file cyVector4f.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Vector4f.
 */
#pragma once

#include "cyMathPrerequisites.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

// Forward declaration
class Vector2f;
class Vector2i;
class Vector3f;

/**
 * @class Vector4f
 * @brief a vector object with x y z w components
 */
class CY_MATH_EXPORT Vector4f {
public:

  /**
   * @brief Default constructor
   */
  Vector4f() = default;

  /**
   * @brief Constructs from a 2D integer vector with optional z and w.
   * @param other Source vector.
   * @param nz Z component.
   * @param nw W component.
   */
  Vector4f(const Vector2i& other, const float& nz = 0.0f, const float& nw = 0.0f);

  /**
   * @brief Constructs from a 2D float vector with optional z and w.
   * @param other Source vector.
   * @param nz Z component.
   * @param nw W component.
   */
  Vector4f(const Vector2f& other, const float& nz = 0.0f, const float& nw = 0.0f);
 
  /**
   * @brief Constructs from a 3D float vector with optional w.
   * @param other Source vector.
   * @param nw W component.
   */
  Vector4f(const Vector3f& other, const float& nw = 0.0f);

  /**
   * @brief Constructs a copy of another vector.
   * @param other Source vector.
   */
  Vector4f(const Vector4f& other);

  /**
   * @brief Constructs from explicit components.
   * @param nx X component.
   * @param ny Y component.
   * @param nz Z component.
   * @param nw W component.
   */
  Vector4f(const float& nx, const float& ny, const float& nz, const float& nw);

  /**
   * @brief Destroys the vector.
   */
  ~Vector4f() = default;

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
  Vector4f
  operator+(const Vector4f& v) const;

  /**
   * @brief - operator overload
   * @param v Vector to subtract.
   * @return a vector difference of *this and v
   */
  Vector4f
  operator-(const Vector4f& v) const;

  /**
   * @brief * operator overload
   * @param v Vector to multiply component-wise.
   * @return a vector multiplication of *this times v
   */
  Vector4f
  operator*(const Vector4f& v) const;

  /**
   * @brief / operator overload
   * @param v Vector to divide component-wise.
   * @return a vector division of *this divided by v
   */
  Vector4f
  operator/(const Vector4f& v) const;

  /**
   * @brief + operator overload
   * @param plus the float to add to all vector
   * @return a vector sum of *this + plus
   */
  Vector4f
  operator+(const float& plus) const;

  /**
   * @brief - operator overload
   * @param minus the float to subtract to all vector
   * @return a vector difference of *this - minus
   */
  Vector4f
  operator-(const float& minus) const;

  /**
   * @brief * operator overload
   * @param times the float to multiply all vector
   * @return a vector multiplication of *this times times
   */
  Vector4f
  operator*(const float& times) const;

  /**
   * @brief / operator overload
   * @param under the float to divide all vector
   * @return a vector divided of *this divided by times
   */
  Vector4f
  operator/(const float& under) const;

  /**
   * @brief | operator overload for dot product
   * @param v Other vector.
   * @return dot product
   */
  float
  operator|(const Vector4f& v) const;

  /**
   * @brief ^ operator overload for cross product
   * @param v Other vector.
   * @return cross product
   */
  Vector4f
  operator^(const Vector4f& v) const;

  /**
   * @brief == operator overload
   * @param v Vector to compare.
   * @return true if this components are == to v components
   */
  bool
  operator==(const Vector4f& v) const;

  /**
   * @brief != operator overload
   * @param v Vector to compare.
   * @return true if this components are != to v components
   */
  bool
  operator!=(const Vector4f& v) const;

  /**
   * @brief < operator overload
   * @param v Vector to compare.
   * @return true if this components are < to v components
   */
  bool
  operator<(const Vector4f& v) const;

  /**
   * @brief > operator overload
   * @param v Vector to compare.
   * @return true if this components are > to v components
   */
  bool
  operator>(const Vector4f& v) const;

  /**
   * @brief <= operator overload
   * @param v Vector to compare.
   * @return true if this components are <= to v components
   */
  bool
  operator<=(const Vector4f& v) const;

  /**
   * @brief >= operator overload
   * @param v Vector to compare.
   * @return true if this components are >= to v components
   */
  bool
  operator>=(const Vector4f& v) const;

  /**
   * @brief - operator overload
   * @return the negative of the vector
   */
  Vector4f
  operator-() const;

  /**
   * @brief += operator overload
   * @param v Vector to add.
   * @return *this + v components
   */
  Vector4f&
  operator+=(const Vector4f& v);

  /**
   * @brief -= operator overload
   * @param v Vector to subtract.
   * @return *this - v components
   */
  Vector4f&
  operator-=(const Vector4f& v);

  /**
   * @brief *= operator overload
   * @param v Vector to multiply component-wise.
   * @return *this * v components
   */
  Vector4f&
  operator*=(const Vector4f& v);

  /**
   * @brief /= operator overload
   * @param v Vector to divide component-wise.
   * @return *this / v components
   */
  Vector4f&
  operator/=(const Vector4f& v);

  /**
   * @brief *= operator overload
   * @param scale Scalar multiplier.
   * @return *this * float
   */
  Vector4f&
  operator*=(const float& scale);

  /**
   * @brief /= operator overload
   * @param scale Scalar divisor.
   * @return *this / float
   */
  Vector4f&
  operator/=(const float& scale);

  /**
   * @brief Computes the dot product between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Dot product value.
   */
  static float
  dot(const Vector4f& a, const Vector4f& b);

  /**
   * @brief Computes the cross product between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Cross product vector.
   */
  static Vector4f
  cross(const Vector4f& a, const Vector4f& b);

  /**
   * @brief Computes scalar projection of b onto a.
   * @param a Projection axis.
   * @param b Vector to project.
   * @return Scalar projection value.
   */
  static float
  projection(const Vector4f& a, const Vector4f& b);

  /**
   * @brief Computes squared distance between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Squared distance.
   */
  static float
  sqrDistance(const Vector4f& a, const Vector4f& b);

  /**
   * @brief Computes distance between two vectors.
   * @param a First vector.
   * @param b Second vector.
   * @return Distance value.
   */
  static float
  distance(const Vector4f& a, const Vector4f& b);

  /**
   * @brief Sets vector components.
   * @param newX X component.
   * @param newY Y component.
   * @param newZ Z component.
   * @param newW W component.
   */
  void
  setValues(const float& newX, const float& newY, const float& newZ, const float& newW);

  /**
   * @brief Applies component-wise minimum with another vector.
   * @param v Vector to compare against.
   */
  void
  min(const Vector4f& v);

  /**
   * @brief Applies component-wise maximum with another vector.
   * @param v Vector to compare against.
   */
  void
  max(const Vector4f& v);

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
  Vector4f
  normalized() const;

  /**
   * @brief Returns normalized copy using fast-path variant.
   * @return Normalized vector.
   */
  Vector4f
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
  areNearlySame(const Vector4f& a, const Vector4f& b, const float& error = Math::SMALLNUMBER);

  /**
   * @brief Returns vector as a printable string.
   * @return String representation of the vector.
   */
  String
  toString();

  /**
   * @brief Converts this vector to Vector3f.
   * @return Vector3f using xyz components.
   */
  Vector3f
  toVector3f() const;

  /**
   * @brief Vector constant with all components set to 0.
   */
  static const Vector4f ZERO;

  /**
   * @brief Vector constant with all components set to 1.
   */
  static const Vector4f ONE;

  /**
   * @brief Unit vector along +X.
   */
  static const Vector4f ONEX;

  /**
   * @brief Unit vector along +Y.
   */
  static const Vector4f ONEY;

  /**
   * @brief Unit vector along +Z.
   */
  static const Vector4f ONEZ;

  /**
   * @brief Unit vector along +W.
   */
  static const Vector4f ONEW;

  /**
   * @brief Right direction vector.
   */
  static const Vector4f RIGHT;

  /**
   * @brief Up direction vector.
   */
  static const Vector4f UP;

  /**
   * @brief Forward direction vector.
   */
  static const Vector4f FRONT;

  /**
   * @brief Left direction vector.
   */
  static const Vector4f LEFT;
  
  /**
   * @brief Down direction vector.
   */
  static const Vector4f DOWN;

  /**
   * @brief Backward direction vector.
   */
  static const Vector4f BACK;

public:

  /**
   * @brief x component
   */
  float x;

  /**
   * @brief y component
   */
  float y;

  /**
   * @brief z component
   */
  float z;

  /**
   * @brief w component
   */
  float w;
};

}


