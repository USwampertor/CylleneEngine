#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector2i;

  class Vector3i;
  class Vector3f;

  class Vector4f;
  class Vector4i;

  class CY_UTILITY_EXPORT Vector2f
  {
  public:

    Vector2f() = default;

    Vector2f(const float& nx, const float& ny);

    Vector2f(const float& values);

    Vector2f(const Vector2i& other);

    Vector2f(const Vector2f& other);

    Vector2f(const Vector3i& other);

    Vector2f(const Vector3f& other);

    Vector2f(const Vector4i& other);

    Vector2f(const Vector4f& other);

    ~Vector2f() = default;

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
    Vector2f
    operator+(const Vector2f& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector2f
    operator-(const Vector2f& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector2f
    operator*(const Vector2f& v) const;

    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector2f
    operator/(const Vector2f& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector2f
    operator+(const float& plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector2f
    operator-(const float& minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector2f
    operator*(const float& times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector2f
    operator/(const float& under) const;

    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Vector2f& v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    float
    operator^(const Vector2f& v) const;

    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector2f& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector2f& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector2f& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector2f& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector2f& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector2f& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return the negative of the vector
     *
     */
    Vector2f
    operator-() const;

    /**
     * @brief += operator overload
     * @param the other vector to add
     * @return *this + v components
     *
     */
    Vector2f&
    operator+=(const Vector2f& v);

    /**
     * @brief -= operator overload
     * @param the other vector to subtract
     * @return *this - v components
     *
     */
    Vector2f&
    operator-=(const Vector2f& v);

    /**
     * @brief *= operator overload
     * @param the other vector to multiply
     * @return *this * v components
     *
     */
    Vector2f&
    operator*=(const Vector2f& v);

    /**
     * @brief /= operator overload
     * @param the other vector to divide
     * @return *this / v components
     *
     */
    Vector2f&
    operator/=(const Vector2f& v);

    /**
     * @brief *= operator overload
     * @param the float to multiply with
     * @return *this * float
     *
     */
    Vector2f&
    operator*=(const float& scale);

    /**
     * @brief /= operator overload
     * @param the float to divide the vector
     * @return *this / float
     *
     */
    Vector2f&
    operator/=(const float& scale);

    /**
     * @brief the dot product
     * @param a nauVector2 and b nauVector2
     * @return the dot product between a and b
     *
     */
    static float
    dot(const Vector2f& a, const Vector2f& b);

    /**
     * @brief the cross product
     * @param a nauVector2 and b nauVector2
     * @return the cross product between a and b
     *
     */
    static float
    cross(const Vector2f& a, const Vector2f& b);

    /**
     * @brief the scale of b over a
     * @param Vector2 that is the reflection and b
     * @param Vector2 that reflects over
     * @return scale of b in a
     *
     */
    static float
    projection(const Vector2f& a, const Vector2f& b);

    /**
     * @brief the square distance
     * @param a nauVector2 and b nauVector2
     * @return the square distance bewteen the two points
     *
     */
    static float
    sqrDistance(const Vector2f& a, const Vector2f& b);

    /**
     * @brief the distance between two points in 2d space
     * @param a nauVector2 and b nauVector2
     * @return the distance bewteen the two points
     *
     */
    static float
    distance(const Vector2f& a, const Vector2f& b);

    /**
     * @brief sets the value of a nauVector2
     * @param float x and y
     * @return
     *
     */
    void
    setValues(const float& newX, const float& newY);

    /**
     * @brief gets the min between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    min(const Vector2f& v);

    /**
     * @brief gets the max between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    max(const Vector2f& v);

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
    Vector2f
    normalized() const;

    /**
     * @brief gets the QUICK normalized version of the vector using the Carmack Equation
     * Use this if you don't have problems having a small error difference from the real value
     * @param
     * @return the normalized vector
     *
     */
    Vector2f
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
     * @brief QUICK normalizes the vector using the Carmack Equation
     * Use this if you don't have problems having a small error difference from the real value
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
     * @brief Checks if one vector is the same as another vector
     * @param Vector first Vector to compare
     * @param Vector second Vector to compare
     * @return true if based on the error is the same
     *
     */
    static bool
    isSame(const Vector2f& a, const Vector2f& b);

    /**
     * @brief Checks if one vector is near the same as another vector based in an error
     * @param const Vector2f& first Vector to compare
     * @param const Vector2f& second Vector to compare
     * @param const float& threshold between the magnitude of both vectors
     * @return true if based on the error is the same
     *
     */
    static bool
    isNearlySame(const Vector2f& a, const Vector2f& b, const float& threshold);

    /**
     * @brief Returns the vector as a printable string
     * @param
     * @return
     *
     */
    String
    toString();

    /**
     * static const
     */

  public:

    /**
     * vauVector2 with all values 0
     */
    static const Vector2f ZERO;

    /**
     * vauVector2 with all values 1
     */
    static const Vector2f ONE;

    /**
     * vauVector2 with x value 1
     */
    static const Vector2f ONEX;

    /**
     * vauVector2 with y value 1
     */
    static const Vector2f ONEY;

    /**
     * vauVector2 with y value 1. This is our right in our world
     */
    static const Vector2f UP;

    /**
     * vauVector2 with x value 1. This is our up in our world
     */
    static const Vector2f RIGHT;

    /**
     * Member declaration
     */

  private:

    /**
     * x component
     */
    float m_x;

    /**
     * y component
     */
    float m_y;

  };

}