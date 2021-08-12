/*********************************************/
/*
 * @file 	cyVector3f
 * @author	Marco "Swampertor" Millan
 * @date	11/08/2021
 * @brief	Vector3 made with floats
 *
 */
/******************************************** */
#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  class Vector2f;
  class Vector2i;

  class Vector3i;
  
  class Vector4f;
  class Vector4i;

  class CY_UTILITY_EXPORT Vector3f {
  public:

    Vector3f() = default;

    Vector3f(const Vector3f& other);

    Vector3f(const Vector3i& other);

    Vector3f(const Vector4f& other);

    Vector3f(const Vector4i& other);
 
    Vector3f(const float& nx, const float& ny, const float& nz);

    ~Vector3f() = default;

    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const int32&
    operator()(uint32& index) const;
    
    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    int32&
    operator()(uint32& index);
    
    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const int32
    operator[](uint32 index) const;

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    int32&
    operator[](uint32 index);

    /**
     * @brief + operator overload
     * @param b the other matrix to add
     * @return a vector sum of *this and v
     *
     */
    Vector3f
    operator+(const Vector3f& v) const;

    /**
     * @brief - operator overload
     * @param b the other matrix to substract
     * @return a vector difference of *this and v
     *
     */
    Vector3f
    operator-(const Vector3f& v) const;

    /**
     * @brief * operator overload
     * @param b the other matrix to multiply
     * @return a vector multiplication of *this times v
     *
     */
    Vector3f
    operator*(const Vector3f& v) const;

    /**
     * @brief / operator overload
     * @param b the other matrix to divide
     * @return a vector division of *this divided by v
     *
     */
    Vector3f
    operator/(const Vector3f& v) const;

    /**
     * @brief + operator overload
     * @param plus the float to add to all vector
     * @return a vector sum of *this + plus
     *
     */
    Vector3f
    operator+(float plus) const;

    /**
     * @brief - operator overload
     * @param minus the float to subtract to all vector
     * @return a vector difference of *this - minus
     *
     */
    Vector3f
    operator-(float minus) const;

    /**
     * @brief * operator overload
     * @param times the float to multiply all vector
     * @return a vector multiplication of *this times times
     *
     */
    Vector3f
    operator*(float times) const;

    /**
     * @brief / operator overload
     * @param under the float to divide all vector
     * @return a vector divided of *this divided by times
     *
     */
    Vector3f
    operator/(float under) const;

    /**
     * @brief | operator overload for dot product
     * @param the other vector
     * @return dot product
     *
     */
    float
    operator|(const Vector3f& v) const;

    /**
     * @brief ^ operator overload for cross product
     * @param the other vector
     * @return cross product
     *
     */
    Vector3f
    operator^(const Vector3f& v) const;

    /**
     * @brief == operator overload
     * @param the other vector to compare
     * @return true if this components are == to v components
     *
     */
    bool
    operator==(const Vector3f& v) const;

    /**
     * @brief != operator overload
     * @param the other vector to compare
     * @return true if this components are != to v components
     *
     */
    bool
    operator!=(const Vector3f& v) const;

    /**
     * @brief < operator overload
     * @param the other vector to compare
     * @return true if this components are < to v components
     *
     */
    bool
    operator<(const Vector3f& v) const;

    /**
     * @brief > operator overload
     * @param the other vector to compare
     * @return true if this components are > to v components
     *
     */
    bool
    operator>(const Vector3f& v) const;

    /**
     * @brief <= operator overload
     * @param the other vector to compare
     * @return true if this components are <= to v components
     *
     */
    bool
    operator<=(const Vector3f& v) const;

    /**
     * @brief >= operator overload
     * @param the other vector to compare
     * @return true if this components are >= to v components
     *
     */
    bool
    operator>=(const Vector3f& v) const;

    /**
     * @brief - operator overload
     * @param
     * @return the negative of the vector
     *
     */
    Vector3f
    operator-() const;

    /**
     * @brief += operator overload
     * @param the other vector to add
     * @return *this + v components
     *
     */
    Vector3f&
    operator+=(const Vector3f& v);

    /**
     * @brief -= operator overload
     * @param the other vector to subtract
     * @return *this - v components
     *
     */
    Vector3f&
    operator-=(const Vector3& v);

    /**
     * @brief *= operator overload
     * @param the other vector to multiply
     * @return *this * v components
     *
     */
    Vector3f&
    operator*=(const Vector3& v);

    /**
     * @brief /= operator overload
     * @param the other vector to divide
     * @return *this / v components
     *
     */
    Vector3f&
    operator/=(const Vector3& v);

    /**
     * @brief *= operator overload
     * @param the float to multiply with
     * @return *this * float
     *
     */
    Vector3f&
    operator*=(float scale);

    /**
     * @brief /= operator overload
     * @param the float to divide the vector
     * @return *this / float
     *
     */
    Vector3f&
    operator/=(float scale);

  private:

    float m_x;
    float m_y;
    float m_z;

  };
}
