/**
 * @file cyMatrix4.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Matrix4.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyVector4f.h"
#include "cyQuaternion.h"
#include "cyRotor.h"

namespace CYLLENE_SDK {

// Forward class declaration
class Matrix2;
class Matrix3;

/**
 * @class Matrix4
 * @brief a column major Matrix of 4x4
 */
class CY_MATH_EXPORT Matrix4
{
 public:
  /**
   * @brief Constructs an uninitialized 4x4 matrix.
   */
  Matrix4() = default;

  /**
   * @brief Destroys the matrix.
   */
  ~Matrix4() = default;

  /**
   * @brief Constructs a matrix initialized from a scalar.
   * @param value Scalar used by the constructor initialization path.
   */
  Matrix4(const float& value);

  /**
   * @brief Constructs a matrix as a copy of another matrix.
   * @param other Source matrix.
   */
  Matrix4(const Matrix4& other);

  /**
   * @brief Constructs a 4x4 matrix from a 3x3 matrix.
   * @param other Source 3x3 matrix.
   */
  Matrix4(const Matrix3& other);

  /**
   * @brief Constructs a 4x4 matrix from a 2x2 matrix.
   * @param other Source 2x2 matrix.
   */
  Matrix4(const Matrix2& other);

  /**
   * @brief Constructs a matrix from 16 explicit components.
   * @param m00 First column, first row component.
   * @param m01 First column, second row component.
   * @param m02 First column, third row component.
   * @param m03 First column, fourth row component.
   * @param m10 Second column, first row component.
   * @param m11 Second column, second row component.
   * @param m12 Second column, third row component.
   * @param m13 Second column, fourth row component.
   * @param m20 Third column, first row component.
   * @param m21 Third column, second row component.
   * @param m22 Third column, third row component.
   * @param m23 Third column, fourth row component.
   * @param m30 Fourth column, first row component.
   * @param m31 Fourth column, second row component.
   * @param m32 Fourth column, third row component.
   * @param m33 Fourth column, fourth row component.
   */
  Matrix4(const float& m00, const float& m01, const float& m02, const float& m03,
          const float& m10, const float& m11, const float& m12, const float& m13,
          const float& m20, const float& m21, const float& m22, const float& m23,
          const float& m30, const float& m31, const float& m32, const float& m33);

  /**
   * @brief + operator overload
   * @param b the other matrix to add
   * @return the sum of *this and b
   */
  Matrix4
  operator+(const Matrix4& b);

  /**
   * @brief - operator overload
   * @param b the other matrix to subtract
   * @return the difference of *this and b
   */
  Matrix4
  operator-(const Matrix4& b);

  /**
   * @brief * operator overload
   * @param b the other matrix to multiply
   * @return the multiplication of *this and b
   */
  Matrix4
  operator*(const Matrix4& b);

  /**
   * @brief += operator overload
   * @param b the other matrix to add
   * @return *this summed b
   */
  Matrix4&
  operator+=(const Matrix4& b);

  /**
   * @brief -= operator overload
   * @param b the other matrix to subtract
   * @return *this minus b
   */
  Matrix4&
  operator-=(const Matrix4& b);

  /**
   * @brief *= operator overload
   * @param b the other matrix to multiply
   * @return *this summed b
   */
  Matrix4&
  operator*=(const Matrix4& b);

  /**
   * @brief += operator overload
   * @param value to add to the matrix
   * @return naumatrix equal to *this summed value
   */
  Matrix4&
  operator+=(const float& value);

  /**
   * @brief -= operator overload
   * @param value to substract to the matrix
   * @return naumatrix equal to *this minus value
   */
  Matrix4&
  operator-=(const float& value);

  /**
   * @brief *= operator overload
   * @param value to multiply the matrix
   * @return naumatrix equal to *this times value
   */
  Matrix4&
  operator*=(const float& value);

  /**
   * @brief /= operator overload
   * @param value to divide to the matrix
   * @return naumatrix equal to *this folded value
   */
  Matrix4&
  operator/=(const float& value);

  /**
   * @brief == operator overload
   * @param b The matrix to compare against.
   * @return true if *this is equal to b
   */
  bool
  operator==(const Matrix4& b);

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
  Matrix4
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
  Matrix4
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
   * @brief Sets all 16 matrix values explicitly.
   * @param v00 First column, first row.
   * @param v10 Second column, first row.
   * @param v20 Third column, first row.
   * @param v30 Fourth column, first row.
   * @param v01 First column, second row.
   * @param v11 Second column, second row.
   * @param v21 Third column, second row.
   * @param v31 Fourth column, second row.
   * @param v02 First column, third row.
   * @param v12 Second column, third row.
   * @param v22 Third column, third row.
   * @param v32 Fourth column, third row.
   * @param v03 First column, fourth row.
   * @param v13 Second column, fourth row.
   * @param v23 Third column, fourth row.
   * @param v33 Fourth column, fourth row.
   */
  void
  setValues(const float& v00, const float& v10, const float& v20, const float& v30,
            const float& v01, const float& v11, const float& v21, const float& v31,
            const float& v02, const float& v12, const float& v22, const float& v32,
            const float& v03, const float& v13, const float& v23, const float& v33);

  /**
   * @brief Builds a transform matrix from position, rotation, and scale.
   * @param position Translation component.
   * @param rotation Rotation component.
   * @param scale Scale component.
   */
  void
  setTransformMatrix(const Vector3f&    position,
                     const Quaternion&  rotation,
                     const Vector3f&    scale);

  /**
   * @brief Builds a transform matrix from position and rotor rotation.
   * @param position Translation component.
   * @param rotation Rotor rotation component.
   */
  void
  setTransformMatrix(const Vector3f& position, const Rotor& rotation);

  /**
   * @brief Builds a look-at matrix.
   * @param eyePos Camera eye position.
   * @param targetPos Camera target position.
   * @param upDir Up direction.
   */
  void
  setLookAt(const Vector3f& eyePos,
            const Vector3f& targetPos,
            const Vector3f& upDir);

  /**
   * @brief Returns the inverse of this matrix.
   * @return Inverse matrix.
   */
  Matrix4
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
   * @brief Builds a view matrix.
   * @param Eye Eye position.
   * @param Target Target position.
   * @param Up Up direction.
   * @return Reference to this matrix.
   */
  Matrix4&
  view(const Vector4f& Eye,
       const Vector4f& Target,
       const Vector4f& Up);

  /**
   * @brief Builds an orthographic projection using viewport width and height.
   * @param width View width.
   * @param height View height.
   * @param zNear Near clip plane.
   * @param zFar Far clip plane.
   * @return Reference to this matrix.
   */
  Matrix4&
  orthogonal(const float& width,
             const float& height,
             const float& zNear,
             const float& zFar);

  /**
   * @brief Builds an orthographic projection from explicit bounds.
   * @param top Top bound.
   * @param bottom Bottom bound.
   * @param left Left bound.
   * @param right Right bound.
   * @param zNear Near clip plane.
   * @param zFar Far clip plane.
   * @return Reference to this matrix.
   */
  Matrix4&
  orthogonal(const float& top,
             const float& bottom,
             const float& left,
             const float& right,
             const float& zNear,
             const float& zFar);

  /**
   * @brief Builds a perspective projection matrix.
   * @param Width View width.
   * @param Height View height.
   * @param ZNear Near clip plane.
   * @param ZFar Far clip plane.
   * @param FOV Vertical field of view in degrees.
   * @return Reference to this matrix.
   */
  Matrix4&
  perspective(const float Width,
              const float Height,
              const float ZNear,
              const float ZFar,
              const float FOV);

  /**
   * @brief Transforms a position using this matrix.
   * @param v Input position.
   * @return Transformed position.
   */
  Vector3f
  transformPosition(const Vector3f& v) const;

  /**
   * @brief Transforms a direction using this matrix.
   * @param v Input direction.
   * @return Transformed direction.
   */
  Vector3f
  transformDirection(const Vector3f& v) const;

  /**
   * @brief Transforms a homogeneous position using this matrix.
   * @param v Input 4D vector.
   * @return Transformed 4D vector.
   */
  Vector4f
  transformPositionV4(const Vector4f& v) const;

  /**
   * @brief Transforms a homogeneous direction using this matrix.
   * @param v Input 4D vector.
   * @return Transformed 4D vector.
   */
  Vector4f
  transformDirectionV4(const Vector4f& v) const;

  /**
   * @brief Applies translation to this matrix.
   * @param translation Translation vector.
   */
  void
  translate(const Vector3f& translation);

  /**
   * @brief Applies translation to this matrix.
   * @param x Translation on X.
   * @param y Translation on Y.
   * @param z Translation on Z.
   */
  void
  translate(const float& x, const float& y, const float& z);

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
   * @brief Applies an Euler delta rotation.
   * @param delta Euler delta rotation.
   */
  void
  rotate(const Vector3f& delta);

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
   * @brief Sets translation component.
   * @param position Position value.
   */
  void
  setPosition(const Vector3f& position);

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
   * @brief Sets scale component.
   * @param newScale Scale vector.
   */
  void
  setScale(const Vector3f& newScale);

  /**
   * @brief Sets uniform scale component.
   * @param newScale Uniform scale.
   */
  void
  setScale(const float& newScale);

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
   * @brief Gets translation component.
   * @return Position vector.
   */
  Vector3f
  getPosition() const;

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
   * @brief Gets translation-only matrix.
   * @return Translation matrix.
   */
  Matrix4
  getTranslationMatrix() const;

  /**
   * @brief Gets rotation-only matrix.
   * @return Rotation matrix.
   */
  Matrix4
  getRotationMatrix() const;

  /**
   * @brief Gets scale-only matrix.
   * @return Scale matrix.
   */
  Matrix4
  getScaleMatrix() const;

  /**
   * @brief Gets the upper-left 3x3 submatrix.
   * @return Reference to 3x3 submatrix.
   */
  const Matrix3&
  subMatrix() const;

  /**
   * @brief Converts matrix to a string representation.
   * @return Formatted matrix string.
   */
  String
  toString();

  /**
   * @brief Zero matrix constant.
   */
  static const Matrix4 ZERO;

  /**
   * @brief Identity matrix constant.
   */
  static const Matrix4 IDENTITY;


private:

  /**
   * @brief Removes scale from rotation basis vectors.
   * @param scale Scale vector to remove.
   * @param rotation Rotation matrix to normalize.
   */
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

