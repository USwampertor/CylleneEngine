#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector2f;

  class CY_UTILITY_EXPORT Vector2i
  {
  public:

    Vector2i() = default;

    ~Vector2i() = default;

    Vector2i(const int32& nx, const int32& ny);

    Vector2i(const int32& values);

    Vector2i(const Vector2f& other);

    Vector2i(const Vector2i& other);

    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const int32&
    operator()(const int32& index) const;
    
    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    int32&
    operator()(const int32& index);
    
    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const int32
    operator[](const int32& index) const;

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    int32&
    operator[](const int32& index);

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return a vector sum of *this and v
     *
     */
    Vector2i
    operator+(const Vector2i& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector2i
    operator-(const Vector2i& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector2i
    operator*(const Vector2i& v) const;

    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector2i
    operator/(const Vector2i& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector2i
    operator+(const int32& plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector2i
    operator-(const int32& minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector2i
    operator*(const int32& times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector2i
    operator/(const int32& under) const;

    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    int32
    operator|(const Vector2i& v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    int32
    operator^(const Vector2i& v) const;

    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector2i& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector2i& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector2i& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector2i& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector2i& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector2i& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return the negative of the vector
     *
     */
    Vector2i
    operator-() const;

    /**
     * @brief += operator overload
     * @param the other vector to add
     * @return *this + v components
     *
     */
    Vector2i&
    operator+=(const Vector2i& v);

    /**
     * @brief -= operator overload
     * @param the other vector to subtract
     * @return *this - v components
     *
     */
    Vector2i&
    operator-=(const Vector2i& v);

    /**
     * @brief *= operator overload
     * @param the other vector to multiply
     * @return *this * v components
     *
     */
    Vector2i&
    operator*=(const Vector2i& v);

    /**
     * @brief /= operator overload
     * @param the other vector to divide
     * @return *this / v components
     *
     */
    Vector2i&
    operator/=(const Vector2i& v);

    /**
     * @brief *= operator overload
     * @param the float to multiply with
     * @return *this * float
     *
     */
    Vector2i&
    operator*=(const int32& scale);

    /**
     * @brief /= operator overload
     * @param the float to divide the vector
     * @return *this / float
     *
     */
    Vector2i&
    operator/=(const int32& scale);

    /**
     * @brief the dot product
     * @param a nauVector2 and b nauVector2
     * @return the dot product between a and b
     *
     */
    static int32
    dot(const Vector2i& a, const Vector2i& b);

    /**
     * @brief the cross product
     * @param a nauVector2 and b nauVector2
     * @return the cross product between a and b
     *
     */
    static int32
    cross(const Vector2i& a, const Vector2i& b);

    /**
     * @brief the scale of b over a
     * @param Vector2 that is the reflection and b 
     * @param Vector2 that reflects over
     * @return scale of b in a
     *
     */
    static int32
    projection(const Vector2i& a, const Vector2i& b);

    /**
     * @brief the square distance
     * @param a nauVector2 and b nauVector2
     * @return the square distance bewteen the two points
     *
     */
    static int32
    sqrDistance(const Vector2i& a, const Vector2i& b);

    /**
     * @brief the distance between two points in 2d space
     * @param a nauVector2 and b nauVector2
     * @return the distance bewteen the two points
     *
     */
    static int32
    distance(const Vector2i& a, const Vector2i& b);

    /**
     * @brief sets the value of a nauVector2
     * @param float x and y
     * @return
     *
     */
    void
    setValues(int32 newX, int32 newY);

    /**
     * @brief gets the min between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    min(const Vector2i& v);

    /**
     * @brief gets the max between two vectors
     * @param the other vector to compare
     * @return
     *
     */
    void
    max(const Vector2i& v);
    
    /**
     * @brief gets the highest value of the vector
     * @param
     * @return
     *
     */
    int32
    getHighest() const;

    /**
     * @brief gets the least value of the vector
     * @param
     * @return
     *
     */
    int32
    getLowest() const;

    /**
     * @brief gets the magnitude value of the vector
     * @param
     * @return
     *
     */
    int32
    magnitude() const;

    /**
     * @brief gets the squared magnitude value of the vector
     * @param
     * @return
     *
     */
    int32
    sqrMagnitude() const;

    /**
     * @brief gets the normalized version of the vector
     * @param
     * @return the normalized vector
     *
     */
    Vector2i 
    normalized() const;

    /**
     * @brief gets the QUICK normalized version of the vector with the Carmack Equation
     * Use this if you don't have problems having a small error difference from the real value
     * @param
     * @return the QUICK normalized vector
     *
     */
    Vector2i
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
     * @param Vector other
     * @return true if based on the error is the same
     *
     */
    static bool
    areSame(const Vector2i& a, const Vector2i& b);

    /**
     * @brief Checks if one vector is near the same as another vector based in an error
     * @param Vector other
     * @return true if based on the error is the same
     *
     */
    static bool
    areNearlySame(const Vector2i& a, const Vector2i& b, const int32& threshold);

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
    static const Vector2i ZERO;

    /**
     * vauVector2 with all values 1
     */
    static const Vector2i ONE;

    /**
     * vauVector2 with x value 1
     */
    static const Vector2i ONEX;

    /**
     * vauVector2 with y value 1
     */
    static const Vector2i ONEY;

    /**
     * vauVector2 with y value 1. This is our right in our world
     */
    static const Vector2i UP;

    /**
     * vauVector2 with x value 1. This is our up in our world
     */
    static const Vector2i RIGHT;

    /**
     * Member declaration
     */

   private:

    /**
     * x component
     */
    int32 m_x;

    /**
     * y component
     */
    int32 m_y;

  };

}