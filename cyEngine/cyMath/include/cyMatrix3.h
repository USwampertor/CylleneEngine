/**
 * @file cyMatrix3.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Matrix3.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector3f.h"

namespace CYLLENE_SDK {

// Forward class declaration
struct Euler;

class Matrix4;
class Matrix2;
class Quaternion;

/**
 * @class Matrix3
 * @brief a column major matrix of 3x3
 */
class CY_MATH_EXPORT Matrix3
{
 public:
  /**
   * @brief Constructs an uninitialized 3x3 matrix.
   */
  Matrix3() = default;

  /**
   * @brief Destroys the matrix.
   */
  ~Matrix3() = default;

  /**
   * @brief Constructs a matrix initialized from a scalar.
   * @param value Scalar used by the constructor initialization path.
   */
  Matrix3(const float& value);

  /**
   * @brief Constructs a matrix as a copy of another matrix.
   * @param other Source matrix.
   */
  Matrix3(const Matrix3& other);

  /**
   * @brief Constructs a 3x3 matrix from a 4x4 matrix.
   * @param other Source 4x4 matrix.
   */
  Matrix3(const Matrix4& other);

  /**
   * @brief Constructs a 3x3 matrix from a 2x2 matrix.
   * @param other Source 2x2 matrix.
   */
  Matrix3(const Matrix2& other);

  /**
   * @brief Constructs a matrix from 9 explicit components.
   * @param m00 First column, first row component.
   * @param m10 Second column, first row component.
   * @param m20 Third column, first row component.
   * @param m01 First column, second row component.
   * @param m11 Second column, second row component.
   * @param m21 Third column, second row component.
   * @param m02 First column, third row component.
   * @param m12 Second column, third row component.
   * @param m22 Third column, third row component.
   */
  Matrix3(const float& m00, const float& m10, const float& m20,
          const float& m01, const float& m11, const float& m21,
          const float& m02, const float& m12, const float& m22);

  /**
   * @brief + operator overload
   * @param b the other matrix to add
   * @return the sum of *this and b
   */
  Matrix3
  operator+(const Matrix3& b);

  /**
   * @brief - operator overload
   * @param b the other matrix to subtract
   * @return the difference of *this and b
   */
  Matrix3
  operator-(const Matrix3& b);

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return the multiplication of *this and b
   */
  Matrix3
  operator*(const Matrix3& b);

  /**
   * @brief += operator overload
   * @param b the other matrix to add
   * @return *this summed b
   */
  Matrix3&
  operator+=(const Matrix3& b);

  /**
   * @brief -= operator overload
   * @param b the other matrix to subtract
   * @return *this minus b
   */
  Matrix3&
  operator-=(const Matrix3& b);

  /**
   * @brief *= operator overload
   * @param b the other matrix to multiply
   * @return *this summed b
   */
  Matrix3&
  operator*=(const Matrix3& b);

  /**
   * @brief += operator overload
   * @param value to add to the matrix
   * @return naumatrix equal to *this summed value
   */
  Matrix3&
  operator+=(const float& value);

  /**
   * @brief -= operator overload
   * @param value to substract to the matrix
   * @return naumatrix equal to *this minus value
   */
  Matrix3&
  operator-=(const float& value);

  /**
   * @brief *= operator overload
   * @param value to multiply the matrix
   * @return naumatrix equal to *this times value
   */
  Matrix3&
  operator*=(const float& value);

  /**
   * @brief /= operator overload
   * @param value to divide to the matrix
   * @return naumatrix equal to *this folded value
   */
  Matrix3&
  operator/=(const float& value);

  /**
   * @brief == operator overload
   * @param b The matrix to compare against.
   * @return true if *this is equal to b
   */
  bool
  operator==(const Matrix3& b);

  /**
   * @brief Sets this matrix to identity.
   */
  void
  identity();

  /**
   * @brief Sets all matrix components to zero.
   */
  void
  zero();

  /**
   * @brief Returns the transpose of this matrix.
   * @return A transposed copy of this matrix.
   */
  Matrix3
  transposed() const;

  /**
   * @brief Transposes this matrix in place.
   */
  void
  transpose();

  /**
   * @brief Returns the cofactor matrix.
   * @return Cofactor matrix of this matrix.
   */
  Matrix3
  cofactored() const;

  /**
   * @brief Replaces this matrix with its cofactor matrix.
   */
  void
  cofactor();

  /**
   * @brief Sets all matrix entries from a single scalar value.
   * @param value Scalar value to apply.
   */
  void
  setValues(const float& value);

  /**
   * @brief Sets all 9 matrix values explicitly.
   * @param m00 First column, first row.
   * @param m10 Second column, first row.
   * @param m20 Third column, first row.
   * @param m01 First column, second row.
   * @param m11 Second column, second row.
   * @param m21 Third column, second row.
   * @param m02 First column, third row.
   * @param m12 Second column, third row.
   * @param m22 Third column, third row.
   */
  void
  setValues(const float& m00, const float& m10, const float& m20,
            const float& m01, const float& m11, const float& m21,
            const float& m02, const float& m12, const float& m22);

  /**
   * @brief Returns the inverse of this matrix.
   * @return Inverse matrix.
   */
  Matrix3
  inversed();

  /**
   * @brief Inverts this matrix in place.
   */
  void
  inverse();

  /**
   * @brief Computes the matrix determinant.
   * @return Determinant value.
   */
  const float
  determinant() const;

  /**
   * @brief Sets rotation component from Euler angles.
   * @param rotation Euler rotation.
   */
  void
  setRotation(const Vector3f& rotation);

  /**
   * @brief Sets rotation component from quaternion.
   * @param rotation Quaternion rotation.
   */
  void
  setRotation(const Quaternion& rotation);

  /**
   * @brief Sets non-uniform scale.
   * @param newScale Scale vector.
   */
  void
  setScale(const Vector3f& newScale);

  /**
   * @brief Sets uniform scale.
   * @param newScale Uniform scale.
   */
  void
  setScale(const float& newScale);

  /**
   * @brief Applies a rotation around X axis.
   * @param angle Rotation angle in radians.
   */
  void
  rotateX(const float& angle);

  /**
   * @brief Applies a rotation around Y axis.
   * @param angle Rotation angle in radians.
   */
  void
  rotateY(const float& angle);

  /**
   * @brief Applies a rotation around Z axis.
   * @param angle Rotation angle in radians.
   */
  void
  rotateZ(const float& angle);

  /**
   * @brief Applies an axis-angle rotation.
   * @param angle Rotation angle in radians.
   * @param axis Rotation axis.
   */
  void
  rotate(const float& angle, const Vector3f& axis);

  /**
   * @brief Applies an axis-angle rotation.
   * @param angle Rotation angle in radians.
   * @param x Rotation axis X component.
   * @param y Rotation axis Y component.
   * @param z Rotation axis Z component.
   */
  void
  rotate(const float& angle, const float& x, const float& y, const float& z);

  /**
   * @brief Applies a quaternion rotation.
   * @param rotation Quaternion rotation.
   */
  void
  rotate(const Quaternion& rotation);

  /**
   * @brief Applies non-uniform scaling.
   * @param newScale Scale vector.
   */
  void
  scale(const Vector3f& newScale);

  /**
   * @brief Applies non-uniform scaling.
   * @param x X scale.
   * @param y Y scale.
   * @param z Z scale.
   */
  void
  scale(const float& x, const float& y, const float& z);

  /**
   * @brief Applies uniform scaling.
   * @param newScale Uniform scale.
   */
  void
  scale(const float& newScale);

  /**
   * @brief Gets forward basis vector.
   * @return Forward direction.
   */
  Vector3f
  getForwardVector() const;
  
  /**
   * @brief Gets right basis vector.
   * @return Right direction.
   */
  Vector3f
  getRightVector() const;

  /**
   * @brief Gets up basis vector.
   * @return Up direction.
   */
  Vector3f
  getUpVector() const;

  /**
   * @brief Gets Euler rotation extracted from this matrix.
   * @return Euler rotation.
   */
  Euler
  getEulerRotation() const;

  /**
   * @brief Gets quaternion rotation extracted from this matrix.
   * @return Quaternion rotation.
   */
  Quaternion
  getQuatRotation() const;
  
  /**
   * @brief Gets scale extracted from this matrix.
   * @return Scale vector.
   */
  Vector3f
  getScale() const;

  /**
   * @brief Gets rotation-only matrix.
   * @return Rotation matrix.
   */
  Matrix3
  getRotationMatrix() const;

  /**
   * @brief Gets scale-only matrix.
   * @return Scale matrix.
   */
  Matrix3
  getScaleMatrix() const;

  /**
   * @brief Converts matrix to a string representation.
   * @return Formatted matrix string.
   */
  String
  toString();

  /**
   * @brief ZERO filled Matrix
   */
  static const Matrix3 ZERO;

  /**
   * @brief IDENTITY matrix
   */
  static const Matrix3 IDENTITY;

 public:

  union {
    /**
     * @brief Row major based matrix struct
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

