#include "cyMatrix3.h"

#include "cyMath.h"
#include "cyMatrix2.h"
#include "cyMatrix4.h"
#include "cyQuaternion.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {
Matrix3::Matrix3(const float& value) {
  memset(this, static_cast<float>(value), sizeof(Matrix3));
}

Matrix3::Matrix3(const Matrix3& other)
  : _m(other._m) {}

Matrix3::Matrix3(const float& m00, const float& m10, const float& m20,
                  const float& m01, const float& m11, const float& m21,
                  const float& m02, const float& m12, const float& m22) {
  _m.m00 = m00; _m.m10 = m10; _m.m20 = m20;
  _m.m01 = m01; _m.m11 = m11; _m.m21 = m21;
  _m.m02 = m02; _m.m12 = m12; _m.m22 = m22;
}

Matrix3::Matrix3(const Matrix4& other) {
  _m.m00 = other._m.m00; _m.m10 = other._m.m10; _m.m20 = other._m.m20;
  _m.m01 = other._m.m01; _m.m11 = other._m.m11; _m.m21 = other._m.m21;
  _m.m02 = other._m.m02; _m.m12 = other._m.m12; _m.m22 = other._m.m22;
}

Matrix3::Matrix3(const Matrix2& other) {
  _m.m00 = other._m.m00; _m.m10 = other._m.m10; _m.m20 = 0;
  _m.m01 = other._m.m01; _m.m11 = other._m.m11; _m.m21 = 0;
  _m.m02 = 0;            _m.m12 = 0;            _m.m22 = 0;
}

Matrix3
Matrix3::operator+(const Matrix3& b) {
  Matrix3 temp;
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      temp.m[c][r] = m[c][r] + b.m[c][r];
    }
  }
  return temp;

}

Matrix3
Matrix3::operator-(const Matrix3& b) {
  Matrix3 temp;
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      temp.m[c][r] = m[c][r] - b.m[c][r];
    }
  }
  return temp;
}

Matrix3
Matrix3::operator*(const Matrix3& b) {
  Matrix3 tmp = ZERO;
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      tmp.m[c][r] = 0;
      for (uint32 k = 0; k < 3; ++k) {
        tmp.m[c][r] += m[k][r] * b.m[c][k];
      }
    }
  }
  return tmp;
}

Matrix3&
Matrix3::operator+=(const Matrix3& b) {
  // or (uint32 i = 0; i < 3; ++i) {
  //  for (uint32 j = 0; j < 3; ++j) {
  //    m[i][j] += b.m[i][j];
  //  }
  // 
  // return *this;

  *this = *this + b;
  return *this;
}

Matrix3&
Matrix3::operator-=(const Matrix3& b) {
  // for (uint32 i = 0; i < 3; ++i) {
  //   for (uint32 j = 0; j < 3; ++j) {
  //     m[i][j] -= b.m[i][j];
  //   }
  // }
  // return *this;

  *this = *this - b;
  return *this;
}

Matrix3&
Matrix3::operator*=(const Matrix3& b) {
  // Matrix3x3 temp = ZERO;
  // for (uint32 i = 0; i < 3; ++i) {
  //   for (uint32 j = 0; j < 3; ++j) {
  //     for (uint32 k = 0; k < 3; ++k) {
  //       temp.m[i][j] += m[i][k] * b.m[k][j];
  //     }
  //   }
  // }
  // *this = temp;
  // return *this;

  *this = *this * b;
  return *this;
}

Matrix3&
Matrix3::operator+=(const float& value) {
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      m[c][r] += value;
    }
  }
  return *this;
}

Matrix3&
Matrix3::operator-=(const float& value) {
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      m[c][r] -= value;
    }
  }
  return *this;
}

Matrix3&
Matrix3::operator*=(const float& value) {
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      m[c][r] *= value;
    }
  }
  return *this;
}

Matrix3&
Matrix3::operator/=(const float& value) {
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      m[c][r] /= value;
    }
  }
  return *this;
}

bool
Matrix3::operator==(const Matrix3& b) {
  for (uint32 c = 0; c < 3; ++c) {
    for (uint32 r = 0; r < 3; ++r) {
      if (!Math::isNearSame(m[c][r], b.m[c][r])) return false;
    }
  }
  return true;
}

void
Matrix3::identity() {
  *this = IDENTITY;
}

void
Matrix3::zero() {
  *this = ZERO;
}

Matrix3
Matrix3::transposed() const {
  return Matrix3(m[0][0], m[0][1], m[0][2],
                  m[1][0], m[1][1], m[1][2],
                  m[2][0], m[2][1], m[2][2]);
}

void
Matrix3::transpose() {
  *this = this->transposed();
}

Matrix3
Matrix3::cofactored() const {
  Matrix3 temp;

  temp.m[0][0] =  ((m[1][1] * m[2][2]) - (m[2][1] * m[1][2]));
  temp.m[0][1] = -((m[1][0] * m[2][2]) - (m[2][0] * m[1][2]));
  temp.m[0][2] =  ((m[1][0] * m[2][1]) - (m[2][0] * m[1][1]));
  temp.m[1][0] = -((m[0][1] * m[2][2]) - (m[2][1] * m[0][2]));
  temp.m[1][1] =  ((m[0][0] * m[2][2]) - (m[2][0] * m[0][2]));
  temp.m[1][2] = -((m[0][0] * m[2][1]) - (m[2][0] * m[0][1]));
  temp.m[2][0] =  ((m[0][1] * m[1][2]) - (m[1][1] * m[0][2]));
  temp.m[2][1] = -((m[0][0] * m[1][2]) - (m[1][0] * m[0][2]));
  temp.m[2][2] =  ((m[0][0] * m[1][1]) - (m[1][0] * m[0][1]));

  return temp;
}

void
Matrix3::cofactor() {
  *this = this->cofactored();
}

void
Matrix3::setValues(const float& value) {
//     m[0][0] = m[0][1] = m[0][2] = 
//     m[1][0] = m[1][1] = m[1][2] =
//     m[2][0] = m[2][1] = m[2][2] = value;

  memset(&_m, static_cast<float>(value), sizeof(_m));

}

void
Matrix3::setValues(const float& m00, const float& m10, const float& m20,
                    const float& m01, const float& m11, const float& m21,
                    const float& m02, const float& m12, const float& m22) {
  *this = Matrix3(m00, m10, m20,
                  m01, m11, m21,
                  m02, m12, m22);
}

Matrix3
Matrix3::inversed() {
  CY_ASSERT(this->determinant() != 0.0f &&
            Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
  Matrix3 temp = *this;
  temp.cofactor();
  temp.transpose();
  temp *= Math::pow(this->determinant(), -1.0f);
  return temp;
}

void
Matrix3::inverse() {
  *this = this->inversed();

}

const float
Matrix3::determinant() const {
  return (((m[0][0] * m[1][1] * m[2][2]) +
            (m[1][0] * m[2][1] * m[0][2]) + 
            (m[2][0] * m[0][1] * m[1][2])) -

          ((m[0][2] * m[1][1] * m[2][0]) + 
            (m[1][2] * m[2][1] * m[0][0]) + 
            (m[2][2] * m[0][1] * m[1][0])));
}

  
void
Matrix3::rotateX(const float& angle) {
  float c = Math::cos(Math::DEG2RAD * angle);
  float s = Math::sin(Math::DEG2RAD * angle);
  Matrix3 temp = *this;

  m[1][0] = c * temp.m[1][0] + s * temp.m[2][0]; m[2][1] = c * temp.m[2][0] - s * temp.m[1][0];
  m[1][1] = c * temp.m[1][1] + s * temp.m[2][1]; m[2][2] = c * temp.m[2][1] - s * temp.m[1][1];
  m[1][2] = c * temp.m[1][2] + s * temp.m[2][2]; m[2][3] = c * temp.m[2][2] - s * temp.m[1][2];
  

}

void
Matrix3::rotateY(const float& angle) {

  float c = Math::cos(angle * Math::DEG2RAD);
  float s = Math::sin(angle * Math::DEG2RAD);
  Matrix3 temp = *this;

#if HANDSYSTEM == HANDSYS_LH
  m[0][0] = temp.m[0][0] * c + temp.m[2][0] * s; m[2][0] =  temp.m[2][0] * c - temp.m[0][0] * s;
  m[0][1] = temp.m[0][1] * c + temp.m[2][1] * s; m[2][2] =  temp.m[2][1] * c - temp.m[0][1] * s;
  m[0][2] = temp.m[0][2] * c + temp.m[2][2] * s; m[2][1] =  temp.m[2][2] * c - temp.m[0][2] * s;
#elif HANDSYSTEM == HANDSYS_RH
  m[0][0] = temp.m[0][0] * c - temp.m[2][0] * s; m[2][0] = temp.m[2][0] * c + temp.m[0][0] * s;
  m[0][1] = temp.m[0][1] * c - temp.m[2][1] * s; m[2][2] = temp.m[2][1] * c + temp.m[0][1] * s;
  m[0][2] = temp.m[0][2] * c - temp.m[2][2] * s; m[2][1] = temp.m[2][2] * c + temp.m[0][2] * s;
#endif 

}

void
Matrix3::rotateZ(const float& angle) {

  float s = Math::sin(angle * Math::DEG2RAD);
  float c = Math::cos(angle * Math::DEG2RAD);
  Matrix3 temp = *this;

#if HANDSYSTEM == HANDSYS_LH
  m[0][0] = c * temp.m[0][0] + s * temp.m[1][0]; m[1][0] = c * temp.m[1][0] - s * temp.m[0][0];
  m[0][1] = c * temp.m[0][1] + s * temp.m[1][1]; m[1][1] = c * temp.m[1][1] - s * temp.m[0][1];
  m[0][2] = c * temp.m[0][2] + s * temp.m[1][2]; m[1][2] = c * temp.m[1][2] - s * temp.m[0][2];
#elif HANDSYSTEM == HANDSYS_RH
  m[0][0] = c * temp.m[0][0] - s * temp.m[1][0]; m[1][0] = c * temp.m[1][0] + s * temp.m[0][0];
  m[0][1] = c * temp.m[0][1] - s * temp.m[1][1]; m[1][1] = c * temp.m[1][1] + s * temp.m[0][1];
  m[0][2] = c * temp.m[0][2] - s * temp.m[1][2]; m[1][2] = c * temp.m[1][2] + s * temp.m[0][2];
#endif
}

void
Matrix3::rotate(const float& angle, const Vector3f& axis) {

  float cos = Math::cos(Math::DEG2RAD * angle);
  float sin = Math::sin(Math::DEG2RAD * angle);
  float omc = 1.0f - cos;  // "One minus cos"
  float x = axis.x, y = axis.y, z = axis.z;
  // Normalize the axis vector
  float length = sqrt(x * x + y * y + z * z);
  if (length != 1.0f && length != 0.0f) {
    x /= length; y /= length; z /= length;
  }

  Matrix3 rot;

#if HANDSYSTEM == HANDSYS_LH
  rot.m[0][0] = cos + x * x * omc;     rot.m[1][0] = x * y * omc - z * sin; rot.m[2][0] = x * z * omc + y * sin;
  rot.m[0][1] = y * x * omc + z * sin; rot.m[1][1] = cos + y * y * omc;     rot.m[2][1] = y * z * omc - x * sin;
  rot.m[0][2] = z * x * omc - y * sin; rot.m[1][2] = z * y * omc + x * sin; rot.m[2][2] = cos + z * z * omc;
#elif HANDSYSTEM == HANDSYS_RH
  rot.m[0][0] = cos + x * x * omc;     rot.m[1][0] = x * y * omc + z * sin; rot.m[2][0] = x * z * omc - y * sin;
  rot.m[0][1] = y * x * omc - z * sin; rot.m[1][1] = cos + y * y * omc;     rot.m[2][1] = y * z * omc + x * sin;
  rot.m[0][2] = z * x * omc + y * sin; rot.m[1][2] = z * y * omc - x * sin; rot.m[2][2] = cos + z * z * omc;
#endif
  *this = *this * rot;

}


void
Matrix3::rotate(const float& angle, const float& x, const float& y, const float& z) {
  return rotate(angle, Vector3f(x, y, z));
}

void
Matrix3::rotate(const Quaternion& rotation) {
  Matrix3 rotationMatrix = rotation.getMatrix3Rotation();
  *this *= rotationMatrix;
}

void
Matrix3::setRotation(const Vector3f& rotation) {
  Quaternion q;
  q.fromEuler(Euler(rotation));
  Matrix3 temp = q.getMatrix3Rotation();
  *this = temp;
}

void
Matrix3::setRotation(const Quaternion& rotation) {
  *this = rotation.getMatrix3Rotation();
}

void
Matrix3::setScale(const float& newScale) {
  m[0][0] = newScale;
  m[1][1] = newScale;
  m[2][2] = newScale;
}

void
Matrix3::setScale(const Vector3f& scale) {
  m[0][0] = scale.x;
  m[1][1] = scale.y;
  m[2][2] = scale.z;
}

void
Matrix3::scale(const Vector3f& scale) {
  m[0][0] *= scale.x;
  m[1][1] *= scale.y;
  m[2][2] *= scale.z;
}

void
Matrix3::scale(const float& scale) {
  m[0][0] *= scale;
  m[1][1] *= scale;
  m[2][2] *= scale;
}

Vector3f
Matrix3::getRightVector() const {
  return Vector3f(m[0][0], 
                  m[0][1], 
                  m[0][2]).normalized();
}

Vector3f
Matrix3::getUpVector() const {
  return Vector3f(m[1][0], 
                  m[1][1], 
                  m[1][2]).normalized();
}

Vector3f
Matrix3::getForwardVector() const {
  return Vector3f(m[2][0], 
                  m[2][1], 
                  m[2][2]).normalized();
}



String
Matrix3::toString() {
  return Utils::format("[(%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f)] \n", 
                        m[0][0], m[0][1], m[0][2],
                        m[1][0], m[1][1], m[1][2],
                        m[2][0], m[2][1], m[2][2]);
}

const Matrix3 Matrix3::ZERO     = Matrix3(0.0f);

const Matrix3 Matrix3::IDENTITY = Matrix3(1.0f, 0.0f, 0.0f, 
                                          0.0f, 1.0f, 0.0f, 
                                          0.0f, 0.0f, 1.0f);
}
