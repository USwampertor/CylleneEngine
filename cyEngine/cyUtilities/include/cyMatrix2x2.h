#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyVector2f.h"

namespace CYLLENE_SDK {

// Forward definition
class Matrix3x3;
class Matrix4x4;

class CY_UTILITY_EXPORT Matrix2x2
{
 public:

  Matrix2x2() = default;

  ~Matrix2x2() = default;

  Matrix2x2(const float& value);

  Matrix2x2(const Matrix2x2& other);

  Matrix2x2(const float& v00, const float& v01,
            const float& v10, const float& v11);

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return the sum of *this and b
    *
    */
  Matrix2x2
  operator+(const Matrix2x2& b);

  /**
    * @brief - operator overload
    * @param b the other matrix to subtract
    * @return the difference of *this and b
    *
    */
  Matrix2x2
  operator-(const Matrix2x2& b);

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return the multiplication of *this and b
    *
    */
  Matrix2x2
  operator*(const Matrix2x2& b);

  /**
    * @brief += operator overload
    * @param b the other matrix to add
    * @return *this summed b
    *
    */
  Matrix2x2&
  operator+=(const Matrix2x2& b);

  /**
    * @brief -= operator overload
    * @param b the other matrix to subtract
    * @return *this minus b
    *
    */
  Matrix2x2&
  operator-=(const Matrix2x2& b);

  /**
    * @brief *= operator overload
    * @param b the other matrix to multiply
    * @return *this summed b
    *
    */
  Matrix2x2&
  operator*=(const Matrix2x2& b);

  /**
    * @brief += operator overload
    * @param value to add to the matrix
    * @return naumatrix equal to *this summed value
    *
    */
  Matrix2x2&
  operator+=(const float& value);

  /**
    * @brief -= operator overload
    * @param value to substract to the matrix
    * @return naumatrix equal to *this minus value
    *
    */
  Matrix2x2&
  operator-=(const float& value);

  /**
    * @brief *= operator overload
    * @param value to multiply the matrix
    * @return naumatrix equal to *this times value
    *
    */
  Matrix2x2&
  operator*=(const float& value);

  /**
    * @brief /= operator overload
    * @param value to divide to the matrix
    * @return naumatrix equal to *this folded value
    *
    */
  Matrix2x2&
  operator/=(const float& value);

  /**
    * @brief == operator overload
    * @param nauMatrix to compare
    * @return true if *this is equal to b
    *
    */
  bool
  operator==(const Matrix2x2& b);

  void
  identity();

  void
  zero();

  Matrix2x2
  transposed() const;

  void
  transpose();

  Matrix2x2
  cofactored() const;

  void
  cofactor();

  void
  setValues(const float& value);

  void
  setValues(const float& v00, const float& v01,
            const float& v10, const float& v11);

  Matrix2x2
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
  static const Matrix2x2 ZERO;

  /**
    * IDENTITY matrix
    */
  static const Matrix2x2 IDENTITY;

public:

  union {
    /**
      * Row major based matrix struct
      */
    struct {
      float m00, m01;
      float m10, m11;
    }_m;
    float m[2][2];
    Vector2f vec[2];
    float fVec[4];
  };



};

}
