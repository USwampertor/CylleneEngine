#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector2f.h"

namespace CYLLENE_SDK {

// Forward definition
class Matrix3;
class Matrix4;

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

  void
  identity();

  void
  zero();

  Matrix2
  transposed() const;

  void
  transpose();

  Matrix2
  cofactored() const;

  void
  cofactor();

  void
  setValues(const float& value);

  void
  setValues(const float& m00, const float& m10,
            const float& m01, const float& m11);

  Matrix2
  inversed();

  void
  inverse();

  const float
  determinant() const;

  String
  toString();

  /**
    * ZERO filled Matrix
    */
  static const Matrix2 ZERO;

  /**
    * IDENTITY matrix
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
