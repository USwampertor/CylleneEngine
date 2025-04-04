#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector4f.h"
#include "cyQuaternion.h"

namespace CYLLENE_SDK {

// Forward class declaration
class Matrix2x2;
class Matrix3x3;

class CY_MATH_EXPORT Matrix4x4
{
 public:
  Matrix4x4() = default;

  ~Matrix4x4() = default;

  Matrix4x4(const float& value);

  Matrix4x4(const Matrix4x4& other);

  Matrix4x4(const Matrix3x3& other);

  Matrix4x4(const Matrix2x2& other);

  Matrix4x4(const float& m00, const float& m01, const float& m02, const float& m03,
            const float& m10, const float& m11, const float& m12, const float& m13,
            const float& m20, const float& m21, const float& m22, const float& m23,
            const float& m30, const float& m31, const float& m32, const float& m33);

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
  setValues(const float& v00, const float& v01, const float& v02, const float& v03,
            const float& v10, const float& v11, const float& v12, const float& v13,
            const float& v20, const float& v21, const float& v22, const float& v23,
            const float& v30, const float& v31, const float& v32, const float& v33);

  void
  setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir);

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

  Vector3f
  transformPosition(const Vector3f& v) const;

  Vector3f
  transformDirection(const Vector3f& v) const;

  void
  translate(const Vector3f& translation);

  void
  setPosition(const Vector3f& position);

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
  getPosition() const;

  Vector3f
  getScale() const;

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

//   union {
//     // Row major based matrix struct
//     struct {
//       float m00, m01, m02, m03;
//       float m10, m11, m12, m13;
//       float m20, m21, m22, m23;
//       float m30, m31, m32, m33;
//     }_m;
//     float m[4][4];
//     Vector4f vec[4];
//     float fVec[16];
//   };
  union {
    // Column-major storage
    struct {
      float m00, m10, m20, m30;
      float m01, m11, m21, m31;
      float m02, m12, m22, m32;
      float m03, m13, m23, m33;
    } _m;
    float m[4][4];  // m[column][row]
    Vector4f columns[4];  // Explicit column storage
    float fVec[16];  // Flat array (column-major order)
  };

  
};
} // namespace CYLLENE_SDK