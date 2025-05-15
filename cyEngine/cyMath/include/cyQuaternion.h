/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyQuaternion.h
 * @author Marco "Swampy" Millan
 * @date 9/1/2021
 * @brief Ah yes, quaternions my old nemesis, now I can call you
 *        my old friend. Quaternions are special vectors used for
 *        rotation regardless of orientation
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyMathPrerequisites.h"

#include "cyEulerHelpers.h"

namespace CYLLENE_SDK {

class Vector3f;
class Vector4f;
class Matrix3;
class Matrix4;
class Quaternion;

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Euler ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct CY_MATH_EXPORT Euler {
public:

  Euler() = default;

  ~Euler() = default;

  Euler(const int32& norder);

  Euler(const float& nx, const float& ny, const float& nz);

  Euler(const float& nx, const float& ny, const float& nz, const int32& norder);

  Euler(const Vector3f& vector);

  Euler(const Vector4f& vector);

  Euler(const Matrix3& rotationMatrix, const int32& order = EulOrdXYZs);

  Euler(const Matrix4& rotationMatrix, const int32& order = EulOrdXYZs);

  Euler(const Quaternion& rotationQuat, const int32& order = EulOrdXYZs);

  Matrix4
  getMatrix4Rotation() const;

  Matrix3
  getMatrix3Rotation() const;

  Quaternion
  getQuatRotation() const;

  String
  toString();

public:
  float x;
  float y;
  float z;

  int32 order;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Quaternion //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CY_MATH_EXPORT Quaternion {
    
public:
    
  // Default constructor
  Quaternion() = default;

  ~Quaternion() = default;

  /**
    * @brief Constructs without any transformation, passing values as is
    * @param float nx
    * @param float ny
    * @param float nz
    *
    */
  Quaternion(const float& nx, const float& ny, const float& nz, const float& nw);

  /**
    * @brief Constructs without any transformation, passing values as is
    * @param Vector3f vector to add
    * This Constructor fills the w value with 0
    *
    */
  Quaternion(const Vector3f& other);

  /**
    * @brief Constructs without any transformation, passing values as is
    * @param Vector4f The x y z w components
    *
    */
  Quaternion(const Vector4f other);

  Quaternion(const Euler& euler);

  Quaternion(const Matrix3& matrix);

  Quaternion(const Matrix4& matrix);

  /**
    * @brief [] operator overload, will return the value at a given position
    * @param index of the object we wanna get
    * @return value at given index
    *
    */
  float&
  operator[](uint32 index);

  /**
    * @brief const [] operator overload, will return the value at a given position
    * @param index of the object we wanna get
    * @return value at given index
    *
    */
  float
  operator[](uint32 index) const;

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return a vector sum of *this and v
    *
    */
  Quaternion
  operator+(const Quaternion& other) const;
  
  /**
    * @brief - operator overload
    * @param b the other matrix to subtract
    * @return a vector difference of *this and v
    *
    */
  Quaternion
  operator-(const Quaternion& other) const;
  
  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return a vector multiplication of *this times v
    *
    */
  Quaternion
  operator*(const Quaternion& other) const;
  
  /**
    * @brief / operator overload with quaternions
    * @param the quaternion to divide
    * @return a quaternion divided by another quaternion
    *
    */
  Quaternion
  operator/(const Quaternion& other) const;

  /**
    * @brief / operator overload
    * @param the scale to divide
    * @return a quaternion divided by scale
    *
    */
  Quaternion
  operator/(const float& scale) const;

  /**
    * @brief / operator overload
    * @param float the scale to multiply
    * @return a vector scaled by scale
    *
    */
  Quaternion
  operator*(const float& scale) const;
  
  /**
    * @brief Operator = overload
    * @param const Quaternion& other quaternion to evaluate
    * @return Quaternion this
    *
    */
  Quaternion&
  operator=(const Quaternion& other);

  /**
    * @brief == operator overload
    * @param the other vector to compare
    * @return true if this is == to v components
    *
    */
  bool
  operator==(const Quaternion& other);

  /**
    * @brief != operator overload
    * @param the other vector to compare
    * @return true if this is != to v components
    *
    */
  bool
  operator!=(const Quaternion& other);

  /**
    * @brief < operator overload
    * @param the other vector to compare
    * @return true if this components are < to v components
    *
    */
  bool
  operator<(const Quaternion& other);

  /**
    * @brief > operator overload
    * @param the other vector to compare
    * @return true if this components are > to v components
    *
    */
  bool
  operator>(const Quaternion& other);

  /**
    * @brief <= operator overload
    * @param the other vector to compare
    * @return true if this components are <= to v components
    *
    */
  bool
  operator<=(const Quaternion& other);

  /**
    * @brief >= operator overload
    * @param the other vector to compare
    * @return true if this components are >= to v components
    *
    */
  bool
  operator>=(const Quaternion& other);
  
  /**
    * @brief negates the whole quaternion
    * @param 
    * @return 
    *
    */
  Quaternion
  operator-() const;

  /**
    * @brief += operator overload
    * @param the other vector to add
    * @return *this + v components
    *
    */
  Quaternion&
  operator+=(const Quaternion& other);
  
  /**
    * @brief -= operator overload
    * @param the other vector to subtract
    * @return *this - v components
    *
    */
  Quaternion&
  operator-=(const Quaternion& other);
  
  /**
    * @brief *= operator overload
    * @param the other vector to multiply
    * @return *this * v components
    *
    */
  Quaternion&
  operator*=(const Quaternion& other);

  /**
    * @brief /= operator overload with quaternions
    * @param the other quaternion to divide
    * @return *this / q components
    *
    */
  Quaternion&
  operator/=(const Quaternion& other);

  /**
    * @brief *= operator overload
    * @param the float to scale the quaternion
    * @return *this * v components
    *
    */
  Quaternion&
  operator*=(float scale);
  
  /**
    * @brief /= operator overload
    * @param the float to scale the quaternion
    * @return *this / v components
    *
    */
  Quaternion&
  operator/=(float scale);
  
  /**
    * @brief | operator overload for dot product
    * @param the other vector
    * @return dot product
    *
    */
  float
  operator|(const Quaternion& v) const;

  static const Vector3f
  transform(const Vector3f& vector, const Quaternion& quaternion);

  static const Quaternion
  slerp(const Quaternion& q1, const Quaternion& q2, float t);

  void
  setRotation(const Euler& euler);

  void
  setRotation(const Matrix3& m);

  void
  setRotation(const Matrix4& m);

  void
  setValues(const float& nx, const float& ny, const float& nz, const float& nw);

  void 
  setValues(const Vector3f& vector, const float& scalar);

  const Vector3f
  getVectorPart() const;

  const float&
  getScalarPart() const;

  Euler
  getEulerRotation(const int32& order = EulOrdXYZs) const;

  const Matrix3
  getMatrix3Rotation() const;

  const Matrix4
  getMatrix4Rotation() const;

  float 
  norm() const;

  float
  magnitude() const;

  Quaternion
  scaled(const float& s) const;

  void 
  scale(const float& s);

  Quaternion
  normalized() const;

  void
  normalize();

  Quaternion
  conjugated() const;

  void
  conjugate();

  Quaternion
  inversed() const;

  void 
  inverse();

  Quaternion
  unitQuaternion() const;

  void
  unit();

  Vector3f
  rotate(const Vector3f& v) const;

  bool
  isPure() const;

  bool
  isReal() const;

  Vector3f
  toVector3() const;

  Vector4f 
  toVector4() const;

  String
  toString() const;

  static const Quaternion
  IDENTITY;

public:

  float x;
  float y;
  float z;
  float w;

};

}
