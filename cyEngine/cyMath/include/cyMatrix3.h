#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector3f.h"

namespace CYLLENE_SDK {

// Forward class declaration
class Matrix4;
class Matrix2;
class Quaternion;

class CY_MATH_EXPORT Matrix3
{
 public:

  Matrix3() = default;

  ~Matrix3() = default;

  Matrix3(const float& value);

  Matrix3(const Matrix3& other);

  Matrix3(const Matrix4& other);

  Matrix3(const Matrix2& other);

  Matrix3(const float& m00, const float& m10, const float& m20,
          const float& m01, const float& m11, const float& m21,
          const float& m02, const float& m12, const float& m22);

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return the sum of *this and b
    *
    */
  Matrix3
  operator+(const Matrix3& b);

  /**
    * @brief - operator overload
    * @param b the other matrix to subtract
    * @return the difference of *this and b
    *
    */
  Matrix3
  operator-(const Matrix3& b);

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return the multiplication of *this and b
    *
    */
  Matrix3
  operator*(const Matrix3& b);

  /**
    * @brief += operator overload
    * @param b the other matrix to add
    * @return *this summed b
    *
    */
  Matrix3&
  operator+=(const Matrix3& b);

  /**
    * @brief -= operator overload
    * @param b the other matrix to subtract
    * @return *this minus b
    *
    */
  Matrix3&
  operator-=(const Matrix3& b);

  /**
    * @brief *= operator overload
    * @param b the other matrix to multiply
    * @return *this summed b
    *
    */
  Matrix3&
  operator*=(const Matrix3& b);

  /**
    * @brief += operator overload
    * @param value to add to the matrix
    * @return naumatrix equal to *this summed value
    *
    */
  Matrix3&
  operator+=(const float& value);

  /**
    * @brief -= operator overload
    * @param value to substract to the matrix
    * @return naumatrix equal to *this minus value
    *
    */
  Matrix3&
  operator-=(const float& value);

  /**
    * @brief *= operator overload
    * @param value to multiply the matrix
    * @return naumatrix equal to *this times value
    *
    */
  Matrix3&
  operator*=(const float& value);

  /**
    * @brief /= operator overload
    * @param value to divide to the matrix
    * @return naumatrix equal to *this folded value
    *
    */
  Matrix3&
  operator/=(const float& value);

  /**
    * @brief == operator overload
    * @param nauMatrix to compare
    * @return true if *this is equal to b
    *
    */
  bool
  operator==(const Matrix3& b);

  void
  identity();

  void
  zero();

  Matrix3
  transposed() const;

  void
  transpose();

  Matrix3
  cofactored() const;

  void
  cofactor();

  void
  setValues(const float& value);

  void
  setValues(const float& m00, const float& m10, const float& m20,
            const float& m01, const float& m11, const float& m21,
            const float& m02, const float& m12, const float& m22);

  Matrix3
  inversed();

  void
  inverse();

  const float
  determinant() const;

  void
  rotateX(const float& angle);

  void
  rotateY(const float& angle);

  void
  rotateZ(const float& angle);

  void
  rotate(const float& angle, const Vector3f& axis);

  void
  rotate(const float& angle, const float& x, const float& y, const float& z);

  void
  rotate(const Quaternion& rotation);

  void
  setRotation(const Vector3f& rotation);

  void
  setRotation(const Quaternion& rotation);

  void
  setScale(const Vector3f& newScale);

  void
  setScale(const float& newScale);

  void
  scale(const Vector3f& newScale);

  void
  scale(const float& newScale);

  Vector3f
  getForwardVector() const;
  
  Vector3f
  getRightVector() const;

  Vector3f
  getUpVector() const;

  Vector3f
  getEulerRotation() const;

  Quaternion
  getQuatRotation() const;
  
  Vector3f
  getScale() const;

  String
  toString();

  /**
    * ZERO filled Matrix
    */
  static const Matrix3 ZERO;

  /**
    * IDENTITY matrix
    */
  static const Matrix3 IDENTITY;

 public:

  union {
    /**
      * Row major based matrix struct
      */
    struct {
      float m00, m10, m20;
      float m01, m11, m21;
      float m02, m12, m22;
    }_m;
    float m[3][3];
    Vector3f columns[3];
    float fVec[9];
  };

};
}
