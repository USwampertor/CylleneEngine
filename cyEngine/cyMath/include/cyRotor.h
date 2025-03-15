#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyQuaternion.h"
#include "cyVector4f.h"
#include "cyVector3f.h"


namespace CYLLENE_SDK {

using Bivector = Vector4f;


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

  Rotor 
  reverse() const;

  Rotor
  geometricProduct(const Rotor& other) const;

  void 
  normalize();

  Rotor
  normalized();

  Vector3f
  rotateVector(const Vector3f& vector);

public:

  Bivector m_bivector;

};

}

