/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyQuaternion.h
 * @author Marco "Swampy" Millan
 * @date 9/1/2021
 * @brief Ah yes, quaternions my old nemesis, now I can call you
 *        my old friend. Quaternions are special vectors used for
 *        rotation regardless of orientation
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector4f;
  class Vector3f;
  class Matrix3x3;
  class Matrix4x4;

  struct CY_UTILITY_EXPORT Euler {
  public:

    Euler() = default;

    ~Euler() = default;

    Euler(const float& nx, const float& ny, const float& nz);

    Euler(const float& nx, const float& ny, const float& nz, const int32& norder);

    Euler(const Vector3f& vector);

    Euler(const Vector4f& vector);

    String
    toString();

  public:
    float x;
    float y;
    float z;

    int32 order;
  };


  class CY_UTILITY_EXPORT Quaternion {
    
  public:
    
    // Default constructor
    Quaternion() = default;

    ~Quaternion() = default;

    /**
     * @brief Constructs without any transformation, passing values as is
     * @param float nx
     * @param float ny
     * @param float nz
     *
     */
    Quaternion(const float& nx, const float& ny, const float& nz, const float& nw)
      : x(nx),
        y(ny),
        z(nz),
        w(nw) {}

    /**
     * @brief Constructs without any transformation, passing values as is
     * @param Vector3f vector to add
     * This Constructor fills the w value with 0
     *
     */
    Quaternion(const Vector3f& other);

    /**
     * @brief Constructs without any transformation, passing values as is
     * @param Vector4f The x y z w components
     *
     */
    Quaternion(const Vector4f other);

    Quaternion(const Euler& euler, const int32& order);

    /**
     * @brief [] operator overload, will return the value at a given position
     * @param index of the object we wanna get
     * @return value at given index
     *
     */
    float&
    operator[](uint32 index);

    /**
     * @brief const [] operator overload, will return the value at a given position
     * @param index of the object we wanna get
     * @return value at given index
     *
     */
    float
    operator[](uint32 index) const;

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return a vector sum of *this and v
     *
     */
    Quaternion
    operator+(const Quaternion& other) const;
  
    /**
     * @brief - operator overload
     * @param b the other matrix to subtract
     * @return a vector difference of *this and v
     *
     */
    Quaternion
    operator-(const Quaternion& other) const;
  
    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Quaternion
    operator*(const Quaternion& other) const;
  
    /**
     * @brief / operator overload with quaternions
     * @param the quaternion to divide
     * @return a quaternion divided by another quaternion
     *
     */
    Quaternion
    operator/(const Quaternion& other) const;

    /**
     * @brief / operator overload
     * @param the scale to divide
     * @return a quaternion divided by scale
     *
     */
    Quaternion
    operator/(const float& scale) const;

    /**
     * @brief / operator overload
     * @param float the scale to multiply
     * @return a vector scaled by scale
     *
     */
    Quaternion
    operator*(const float& scale) const;
  
    /**
     * @brief Operator = overload
     * @param const Quaternion& other quaternion to evaluate
     * @return Quaternion this
     *
     */
    Quaternion&
    operator=(const Quaternion& other);

    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this is == to v components
     *
     */
    bool
    operator==(const Quaternion& other);

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this is != to v components
     *
     */
    bool
    operator!=(const Quaternion& other);

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Quaternion& other);

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Quaternion& other);

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Quaternion& other);

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Quaternion& other);
  
    /**
     * @brief negates the whole quaternion
     * @param 
     * @return 
     *
     */
    Quaternion
    operator-() const;

    /**
     * @brief += operator overload
     * @param the other vector to add
     * @return *this + v components
     *
     */
    Quaternion&
    operator+=(const Quaternion& other);
  
    /**
     * @brief -= operator overload
     * @param the other vector to subtract
     * @return *this - v components
     *
     */
    Quaternion&
    operator-=(const Quaternion& other);
  
    /**
     * @brief *= operator overload
     * @param the other vector to multiply
     * @return *this * v components
     *
     */
    Quaternion&
    operator*=(const Quaternion& other);

    /**
     * @brief /= operator overload with quaternions
     * @param the other quaternion to divide
     * @return *this / q components
     *
     */
    Quaternion&
    operator/=(const Quaternion& other);

    /**
     * @brief *= operator overload
     * @param the float to scale the quaternion
     * @return *this * v components
     *
     */
    Quaternion&
    operator*=(float scale);
  
    /**
     * @brief /= operator overload
     * @param the float to scale the quaternion
     * @return *this / v components
     *
     */
    Quaternion&
    operator/=(float scale);
  
    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Quaternion& v) const;

    static const Vector3f
    transform(const Vector3f& vector, const Quaternion& quaternion);

    static const Quaternion
    slerp(const Quaternion& q1, const Quaternion& q2, float t);

    void
    fromEuler(const Euler& euler, int32 order);

    void
    setValues(const float& nx, const float& ny, const float& nz, const float& nw);

    void 
    setValues(const Vector3f& vector, const float& scalar);

    void
    setRotationMatrix(const Matrix3x3& m);

    const Vector3f
    getVectorPart() const;

    const float&
    getScalarPart() const;

    const Matrix3x3
    getRotationMatrix() const;

    float 
    norm() const;

    float
    magnitude() const;

    Quaternion
    scaled(const float& s) const;

    void 
    scale(const float& s);

    Quaternion
    normalized() const;

    void
    normalize();

    Quaternion
    conjugated() const;

    void
    conjugate();

    Quaternion
    inversed() const;

    void 
    inverse();

    Quaternion
    unitQuaternion() const;

    void
    unit();

    Vector3f
    rotate(const Vector3f& v) const;

    bool
    isPure() const;

    bool
    isReal() const;

    Euler
    toEuler(const int32& order) const;

    Vector3f
    toVector3() const;

    Vector4f 
    toVector4() const;

    String
    toString() const;


  public:

    float x;
    float y;
    float z;
    float w;

  };

}
