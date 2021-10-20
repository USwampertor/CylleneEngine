#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyVector4f.h"

namespace CYLLENE_SDK {

  class Matrix2x2;
  class Matrix3x3;

  class CY_UTILITY_EXPORT Matrix4x4
  {
  public:
    Matrix4x4() = default;

    ~Matrix4x4() = default;

    Matrix4x4(const float& value);

    Matrix4x4(const Matrix4x4& other);

    Matrix4x4(const Matrix3x3& other);

    Matrix4x4(const Matrix2x2& other);

    Matrix4x4(const float& v00, const float& v01, const float& v02, const float v03,
              const float& v10, const float& v11, const float& v12, const float v13,
              const float& v20, const float& v21, const float& v22, const float v23,
              const float& v30, const float& v31, const float& v32, const float v33);

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return the sum of *this and b
     *
     */
    Matrix4x4
    operator+(const Matrix4x4& b);

    /**
     * @brief - operator overload
     * @param b the other matrix to subtract
     * @return the difference of *this and b
     *
     */
    Matrix4x4
    operator-(const Matrix4x4& b);

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return the multiplication of *this and b
     *
     */
    Matrix4x4
    operator*(const Matrix4x4& b);

    /**
     * @brief += operator overload
     * @param b the other matrix to add
     * @return *this summed b
     *
     */
    Matrix4x4&
    operator+=(const Matrix4x4& b);

    /**
     * @brief -= operator overload
     * @param b the other matrix to subtract
     * @return *this minus b
     *
     */
    Matrix4x4&
    operator-=(const Matrix4x4& b);

    /**
     * @brief *= operator overload
     * @param b the other matrix to multiply
     * @return *this summed b
     *
     */
    Matrix4x4&
    operator*=(const Matrix4x4& b);

    /**
     * @brief += operator overload
     * @param value to add to the matrix
     * @return naumatrix equal to *this summed value
     *
     */
    Matrix4x4&
    operator+=(const float& value);

    /**
     * @brief -= operator overload
     * @param value to substract to the matrix
     * @return naumatrix equal to *this minus value
     *
     */
    Matrix4x4&
    operator-=(const float& value);

    /**
     * @brief *= operator overload
     * @param value to multiply the matrix
     * @return naumatrix equal to *this times value
     *
     */
    Matrix4x4&
    operator*=(const float& value);

    /**
     * @brief /= operator overload
     * @param value to divide to the matrix
     * @return naumatrix equal to *this folded value
     *
     */
    Matrix4x4&
    operator/=(const float& value);

    /**
     * @brief == operator overload
     * @param nauMatrix to compare
     * @return true if *this is equal to b
     *
     */
    bool
    operator==(const Matrix4x4& b);

    void
    identity();

    void
    zero();

    Matrix4x4
    transposed() const;

    void
    transpose();

    Matrix4x4
    cofactored() const;

    void
    cofactor();

    void
    setValues(const float& value);

    void
    setValues(const float& v00, const float& v01, const float& v02, const float v03,
              const float& v10, const float& v11, const float& v12, const float v13,
              const float& v20, const float& v21, const float& v22, const float v23,
              const float& v30, const float& v31, const float& v32, const float v33);

    Matrix4x4
    inversed();

    void
    inverse();

    const float
    determinant() const;

    Matrix4x4&
    View(const Vector4f& Eye,
         const Vector4f& Target,
         const Vector4f& Up);

    Matrix4x4&
    Orthogonal(const float Width,
               const float Height,
               const float ZNear,
               const float ZFar);

    Matrix4x4&
    Perspective(const float Width,
                const float Height,
                const float ZNear,
                const float ZFar,
                const float FOV);

    Matrix3x3
    subMatrix();

    String
    toString();

    /**
     * ZERO filled Matrix
     */
    static const Matrix4x4 ZERO;

    /**
     * IDENTITY matrix
     */
    static const Matrix4x4 IDENTITY;

  public:

    union {
      /**
       * Row major based matrix struct
       */
      struct {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;
      }_m;
      float m[4][4];
      Vector4f vec[4];
      float fVec[16];
    };


  
  };
}