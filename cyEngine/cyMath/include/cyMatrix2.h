/**
 * @file cyMatrix2.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Matrix2.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector2f.h"

namespace CYLLENE_SDK {

// Forward definition
class Matrix3;
class Matrix4;

/**
 * @class Matrix2
 * @brief 2x2 matrix type stored in row-major order.
 */
class CY_MATH_EXPORT Matrix2
{
 public:

  Matrix2() = default;

  ~Matrix2() = default;

  Matrix2(const float& value);

  Matrix2(const Matrix2& other);

  Matrix2(const float& m00, const float& m10,
          const float& m01, const float& m11);

  /**
   * @brief + operator overload
   * @param b the other matrix to add
   * @return the sum of *this and b
   *
   */
  Matrix2
  operator+(const Matrix2& b);

  /**
   * @brief - operator overload
   * @param b the other matrix to subtract
   * @return the difference of *this and b
   *
   */
  Matrix2
  operator-(const Matrix2& b);

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return the multiplication of *this and b
   *
   */
  Matrix2
  operator*(const Matrix2& b);

  /**
   * @brief += operator overload
   * @param b the other matrix to add
   * @return *this summed b
   *
   */
  Matrix2&
  operator+=(const Matrix2& b);

  /**
   * @brief -= operator overload
   * @param b the other matrix to subtract
   * @return *this minus b
   *
   */
  Matrix2&
  operator-=(const Matrix2& b);

  /**
   * @brief *= operator overload
   * @param b the other matrix to multiply
   * @return *this summed b
   *
   */
  Matrix2&
  operator*=(const Matrix2& b);

  /**
   * @brief += operator overload
   * @param value to add to the matrix
   * @return naumatrix equal to *this summed value
   *
   */
  Matrix2&
  operator+=(const float& value);

  /**
   * @brief -= operator overload
   * @param value to substract to the matrix
   * @return naumatrix equal to *this minus value
   *
   */
  Matrix2&
  operator-=(const float& value);

  /**
   * @brief *= operator overload
   * @param value to multiply the matrix
   * @return naumatrix equal to *this times value
   *
   */
  Matrix2&
  operator*=(const float& value);

  /**
   * @brief /= operator overload
   * @param value to divide to the matrix
   * @return naumatrix equal to *this folded value
   *
   */
  Matrix2&
  operator/=(const float& value);

  /**
   * @brief == operator overload
   * @param nauMatrix to compare
   * @return true if *this is equal to b
   *
   */
  bool
  operator==(const Matrix2& b);

  /**
   * @brief Sets this matrix to identity.
   */
  void
  identity();

  /**
   * @brief Sets all matrix elements to zero.
   */
  void
  zero();

  /**
   * @brief Returns the transpose of this matrix.
   * @return Transposed copy of this matrix.
   */
  Matrix2
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
  Matrix2
  cofactored() const;

  /**
   * @brief Replaces this matrix with its cofactor matrix.
   */
  void
  cofactor();

  /**
   * @brief Sets all elements to the same value.
   * @param value Value to assign to all elements.
   */
  void
  setValues(const float& value);

  /**
   * @brief Sets matrix elements explicitly.
   * @param m00 Element at row 0, column 0.
   * @param m10 Element at row 0, column 1.
   * @param m01 Element at row 1, column 0.
   * @param m11 Element at row 1, column 1.
   */
  void
  setValues(const float& m00, const float& m10,
            const float& m01, const float& m11);

  /**
   * @brief Returns the inverse of this matrix.
   * @return Inverse matrix.
   */
  Matrix2
  inversed();

  /**
   * @brief Inverts this matrix in place.
   */
  void
  inverse();

  /**
   * @brief Computes matrix determinant.
   * @return Determinant value.
   */
  const float
  determinant() const;

  /**
   * @brief Converts matrix to string representation.
   * @return Formatted matrix string.
   */
  String
  toString();

  /**
   * @brief Zero matrix constant.
   */
  static const Matrix2 ZERO;

  /**
   * @brief Identity matrix constant.
   */
  static const Matrix2 IDENTITY;

public:

  union {
    /**
     * Row major based matrix struct
     */
    struct {
      float m00, m10;
      float m01, m11;
    }_m;
    float m[2][2];
    Vector2f columns[2];
    float fVec[4];
  };



};

}

