#include "cyRotor.h"

namespace CYLLENE_SDK {

void
Rotor::normalize() {
  *this = this->normalized();
}

Rotor
Rotor::normalized() {

  Rotor tmp = *this;
  
  float norm = Math::sqrt(tmp.m_bivector.w * tmp.m_bivector.w +
                          tmp.m_bivector.x * tmp.m_bivector.x +
                          tmp.m_bivector.y * tmp.m_bivector.y +
                          tmp.m_bivector.z * tmp.m_bivector.z);
  tmp.m_bivector.w /= norm;
  tmp.m_bivector.x /= norm;
  tmp.m_bivector.y /= norm;
  tmp.m_bivector.z /= norm;
  return tmp;
}

Rotor
Rotor::reverse() const {
  return Rotor(-m_bivector.x, -m_bivector.y, -m_bivector.z, m_bivector.w);
}

Rotor
Rotor::geometricProduct(const Rotor& other) const {
  Rotor result;
  result.m_bivector.w = m_bivector.w * other.m_bivector.w - 
                        m_bivector.x * other.m_bivector.x - 
                        m_bivector.y * other.m_bivector.y - 
                        m_bivector.z * other.m_bivector.z;

  result.m_bivector.x = m_bivector.w * other.m_bivector.x + 
                        m_bivector.x * other.m_bivector.w + 
                        m_bivector.y * other.m_bivector.z - 
                        m_bivector.z * other.m_bivector.y;
  
  result.m_bivector.y = m_bivector.w * other.m_bivector.y + 
                        m_bivector.y * other.m_bivector.w + 
                        m_bivector.z * other.m_bivector.x - 
                        m_bivector.x * other.m_bivector.z;
  
  result.m_bivector.z = m_bivector.w * other.m_bivector.z + 
                        m_bivector.z * other.m_bivector.w + 
                        m_bivector.x * other.m_bivector.y - 
                        m_bivector.y * other.m_bivector.x;
  return result;
}

Vector3f
Rotor::rotateVector(const Vector3f& vector) {
  Vector3f result;
  Rotor vectorRotor(vector);

  // Compute the rotated vector: v' = R v R~
  Rotor rotatedVectorRotor = geometricProduct(vectorRotor).geometricProduct(reverse());

  // Extract the rotated vector components
  result.x = rotatedVectorRotor.m_bivector.x;
  result.y = rotatedVectorRotor.m_bivector.y;
  result.z = rotatedVectorRotor.m_bivector.z;
  return result;
}


}
