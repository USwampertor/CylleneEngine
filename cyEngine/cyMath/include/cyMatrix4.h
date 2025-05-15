#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector4f.h"
#include "cyQuaternion.h"
#include "cyRotor.h"

namespace CYLLENE_SDK {

// Forward class declaration
class Matrix2;
class Matrix3;

class CY_MATH_EXPORT Matrix4
{
 public:
  Matrix4() = default;

  ~Matrix4() = default;

  Matrix4(const float& value);

  Matrix4(const Matrix4& other);

  Matrix4(const Matrix3& other);

  Matrix4(const Matrix2& other);

  Matrix4(const float& m00, const float& m01, const float& m02, const float& m03,
          const float& m10, const float& m11, const float& m12, const float& m13,
          const float& m20, const float& m21, const float& m22, const float& m23,
          const float& m30, const float& m31, const float& m32, const float& m33);

  /**
    * @brief + operator overload
    * @param b the other matrix to add
    * @return the sum of *this and b
    *
    */
  Matrix4
  operator+(const Matrix4& b);

  /**
    * @brief - operator overload
    * @param b the other matrix to subtract
    * @return the difference of *this and b
    *
    */
  Matrix4
  operator-(const Matrix4& b);

  /**
    * @brief * operator overload
    * @param b the other matrix to multiply
    * @return the multiplication of *this and b
    *
    */
  Matrix4
  operator*(const Matrix4& b);

  /**
    * @brief += operator overload
    * @param b the other matrix to add
    * @return *this summed b
    *
    */
  Matrix4&
  operator+=(const Matrix4& b);

  /**
    * @brief -= operator overload
    * @param b the other matrix to subtract
    * @return *this minus b
    *
    */
  Matrix4&
  operator-=(const Matrix4& b);

  /**
    * @brief *= operator overload
    * @param b the other matrix to multiply
    * @return *this summed b
    *
    */
  Matrix4&
  operator*=(const Matrix4& b);

  /**
    * @brief += operator overload
    * @param value to add to the matrix
    * @return naumatrix equal to *this summed value
    *
    */
  Matrix4&
  operator+=(const float& value);

  /**
    * @brief -= operator overload
    * @param value to substract to the matrix
    * @return naumatrix equal to *this minus value
    *
    */
  Matrix4&
  operator-=(const float& value);

  /**
    * @brief *= operator overload
    * @param value to multiply the matrix
    * @return naumatrix equal to *this times value
    *
    */
  Matrix4&
  operator*=(const float& value);

  /**
    * @brief /= operator overload
    * @param value to divide to the matrix
    * @return naumatrix equal to *this folded value
    *
    */
  Matrix4&
  operator/=(const float& value);

  /**
    * @brief == operator overload
    * @param nauMatrix to compare
    * @return true if *this is equal to b
    *
    */
  bool
  operator==(const Matrix4& b);

  void
  identity();

  void
  zero();

  Matrix4
  transposed() const;

  void
  transpose();

  Matrix4
  cofactored() const;

  void
  cofactor();

  void
  setValues(const float& value);

  void
  setValues(const float& v00, const float& v10, const float& v20, const float& v30,
            const float& v01, const float& v11, const float& v21, const float& v31,
            const float& v02, const float& v12, const float& v22, const float& v32,
            const float& v03, const float& v13, const float& v23, const float& v33);

  void
  setTransformMatrix(const Vector3f&    position,
                     const Quaternion&  rotation,
                     const Vector3f&    scale);

  void
  setTransformMatrix(const Vector3f& position, const Rotor& rotation);

  void
  setLookAt(const Vector3f& eyePos,
            const Vector3f& targetPos,
            const Vector3f& upDir);

  Matrix4
  inversed();

  void
  inverse();

  const float
  determinant() const;

  Matrix4&
  view(const Vector4f& Eye,
       const Vector4f& Target,
       const Vector4f& Up);

  Matrix4&
  orthogonal(const float& width,
             const float& height,
             const float& zNear,
             const float& zFar);

  Matrix4&
  orthogonal(const float& top,
             const float& bottom,
             const float& left,
             const float& right,
             const float& zNear,
             const float& zFar);

  Matrix4&
  perspective(const float Width,
              const float Height,
              const float ZNear,
              const float ZFar,
              const float FOV);

  Vector3f
  transformPosition(const Vector3f& v) const;

  Vector3f
  transformDirection(const Vector3f& v) const;

  Vector4f
  transformPositionV4(const Vector4f& v) const;

  Vector4f
  transformDirectionV4(const Vector4f& v) const;

  void
  translate(const Vector3f& translation);

  void
  translate(const float& x, const float& y, const float& z);

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
  rotate(const Vector3f& delta);

  void
  scale(const Vector3f& newScale);

  void
  scale(const float& x, const float& y, const float& z);

  void
  scale(const float& newScale);

  void
  setPosition(const Vector3f& position);

  void
  setRotation(const Vector3f& rotation);

  void
  setRotation(const Quaternion& rotation);

  void
  setScale(const Vector3f& newScale);

  void
  setScale(const float& newScale);

  Vector3f
  getForwardVector() const;
  
  Vector3f
  getRightVector() const;

  Vector3f
  getUpVector() const;

  Vector3f
  getPosition() const;

  Euler
  getEulerRotation() const;

  Quaternion
  getQuatRotation() const;

  Vector3f
  getScale() const;

  Matrix4
  getTranslationMatrix() const;

  Matrix4
  getRotationMatrix() const;

  Matrix4
  getScaleMatrix() const;

  const Matrix3&
  subMatrix() const;

  String
  toString();

  /**
    * ZERO filled Matrix
    */
  static const Matrix4 ZERO;

  /**
    * IDENTITY matrix
    */
  static const Matrix4 IDENTITY;


private:

  void
  removeScaleFromRotation(Vector3f scale, Matrix3& rotation) const;


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