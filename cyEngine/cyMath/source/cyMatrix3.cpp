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

Matrix3::Matrix3(const float& m00, const float& m10, const float& m20,
                 const float& m01, const float& m11, const float& m21,
                 const float& m02, const float& m12, const float& m22) {
  _m.m00 = m00; _m.m10 = m10; _m.m20 = m20;
  _m.m01 = m01; _m.m11 = m11; _m.m21 = m21;
  _m.m02 = m02; _m.m12 = m12; _m.m22 = m22;
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
Matrix3::setRotation(const Vector3f& rotation) {
  Quaternion q = Quaternion(Euler(rotation));

  Matrix3 R = q.getMatrix3Rotation();
  Matrix3 S = getScaleMatrix();

  *this = R * S;
}

void
Matrix3::setRotation(const Quaternion& rotation) {
  Matrix3 R = rotation.getMatrix3Rotation();
  Matrix3 S = getScaleMatrix();

  *this = R * S;
}

void
Matrix3::setScale(const float& newScale) {
  Matrix3 R = getRotationMatrix();
  Matrix3 S = Matrix3::IDENTITY;

  S.m[0][0] = newScale;
  S.m[1][1] = newScale;
  S.m[2][2] = newScale;

  *this = R * S;
}

void
Matrix3::setScale(const Vector3f& scale) {
  Matrix3 R = getRotationMatrix();
  Matrix3 S = Matrix3::IDENTITY;

  S.m[0][0] = scale.x;
  S.m[1][1] = scale.y;
  S.m[2][2] = scale.z;

  *this = R * S;
}

void
Matrix3::rotateX(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(angle, 0.0f, 0.0f));

  setRotation(q * t);
}

void
Matrix3::rotateY(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(0.0f, angle, 0.0f));

  setRotation(q * t);
}

void
Matrix3::rotateZ(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(0.0f, 0.0f, angle));

  setRotation(q * t);
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

  Quaternion t(this->getRotationMatrix());
  Quaternion q(rot);

  setRotation(q * t);
}

void
Matrix3::rotate(const float& angle, const float& x, const float& y, const float& z) {
  return rotate(angle, Vector3f(x, y, z));
}

void
Matrix3::rotate(const Quaternion& rotation) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(rotation);

  setRotation(q * t);
}

void
Matrix3::scale(const Vector3f& scale) {
  m[0][0] *= scale.x;
  m[1][1] *= scale.y;
  m[2][2] *= scale.z;
}

void
Matrix3::scale(const float& x, const float& y, const float& z) {
  m[0][0] *= x;
  m[1][1] *= y;
  m[2][2] *= z;
}

void
Matrix3::scale(const float& scale) {
  m[0][0] *= scale;
  m[1][1] *= scale;
  m[2][2] *= scale;
}

Vector3f
Matrix3::getForwardVector() const {
  return Vector3f(m[2][0],
                  m[2][1],
                  m[2][2]).normalized();
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

Euler
Matrix3::getEulerRotation() const {
  return Euler(*this);
}

Quaternion
Matrix3::getQuatRotation() const {
  return Quaternion(*this);
}

Vector3f
Matrix3::getScale() const {
  return Vector3f(std::sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
                  std::sqrt(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
                  std::sqrt(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]));
}

Matrix3
Matrix3::getRotationMatrix() const {
  const Vector3f& scale = getScale();

  Matrix4 rotationMatrix = Matrix4::IDENTITY;

  CY_ASSERT(!Math::isNearSame(scale.x, 0.0f) &&
            !Math::isNearSame(scale.y, 0.0f) &&
            !Math::isNearSame(scale.z, 0.0f) &&
            Utils::format("Trying to get rotation matrix with a scale component of 0").c_str());

  rotationMatrix.m[0][0] /= scale.x;
  rotationMatrix.m[0][1] /= scale.x;
  rotationMatrix.m[0][2] /= scale.x;

  rotationMatrix.m[1][0] /= scale.y;
  rotationMatrix.m[1][1] /= scale.y;
  rotationMatrix.m[1][2] /= scale.y;

  rotationMatrix.m[2][0] /= scale.z;
  rotationMatrix.m[2][1] /= scale.z;
  rotationMatrix.m[2][2] /= scale.z;

  return rotationMatrix;
}

Matrix3
Matrix3::getScaleMatrix() const {
  Matrix4 rotationMatrix = Matrix4::IDENTITY;
  const Vector3f& scale = getScale();

  CY_ASSERT(!Math::isNearSame(scale.x, 0.0f) &&
            !Math::isNearSame(scale.y, 0.0f) &&
            !Math::isNearSame(scale.z, 0.0f) &&
            Utils::format("Trying to get scale matrix with a scale component of 0").c_str());

  rotationMatrix.m[0][0] = scale.x;
  rotationMatrix.m[1][1] = scale.y;
  rotationMatrix.m[2][2] = scale.z;

  return rotationMatrix;
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
