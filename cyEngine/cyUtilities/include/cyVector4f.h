/*********************************************/
/*
 * @file 	cyVector4f
 * @author	Marco "Swampertor" Millan
 * @date	12/08/2021
 * @brief	
 *
 */
/******************************************** */
#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector2f;
  class Vector2i;

  class Vector3f;

  class CY_UTILITY_EXPORT Vector4f {
  public:

    /**
     * Default constructor
     */
    Vector4f() = default;

    /**
     * COnstructor taking a Vector3 as argument (you take x y z)
     */
    Vector4f(const Vector2i& other);

    Vector4f(const Vector2f& other);
 
    Vector4f(const Vector3f& other);

    Vector4f(const Vector4f& other);

    Vector4f(const float& nx, const float& ny, const float& nz, const float& nw);

    ~Vector4f() = default;

    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const float&
    operator()(const uint32& index) const;
    
    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    float&
    operator()(const uint32& index);
    
    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const float
    operator[](const uint32& index) const;

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    float&
    operator[](const uint32& index);

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return a vector sum of *this and v
     *
     */
    Vector4f
    operator+(const Vector4f& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector4f
    operator-(const Vector4f& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector4f
    operator*(const Vector4f& v) const;

    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector4f
    operator/(const Vector4f& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector4f
    operator+(const float& plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector4f
    operator-(const float& minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector4f
    operator*(const float& times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector4f
    operator/(const float& under) const;

    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Vector4f& v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    Vector4f
    operator^(const Vector4f& v) const;

    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector4f& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector4f& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector4f& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector4f& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector4f& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector4f& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return the negative of the vector
     *
     */
    Vector4f
    operator-() const;

    /**
     * @brief += operator overload
     * @param the other vector to add
     * @return *this + v components
     *
     */
    Vector4f&
    operator+=(const Vector4f& v);

    /**
     * @brief -= operator overload
     * @param the other vector to subtract
     * @return *this - v components
     *
     */
    Vector4f&
    operator-=(const Vector4f& v);

    /**
     * @brief *= operator overload
     * @param the other vector to multiply
     * @return *this * v components
     *
     */
    Vector4f&
    operator*=(const Vector4f& v);

    /**
     * @brief /= operator overload
     * @param the other vector to divide
     * @return *this / v components
     *
     */
    Vector4f&
    operator/=(const Vector4f& v);

    /**
     * @brief *= operator overload
     * @param the float to multiply with
     * @return *this * float
     *
     */
    Vector4f&
    operator*=(const float& scale);

    /**
     * @brief /= operator overload
     * @param the float to divide the vector
     * @return *this / float
     *
     */
    Vector4f&
    operator/=(const float& scale);

    /**
     * @brief the dot product
     * @param a Vector3 and b Vector3
     * @return the dot product between a and b
     *
     */
    static float
    dot(const Vector4f& a, const Vector4f& b);

    /**
     * @brief the cross product
     * @param a Vector3 and b Vector3
     * @return the cross product between a and b
     *
     */
    static Vector4f
    cross(const Vector4f& a, const Vector4f& b);

    /**
     * @brief the projection of b over a
     * @param Vector3 a the one used as scale
     * @param Vector3 b the one projecting on a
     * @return scale of b in a
     *
     */
    static float
    projection(const Vector4f& a, const Vector4f& b);

    /**
     * @brief the square distance
     * @param a Vector3 and b Vector3
     * @return the square distance between the two points
     *
     */
    static float
    sqrDistance(const Vector4f& a, const Vector4f& b);

    /**
     * @brief the distance between two points in 2d space
     * @param a Vector3 and b Vector3
     * @return the distance between the two points
     *
     */
    static float
    distance(const Vector4f& a, const Vector4f& b);

    /**
     * @brief sets the value of a Vector3
     * @param float x
     * @param float y
     * @param float z
     * @return
     *
     */
    void
    setValues(const float& newX, const float& newY, const float& newZ, const float& newW);

    /**
     * @brief gets the min between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    min(const Vector4f& v);

    /**
     * @brief gets the max between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    max(const Vector4f& v);

    /**
     * @brief floors the components
     * @param
     * @return
     *
     */
    void
    floor();

    /**
     * @brief ceils the components
     * @param
     * @return
     *
     */
    void
    ceiling();

    /**
     * @brief rounds the components x.0 <- -> y.0
     * @param
     * @return
     *
     */
    void
    round();

    /**
     * @brief rounds components taking in account also x.0 - x.5 - y.0
     * @param
     * @return
     *
     */
    void
    roundHalf();

    /**
     * @brief gets the highest value of the vector
     * @param
     * @return
     *
     */
    float
    getHighest() const;

    /**
     * @brief gets the least value of the vector
     * @param
     * @return
     *
     */
    float
    getLowest() const;

    /**
     * @brief gets the magnitude value of the vector
     * @param
     * @return
     *
     */
    float
    magnitude() const;

    /**
     * @brief gets the squared magnitude value of the vector
     * @param
     * @return
     *
     */
    float
    sqrMagnitude() const;

    /**
     * @brief gets the normalized version of the vector
     * @param
     * @return the normalized vector
     *
     */
    Vector4f
    normalized() const;

    /**
     * @brief gets the normalized version of the vector
     * @param
     * @return the normalized vector
     *
     */
    Vector4f
    qNormalized() const;

    /**
     * @brief normalizes the vector
     * @param
     * @return
     *
     */
    void
    normalize();

    /**
     * @brief normalizes the vector
     * @param
     * @return
     *
     */
    void
    qNormalize();

    /**
     * @brief Checks if the vector is empty (0s)
     * @param
     * @return true if it is empty
     *
     */
    bool
    isZero() const;

    /**
     * @brief Checks if one vector is near the same as another vector based in an error
     * @param Vector other, error (default is LITTLENUMBER)
     * @return true if based on the error is the same
     *
     */
    static bool
    isNearlySame(const Vector4f& a, const Vector4f& b, const float& error);

    /**
     * @brief Returns the vector as a printable string
     * @param 
     * @return String with the values of the vector
     *
     */
    String
    toString();

    Vector3f
    toVector3f() const;

    /**
     * nauVector4 that has all values at 0
     */
    static const Vector4f ZERO;

    /**
     * nauVector4 that has all values at 1
     */
    static const Vector4f ONE;

    /**
     * nauVector4 that has x value at 1
     */
    static const Vector4f ONEX;

    /**
     * nauVector4 that has y value at 1
     */
    static const Vector4f ONEY;

    /**
     * nauVector4 that has z value at 1
     */
    static const Vector4f ONEZ;

    /**
     * nauVector4 that has w value at 1
     */
    static const Vector4f ONEW;

    /**
     * Vector4 that has x value 1
     */
    static const Vector4f RIGHT;

    /**
     * Vector4 with y value 1
     */
    static const Vector4f UP;

    /**
     * Vector4 with z value 1
     */
    static const Vector4f FRONT;

  public:
    float x;
    float y;
    float z;
    float w;
  };
}