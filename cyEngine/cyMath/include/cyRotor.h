/**
 * @file cyRotor.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Rotor.
 */
#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyQuaternion.h"
#include "cyVector4f.h"
#include "cyVector3f.h"


namespace CYLLENE_SDK {

using Bivector = Vector4f;

/**
 * @class Rotor
 * @brief Geometric-algebra rotor used to rotate 3D vectors.
 */
class CY_MATH_EXPORT Rotor
{
public:

  Rotor() = default;
  
  Rotor(const Rotor& other)
    : m_bivector(other.m_bivector) {}

  Rotor(const Vector3f& bivector)
    : m_bivector(bivector, 0.0f) {}

  Rotor(const Vector4f& bivector)
    : m_bivector(bivector) {}

  Rotor(const Quaternion& bivector) {
    m_bivector.x = bivector.x;
    m_bivector.y = bivector.y;
    m_bivector.z = bivector.z;
    m_bivector.w = bivector.w;
  }

  Rotor(const float& x, const float& y, const float& z, const float& w)
    : m_bivector(x,y,z,w) {}

  /**
   * @brief Returns the reverse of this rotor.
   * @return Reversed rotor.
   */
  Rotor 
  reverse() const;

  /**
   * @brief Computes geometric product between two rotors.
   * @param other Rotor multiplied with this one.
   * @return Product rotor.
   */
  Rotor
  geometricProduct(const Rotor& other) const;

  /**
   * @brief Normalizes this rotor in place.
   */
  void 
  normalize();

  /**
   * @brief Returns a normalized copy of this rotor.
   * @return Normalized rotor.
   */
  Rotor
  normalized();

  /**
   * @brief Rotates a vector by this rotor.
   * @param vector Vector to rotate.
   * @return Rotated vector.
   */
  Vector3f
  rotateVector(const Vector3f& vector);

public:

  /**
   * @brief Bivector components that define the rotor.
   */
  Bivector m_bivector;

};

}


