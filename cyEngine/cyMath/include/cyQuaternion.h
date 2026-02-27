/**
 * @file cyQuaternion.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Quaternion.
 */
#pragma once

#include "cyMathPrerequisites.h"

#include "cyEulerHelpers.h"

namespace CYLLENE_SDK {

// Forward declaration
class Vector3f;
class Vector4f;
class Matrix3;
class Matrix4;
class Quaternion;

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Euler ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @struct Euler
 * @brief an Euler helper to define rotations
 */
struct CY_MATH_EXPORT Euler {
public:
  /**
   * @brief Constructs an uninitialized Euler rotation.
   */
  Euler() = default;

  /**
   * @brief Destroys the Euler rotation.
   */
  ~Euler() = default;

  /**
   * @brief Constructs Euler rotation with a specific order.
   * @param norder Euler order.
   */
  Euler(const int32& norder);

  /**
   * @brief Constructs Euler rotation from XYZ angles.
   * @param nx X angle.
   * @param ny Y angle.
   * @param nz Z angle.
   */
  Euler(const float& nx, const float& ny, const float& nz);

  /**
   * @brief Constructs Euler rotation from XYZ angles and order.
   * @param nx X angle.
   * @param ny Y angle.
   * @param nz Z angle.
   * @param norder Euler order.
   */
  Euler(const float& nx, const float& ny, const float& nz, const int32& norder);

  /**
   * @brief Constructs Euler rotation from a Vector3f.
   * @param vector Source vector.
   */
  Euler(const Vector3f& vector);

  /**
   * @brief Constructs Euler rotation from a Vector4f.
   * @param vector Source vector.
   */
  Euler(const Vector4f& vector);

  /**
   * @brief Constructs Euler rotation from a Matrix3.
   * @param rotationMatrix Source rotation matrix.
   * @param order Euler order.
   */
  Euler(const Matrix3& rotationMatrix, const int32& order = EulOrdXYZs);

  /**
   * @brief Constructs Euler rotation from a Matrix4.
   * @param rotationMatrix Source rotation matrix.
   * @param order Euler order.
   */
  Euler(const Matrix4& rotationMatrix, const int32& order = EulOrdXYZs);

  /**
   * @brief Constructs Euler rotation from a quaternion.
   * @param rotationQuat Source quaternion.
   * @param order Euler order.
   */
  Euler(const Quaternion& rotationQuat, const int32& order = EulOrdXYZs);

  /**
   * @brief Converts this Euler rotation to Matrix4.
   * @return Rotation matrix.
   */
  Matrix4
  getMatrix4Rotation() const;

  /**
   * @brief Converts this Euler rotation to Matrix3.
   * @return Rotation matrix.
   */
  Matrix3
  getMatrix3Rotation() const;

  /**
   * @brief Converts this Euler rotation to quaternion.
   * @return Quaternion rotation.
   */
  Quaternion
  getQuatRotation() const;

  /**
   * @brief Converts Euler rotation to a string representation.
   * @return Formatted Euler string.
   */
  String
  toString();

public:

  /**
   * @brief x component
   */
  float x;

  /**
   * @brief y component
   */
  float y;

  /**
   * @brief z component
   */
  float z;

  int32 order;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Quaternion //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Quaternion
 * @brief A quaternion object to make rotations without gimbal locking
 */
class CY_MATH_EXPORT Quaternion {
    
public:
  /**
   * @brief Constructs an uninitialized quaternion.
   */
  Quaternion() = default;

  /**
   * @brief Destroys the quaternion.
   */
  ~Quaternion() = default;

  /**
   * @brief Constructs a quaternion from explicit components.
   * @param nx X component.
   * @param ny Y component.
   * @param nz Z component.
   * @param nw W component.
   */
  Quaternion(const float& nx, const float& ny, const float& nz, const float& nw);

  /**
   * @brief Constructs a quaternion from a Vector3f.
   * @param other Source vector.
   */
  Quaternion(const Vector3f& other);

  /**
   * @brief Constructs a quaternion from a Vector4f.
   * @param other Source vector.
   */
  Quaternion(const Vector4f other);

  /**
   * @brief Constructs a quaternion from Euler angles.
   * @param euler Source Euler rotation.
   */
  Quaternion(const Euler& euler);

  /**
   * @brief Constructs a quaternion from a Matrix3 rotation.
   * @param matrix Source matrix.
   */
  Quaternion(const Matrix3& matrix);

  /**
   * @brief Constructs a quaternion from a Matrix4 rotation.
   * @param matrix Source matrix.
   */
  Quaternion(const Matrix4& matrix);

  /**
   * @brief [] operator overload, will return the value at a given position
   * @param index of the object we wanna get
   * @return value at given index
   */
  float&
  operator[](uint32 index);

  /**
   * @brief const [] operator overload, will return the value at a given position
   * @param index of the object we wanna get
   * @return value at given index
   */
  float
  operator[](uint32 index) const;

  /**
   * @brief + operator overload
   * @param other The quaternion to add.
   * @return Quaternion sum.
   */
  Quaternion
  operator+(const Quaternion& other) const;
  
  /**
   * @brief - operator overload
   * @param other The quaternion to subtract.
   * @return Quaternion difference.
   */
  Quaternion
  operator-(const Quaternion& other) const;
  
  /**
   * @brief * operator overload
   * @param other The quaternion to multiply by.
   * @return Quaternion product.
   */
  Quaternion
  operator*(const Quaternion& other) const;
  
  /**
   * @brief / operator overload with quaternions
   * @param other The quaternion divisor.
   * @return Quaternion quotient.
   */
  Quaternion
  operator/(const Quaternion& other) const;

  /**
   * @brief / operator overload
   * @param scale Scalar divisor.
   * @return Quaternion divided by scalar.
   */
  Quaternion
  operator/(const float& scale) const;

  /**
   * @brief / operator overload
   * @param scale Scalar multiplier.
   * @return Quaternion multiplied by scalar.
   */
  Quaternion
  operator*(const float& scale) const;
  
  /**
   * @brief Operator = overload
   * @param other Source quaternion.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator=(const Quaternion& other);

  /**
   * @brief == operator overload
   * @param other Quaternion to compare.
   * @return True if quaternions are equal.
   */
  bool
  operator==(const Quaternion& other);

  /**
   * @brief != operator overload
   * @param other Quaternion to compare.
   * @return True if quaternions are not equal.
   */
  bool
  operator!=(const Quaternion& other);

  /**
   * @brief < operator overload
   * @param other Quaternion to compare.
   * @return True if this quaternion is less than other.
   */
  bool
  operator<(const Quaternion& other);

  /**
   * @brief > operator overload
   * @param other Quaternion to compare.
   * @return True if this quaternion is greater than other.
   */
  bool
  operator>(const Quaternion& other);

  /**
   * @brief <= operator overload
   * @param other Quaternion to compare.
   * @return True if this quaternion is less than or equal to other.
   */
  bool
  operator<=(const Quaternion& other);

  /**
   * @brief >= operator overload
   * @param other Quaternion to compare.
   * @return True if this quaternion is greater than or equal to other.
   */
  bool
  operator>=(const Quaternion& other);
  
  /**
   * @brief Returns the negated quaternion.
   * @return Negated quaternion.
   */
  Quaternion
  operator-() const;

  /**
   * @brief += operator overload
   * @param other Quaternion to add.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator+=(const Quaternion& other);
  
  /**
   * @brief -= operator overload
   * @param other Quaternion to subtract.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator-=(const Quaternion& other);
  
  /**
   * @brief *= operator overload
   * @param other Quaternion to multiply by.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator*=(const Quaternion& other);

  /**
   * @brief /= operator overload with quaternions
   * @param other Quaternion divisor.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator/=(const Quaternion& other);

  /**
   * @brief *= operator overload
   * @param scale Scalar multiplier.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator*=(float scale);
  
  /**
   * @brief /= operator overload
   * @param scale Scalar divisor.
   * @return Reference to this quaternion.
   */
  Quaternion&
  operator/=(float scale);
  
  /**
   * @brief | operator overload for dot product
   * @param v The other quaternion.
   * @return Dot product.
   */
  float
  operator|(const Quaternion& v) const;

  /**
   * @brief Rotates a vector by a quaternion.
   * @param vector Input vector.
   * @param quaternion Rotation quaternion.
   * @return Rotated vector.
   */
  static const Vector3f
  transform(const Vector3f& vector, const Quaternion& quaternion);

  /**
   * @brief Spherical linear interpolation between two quaternions.
   * @param q1 Start quaternion.
   * @param q2 End quaternion.
   * @param t Interpolation factor in [0, 1].
   * @return Interpolated quaternion.
   */
  static const Quaternion
  slerp(const Quaternion& q1, const Quaternion& q2, float t);

  /**
   * @brief Sets rotation from Euler angles.
   * @param euler Euler rotation.
   */
  void
  setRotation(const Euler& euler);

  /**
   * @brief Sets rotation from a Matrix3.
   * @param m Rotation matrix.
   */
  void
  setRotation(const Matrix3& m);

  /**
   * @brief Sets rotation from a Matrix4.
   * @param m Rotation matrix.
   */
  void
  setRotation(const Matrix4& m);

  /**
   * @brief Sets quaternion components explicitly.
   * @param nx X component.
   * @param ny Y component.
   * @param nz Z component.
   * @param nw W component.
   */
  void
  setValues(const float& nx, const float& ny, const float& nz, const float& nw);

  /**
   * @brief Sets quaternion from vector and scalar parts.
   * @param vector Vector part.
   * @param scalar Scalar part.
   */
  void 
  setValues(const Vector3f& vector, const float& scalar);

  /**
   * @brief Gets quaternion vector part.
   * @return Vector part (x, y, z).
   */
  const Vector3f
  getVectorPart() const;

  /**
   * @brief Gets quaternion scalar part.
   * @return Scalar part (w).
   */
  const float&
  getScalarPart() const;

  /**
   * @brief Converts quaternion to Euler angles.
   * @param order Euler order.
   * @return Euler rotation.
   */
  Euler
  getEulerRotation(const int32& order = EulOrdXYZs) const;

  /**
   * @brief Converts quaternion to Matrix3 rotation.
   * @return 3x3 rotation matrix.
   */
  const Matrix3
  getMatrix3Rotation() const;

  /**
   * @brief Converts quaternion to Matrix4 rotation.
   * @return 4x4 rotation matrix.
   */
  const Matrix4
  getMatrix4Rotation() const;

  /**
   * @brief Computes quaternion norm.
   * @return Norm value.
   */
  float 
  norm() const;

  /**
   * @brief Computes quaternion magnitude.
   * @return Magnitude value.
   */
  float
  magnitude() const;

  /**
   * @brief Returns a scaled copy of this quaternion.
   * @param s Scale factor.
   * @return Scaled quaternion.
   */
  Quaternion
  scaled(const float& s) const;

  /**
   * @brief Scales this quaternion in place.
   * @param s Scale factor.
   */
  void 
  scale(const float& s);

  /**
   * @brief Returns normalized quaternion.
   * @return Normalized quaternion.
   */
  Quaternion
  normalized() const;

  /**
   * @brief Normalizes this quaternion in place.
   */
  void
  normalize();

  /**
   * @brief Returns quaternion conjugate.
   * @return Conjugated quaternion.
   */
  Quaternion
  conjugated() const;

  /**
   * @brief Conjugates this quaternion in place.
   */
  void
  conjugate();

  /**
   * @brief Returns quaternion inverse.
   * @return Inverted quaternion.
   */
  Quaternion
  inversed() const;

  /**
   * @brief Inverts this quaternion in place.
   */
  void 
  inverse();

  /**
   * @brief Returns a unit quaternion copy.
   * @return Unit quaternion.
   */
  Quaternion
  unitQuaternion() const;

  /**
   * @brief Converts this quaternion to unit length in place.
   */
  void
  unit();

  /**
   * @brief Rotates a vector by this quaternion.
   * @param v Input vector.
   * @return Rotated vector.
   */
  Vector3f
  rotate(const Vector3f& v) const;

  /**
   * @brief Checks if quaternion is pure (zero scalar part).
   * @return True if pure quaternion.
   */
  bool
  isPure() const;

  /**
   * @brief Checks if quaternion is real (zero vector part).
   * @return True if real quaternion.
   */
  bool
  isReal() const;

  /**
   * @brief Converts quaternion to Vector3f.
   * @return Vector3f using xyz components.
   */
  Vector3f
  toVector3() const;

  /**
   * @brief Converts quaternion to Vector4f.
   * @return Vector4f using xyzw components.
   */
  Vector4f 
  toVector4() const;

  /**
   * @brief Converts quaternion to a string representation.
   * @return Formatted quaternion string.
   */
  String
  toString() const;

  /**
   * @brief a const version of what an Identity quaternion looks like
   */
  static const Quaternion
  IDENTITY;

public:

  /**
   * @brief x component
   */
  float x;

  /**
   * @brief y component
   */
  float y;

  /**
   * @brief z component
   */
  float z;

  /**
   * @brief w component
   */
  float w;

};

}
