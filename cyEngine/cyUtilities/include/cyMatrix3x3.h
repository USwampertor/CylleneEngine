#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyVector3f.h"

namespace CYLLENE_SDK {
  class CY_UTILITY_EXPORT Matrix3x3
  {
  public:

    Matrix3x3() = default;

    ~Matrix3x3() = default;

    Matrix3x3(const float& value);

    Matrix3x3(const Matrix3x3& other);

    Matrix3x3(const float& v00, const float& v01, const float& v02,
              const float& v10, const float& v11, const float& v12,
              const float& v20, const float& v21, const float& v22);

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return the sum of *this and b
     *
     */
    Matrix3x3
    operator+(const Matrix3x3& b);

    /**
     * @brief - operator overload
     * @param b the other matrix to subtract
     * @return the difference of *this and b
     *
     */
    Matrix3x3
    operator-(const Matrix3x3& b);

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return the multiplication of *this and b
     *
     */
    Matrix3x3
    operator*(const Matrix3x3& b);

    /**
     * @brief += operator overload
     * @param b the other matrix to add
     * @return *this summed b
     *
     */
    Matrix3x3&
    operator+=(const Matrix3x3& b);

    /**
     * @brief -= operator overload
     * @param b the other matrix to subtract
     * @return *this minus b
     *
     */
    Matrix3x3&
    operator-=(const Matrix3x3& b);

    /**
     * @brief *= operator overload
     * @param b the other matrix to multiply
     * @return *this summed b
     *
     */
    Matrix3x3&
    operator*=(const Matrix3x3& b);

    /**
     * @brief += operator overload
     * @param value to add to the matrix
     * @return naumatrix equal to *this summed value
     *
     */
    Matrix3x3&
    operator+=(const float& value);

    /**
     * @brief -= operator overload
     * @param value to substract to the matrix
     * @return naumatrix equal to *this minus value
     *
     */
    Matrix3x3&
    operator-=(const float& value);

    /**
     * @brief *= operator overload
     * @param value to multiply the matrix
     * @return naumatrix equal to *this times value
     *
     */
    Matrix3x3&
    operator*=(const float& value);

    /**
     * @brief /= operator overload
     * @param value to divide to the matrix
     * @return naumatrix equal to *this folded value
     *
     */
    Matrix3x3&
    operator/=(const float& value);

    /**
     * @brief == operator overload
     * @param nauMatrix to compare
     * @return true if *this is equal to b
     *
     */
    bool
    operator==(const Matrix3x3& b);

    void
    identity();

    void
    zero();

    Matrix3x3
    transposed() const;

    void
    transpose();

    Matrix3x3
    cofactored() const;

    void
    cofactor();

    void
    setValues(const float& value);

    void
    setValues(const float& v00, const float& v01, const float& v02,
              const float& v10, const float& v11, const float& v12,
              const float& v20, const float& v21, const float& v22);

    Matrix3x3
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
    static const Matrix3x3 ZERO;

    /**
     * IDENTITY matrix
     */
    static const Matrix3x3 IDENTITY;

  public:

    union {
      /**
       * Row major based matrix struct
       */
      struct {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
      }_m;
      float m[3][3];
      Vector3f vec[3];
      float fVec[9];
    };



  };
}
