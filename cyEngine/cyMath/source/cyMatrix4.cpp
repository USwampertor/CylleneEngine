#include "cyMatrix4.h"
#include "cyUtilities.h"

#include "cyMatrix2.h"
#include "cyMatrix3.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

Matrix4::Matrix4(const float& value) {
  memset(&_m, static_cast<float>(value), sizeof(_m));

  if (0 != value) {
    _m.m00 = _m.m11 = _m.m22 = _m.m33 = 1.0f;
  }
}

Matrix4::Matrix4(const Matrix4& other)
  : _m(other._m) {}

Matrix4::Matrix4(const Matrix3& other) {
  m[0][0] = other.m[0][0]; m[1][0] = other.m[1][0]; m[2][0] = other.m[2][0]; m[3][0] = 0;
  m[0][1] = other.m[0][1]; m[1][1] = other.m[1][1]; m[2][1] = other.m[2][1]; m[3][1] = 0;
  m[0][2] = other.m[0][2]; m[1][2] = other.m[1][2]; m[2][2] = other.m[2][2]; m[3][2] = 0;
  m[0][3] = 0;             m[1][3] = 0;             m[2][3] = 0;             m[3][3] = 0;
}

Matrix4::Matrix4(const Matrix2& other) {
  m[0][0] = other.m[0][0]; m[1][0] = other.m[1][0]; m[2][0] = 0; m[3][0] = 0;
  m[0][1] = other.m[0][1]; m[1][1] = other.m[1][1]; m[2][1] = 0; m[3][1] = 0;
  m[0][2] = 0;             m[1][2] = 0;             m[2][2] = 0; m[3][2] = 0;
  m[0][3] = 0;             m[1][3] = 0;             m[2][3] = 0; m[3][3] = 0;
}

Matrix4::Matrix4(const float& m00, const float& m10, const float& m20, const float& m30,   // Row 0
                 const float& m01, const float& m11, const float& m21, const float& m31,   // Row 1
                 const float& m02, const float& m12, const float& m22, const float& m32,   // Row 2
                 const float& m03, const float& m13, const float& m23, const float& m33) { // Row 3
  m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
  m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
  m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
  m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
}

Matrix4
Matrix4::operator+(const Matrix4& b) {
  Matrix4 temp;
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      temp.m[c][r] = m[c][r] + b.m[c][r];
    }
  }
  return temp;

}

Matrix4
Matrix4::operator-(const Matrix4& b) {
  Matrix4 temp;
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      temp.m[c][r] = m[c][r] - b.m[c][r];
    }
  }
  return temp;
}

Matrix4
Matrix4::operator*(const Matrix4& b) {
  Matrix4 tmp;
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      tmp.m[c][r] = 0;
      for (int k = 0; k < 4; k++) {
        tmp.m[c][r] += m[k][r] * b.m[c][k];
      }
    }
  }
  return tmp;
}

Matrix4&
Matrix4::operator+=(const Matrix4& b) {
  // for (uint32 c = 0; c < 4; ++c) {
  //   for (uint32 r = 0; r < 4; ++r) {
  //     m[c][r] += b.m[c][r];
  //   }
  // }
  // return *this;

  *this = *this + b;
  return *this;
}

Matrix4&
Matrix4::operator-=(const Matrix4& b) {
  // for (uint32 c = 0; c < 4; ++c) {
  //   for (uint32 r = 0; r < 4; ++r) {
  //     m[c][r] -= b.m[c][r];
  //   }
  // }
  // return *this;

  *this = *this - b;
  return *this;
}

Matrix4&
Matrix4::operator*=(const Matrix4& b) {
  *this = *this * b;
  return *this;
}

Matrix4&
Matrix4::operator+=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] += value;
    }
  }
  return *this;
}

Matrix4&
Matrix4::operator-=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] -= value;
    }
  }
  return *this;
}

Matrix4&
Matrix4::operator*=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] *= value;
    }
  }
  return *this;
}

Matrix4&
Matrix4::operator/=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] /= value;
    }
  }
  return *this;
}

bool
Matrix4::operator==(const Matrix4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      if (!Math::isNearSame(m[c][r], b.m[c][r])) return false;
    }
  }
  return true;
}

void
Matrix4::identity() {
  *this = IDENTITY;
}

void
Matrix4::zero() {
  *this = ZERO;
}

Matrix4
Matrix4::transposed() const {
  return Matrix4(m[0][0], m[0][1], m[0][2], m[0][3],
                 m[1][0], m[1][1], m[1][2], m[1][3],
                 m[2][0], m[2][1], m[2][2], m[2][3],
                 m[3][0], m[3][1], m[3][2], m[3][3]);
}

void
Matrix4::transpose() {
  *this = this->transposed();
}

Matrix4
Matrix4::cofactored() const {
  Matrix4 temp;

  temp.m[0][0] = m[1][1]*m[2][2]*m[3][3] + m[1][2]*m[2][3]*m[3][1] +
                 m[1][3]*m[2][1]*m[3][2] - m[1][1]*m[2][3]*m[3][2] -
                 m[1][2]*m[2][1]*m[3][3] - m[1][3]*m[2][2]*m[3][1];
  temp.m[1][0] = m[0][1]*m[2][3]*m[3][2] + m[0][2]*m[2][1]*m[3][3] +
                 m[0][3]*m[2][2]*m[3][1] - m[0][1]*m[2][2]*m[3][3] -
                 m[0][2]*m[2][3]*m[3][1] - m[0][3]*m[2][1]*m[3][2];
  temp.m[2][0] = m[0][1]*m[1][2]*m[3][3] + m[0][2]*m[1][3]*m[3][1] +
                 m[0][3]*m[1][1]*m[3][2] - m[0][1]*m[1][3]*m[3][2] -
                 m[0][2]*m[1][1]*m[3][3] - m[0][3]*m[1][2]*m[3][1];
  temp.m[3][0] = m[0][1]*m[1][3]*m[2][2] + m[0][2]*m[1][1]*m[2][3] +
                 m[0][3]*m[1][2]*m[2][1] - m[0][1]*m[1][2]*m[2][3] -
                 m[0][2]*m[1][3]*m[2][1] - m[0][3]*m[1][1]*m[2][2];
  temp.m[0][1] = m[1][0]*m[2][3]*m[3][2] + m[1][2]*m[2][0]*m[3][3] +
                 m[1][3]*m[2][2]*m[3][0] - m[1][0]*m[2][2]*m[3][3] -
                 m[1][2]*m[2][3]*m[3][0] - m[1][3]*m[2][0]*m[3][2];
  temp.m[1][1] = m[0][0]*m[2][2]*m[3][3] + m[0][2]*m[2][3]*m[3][0] +
                 m[0][3]*m[2][0]*m[3][2] - m[0][0]*m[2][3]*m[3][2] -
                 m[0][2]*m[2][0]*m[3][3] - m[0][3]*m[2][2]*m[3][0];
  temp.m[2][1] = m[0][0]*m[1][3]*m[3][2] + m[0][2]*m[1][0]*m[3][3] +
                 m[0][3]*m[1][2]*m[3][0] - m[0][0]*m[1][2]*m[3][3] -
                 m[0][2]*m[1][3]*m[3][0] - m[0][3]*m[1][0]*m[3][2];
  temp.m[3][1] = m[0][0]*m[1][2]*m[2][3] + m[0][2]*m[1][3]*m[2][0] +
                 m[0][3]*m[1][0]*m[2][2] - m[0][0]*m[1][3]*m[2][2] -
                 m[0][2]*m[1][0]*m[2][3] - m[0][3]*m[1][2]*m[2][0];
  temp.m[0][2] = m[1][0]*m[2][1]*m[3][3] + m[1][1]*m[2][3]*m[3][0] +
                 m[1][3]*m[2][0]*m[3][1] - m[1][0]*m[2][3]*m[3][1] -
                 m[1][1]*m[2][0]*m[3][3] - m[1][3]*m[2][1]*m[3][0];
  temp.m[1][2] = m[0][0]*m[2][3]*m[3][1] + m[0][1]*m[2][0]*m[3][3] +
                 m[0][3]*m[2][1]*m[3][0] - m[0][0]*m[2][1]*m[3][3] -
                 m[0][1]*m[2][3]*m[3][0] - m[0][3]*m[2][0]*m[3][1];
  temp.m[2][2] = m[0][0]*m[1][1]*m[3][3] + m[0][1]*m[1][3]*m[3][0] +
                 m[0][3]*m[1][0]*m[3][1] - m[0][0]*m[1][3]*m[3][1] -
                 m[0][1]*m[1][0]*m[3][3] - m[0][3]*m[1][1]*m[3][0];
  temp.m[3][2] = m[0][0]*m[1][3]*m[2][1] + m[0][1]*m[1][0]*m[2][3] +
                 m[0][3]*m[1][1]*m[2][0] - m[0][0]*m[1][1]*m[2][3] -
                 m[0][1]*m[1][3]*m[2][0] - m[0][3]*m[1][0]*m[2][1];
  temp.m[0][3] = m[1][0]*m[2][2]*m[3][1] + m[1][1]*m[2][0]*m[3][2] +
                 m[1][2]*m[2][1]*m[3][0] - m[1][0]*m[2][1]*m[3][2] -
                 m[1][1]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][1];
  temp.m[1][3] = m[0][0]*m[2][1]*m[3][2] + m[0][1]*m[2][2]*m[3][0] +
                 m[0][2]*m[2][0]*m[3][1] - m[0][0]*m[2][2]*m[3][1] -
                 m[0][1]*m[2][0]*m[3][2] - m[0][2]*m[2][1]*m[3][0];
  temp.m[2][3] = m[0][0]*m[1][2]*m[3][1] + m[0][1]*m[1][0]*m[3][2] +
                 m[0][2]*m[1][1]*m[3][0] - m[0][0]*m[1][1]*m[3][2] -
                 m[0][1]*m[1][2]*m[3][0] - m[0][2]*m[1][0]*m[3][1];
  temp.m[3][3] = m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] +
                 m[0][2]*m[1][0]*m[2][1] - m[0][0]*m[1][2]*m[2][1] -
                 m[0][1]*m[1][0]*m[2][2] - m[0][2]*m[1][1]*m[2][0];
  return temp;
}

void
Matrix4::cofactor() {
  *this = this->cofactored();
}

void
Matrix4::setValues(const float& value) {
  memset(&_m, static_cast<float>(value), sizeof(_m));

  // m[0][0] = m[0][1] = m[0][2] = m[0][3] = 
  // m[1][0] = m[1][1] = m[1][2] = m[1][3] =
  // m[2][0] = m[2][1] = m[2][2] = m[2][3] =
  // m[3][0] = m[3][1] = m[3][2] = m[3][3] = value;
}

void
Matrix4::setValues(const float& m00, const float& m10, const float& m20, const float& m30,  // Row 0
                   const float& m01, const float& m11, const float& m21, const float& m31,  // Row 1
                   const float& m02, const float& m12, const float& m22, const float& m32,  // Row 2
                   const float& m03, const float& m13, const float& m23, const float& m33) {// Row 3
  *this = Matrix4(m00, m10, m20, m30,
                  m01, m11, m21, m31,
                  m02, m12, m22, m32,
                  m03, m13, m23, m33);
}

void
Matrix4::setTransformMatrix(const Vector3f& position, 
                            const Quaternion& rotation, 
                            const Vector3f& scale) {
  Matrix4 T = Matrix4::IDENTITY;
  T.setPosition(position);

  Matrix4 R = rotation.getMatrix4Rotation();

  Matrix4 S = Matrix4::IDENTITY;
  S.setScale(scale);

  *this = T * (R * S);
}

// TODO: This is not the correct way to set the transform matrix
void
Matrix4::setTransformMatrix(const Vector3f& position,
                            const Rotor& rotation) {
  float pitchSin  = Math::sin(Math::degToRad(rotation.m_bivector.x));
  float yawSin    = Math::sin(Math::degToRad(rotation.m_bivector.y));
  float rollSin   = Math::sin(Math::degToRad(rotation.m_bivector.z));

  float pitchCos  = Math::cos(Math::degToRad(rotation.m_bivector.x));
  float yawCos    = Math::cos(Math::degToRad(rotation.m_bivector.y));
  float rollCos   = Math::cos(Math::degToRad(rotation.m_bivector.z));

  m[0][0] = pitchCos * yawCos;
  m[0][1] = pitchCos * yawSin;
  m[0][2] = pitchSin;
  m[0][3] = 0.f;

  m[1][0] = rollSin * pitchSin * yawCos - rollCos * yawSin;
  m[1][1] = rollSin * pitchSin * yawSin + rollCos * yawCos;
  m[1][2] = -rollSin * pitchCos;
  m[1][3] = 0.f;

  m[2][0] = -(rollCos * pitchSin * yawCos + rollSin * yawSin);
  m[2][1] = yawCos * rollSin - rollCos * pitchSin * yawSin;
  m[2][2] = rollCos * pitchCos;
  m[2][3] = 0.f;

  m[3][0] = position.x;
  m[3][1] = position.y;
  m[3][2] = position.z;
  m[3][3] = 1.f;
}

void
Matrix4::setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  CY_ASSERT(false, "Not implemented yet!");
}

Matrix4
Matrix4::inversed() {
  CY_ASSERT(this->determinant() != 0.0f &&
            Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    
  Matrix4 temp = *this;
  temp.cofactor();
  temp.transpose();

  temp *= Math::pow(this->determinant(), -1.0f);
  return temp;
}

void
Matrix4::inverse() {
  *this = this->inversed();
}

const float
Matrix4::determinant() const {
  float a;
  a = (m[0][3] * m[1][2] * m[2][1] * m[3][0]) - (m[0][2] * m[1][3] * m[2][1] * m[3][0]) -
      (m[0][3] * m[1][1] * m[2][2] * m[3][0]) + (m[0][1] * m[1][3] * m[2][2] * m[3][0]) +
      (m[0][2] * m[1][1] * m[2][3] * m[3][0]) - (m[0][1] * m[1][2] * m[2][3] * m[3][0]) -
      (m[0][3] * m[1][2] * m[2][0] * m[3][1]) + (m[0][2] * m[1][3] * m[2][0] * m[3][1]) +
      (m[0][3] * m[1][0] * m[2][2] * m[3][1]) - (m[0][0] * m[1][3] * m[2][2] * m[3][1]) -
      (m[0][2] * m[1][0] * m[2][3] * m[3][1]) + (m[0][0] * m[1][2] * m[2][3] * m[3][1]) +
      (m[0][3] * m[1][1] * m[2][0] * m[3][2]) - (m[0][1] * m[1][3] * m[2][0] * m[3][2]) -
      (m[0][3] * m[1][0] * m[2][1] * m[3][2]) + (m[0][0] * m[1][3] * m[2][1] * m[3][2]) +
      (m[0][1] * m[1][0] * m[2][3] * m[3][2]) - (m[0][0] * m[1][1] * m[2][3] * m[3][2]) -
      (m[0][2] * m[1][1] * m[2][0] * m[3][3]) + (m[0][1] * m[1][2] * m[2][0] * m[3][3]) +
      (m[0][2] * m[1][0] * m[2][1] * m[3][3]) - (m[0][0] * m[1][2] * m[2][1] * m[3][3]) -
      (m[0][1] * m[1][0] * m[2][2] * m[3][3]) + (m[0][0] * m[1][1] * m[2][2] * m[3][3]);
  return a;
}
  
Matrix4&
Matrix4::view(const Vector4f& eye,
              const Vector4f& target,
              const Vector4f& worldUp) {
#if HANDSYSTEM == HANDSYS_LH
  Vector3f front = (target - eye).normalized();
#elif HANDSYSTEM == HANDSYS_RH
  Vector3f front = (eye - target).normalized();
#endif

  Vector3f right = Vector3f::cross(worldUp, front).normalized();
  Vector3f up = Vector3f::cross(front, right).normalized();

#if HANDSYSTEM == HANDSYS_LH
  float A = -Vector3f::dot(right, eye);
  float B = -Vector3f::dot(up, eye);
  float C = -Vector3f::dot(front, eye);
#elif HANDSYSTEM == HANDSYS_RH
  float A = Vector3f::dot(right, eye);
  float B = Vector3f::dot(up, eye);
  float C = Vector3f::dot(front, eye);
#endif

  m[0][0] = right.x; m[1][0] = up.x; m[2][0] = front.x; m[3][0] = 0.0f;
  m[0][1] = right.y; m[1][1] = up.y; m[2][1] = front.y; m[3][1] = 0.0f;
  m[0][2] = right.z; m[1][2] = up.z; m[2][2] = front.z; m[3][2] = 0.0f;
  m[0][3] = A;       m[1][3] = B;    m[2][3] = C;       m[3][3] = 1.0f;

  return *this;
}
  
Matrix4&
Matrix4::orthogonal(const float& width,
                    const float& height,
                    const float& zNear,
                    const float& zFar) {
  (*this) = Matrix4::ZERO;

  m[0][0] = 2.0f / width;
  m[1][1] = 2.0f / height;

#if GAPI_MATHTYPE == GAPI_GL
#if HANDSYSTEM == HANDSYS_LH
  m[2][2] = 2.0f / (zFar - zNear);
#elif HANDSYSTEM == HANDSYS_RH
  m[2][2] = -2.0f / (zNear - zFar);
#endif

  // TODO: Missing M[3][0] ???
  // TODO: Missing M[3][1] ???

  m[3][0] = 1; // -(right+left)/(right-left);
  m[3][1] = 1; // -(top+bottom)/(top-bottom);
  m[3][2] = -(zFar + zNear) / (zFar - zNear);

#elif GAPI_MATHTYPE == GAPI_DX
#if HANDSYSTEM == HANDSYS_LH
  m[2][2] = 1.0f / (zFar - zNear);
#elif HANDSYSTEM == HANDSYS_RH
  m[2][2] = 1.0f / (zNear - zFar);
#endif

  m[3][2] = zNear / (zNear - zFar);
#endif

  m[3][3] = 1.0f;

  return *this;
}

Matrix4&
Matrix4::orthogonal(const float& top,
                    const float& bottom,
                    const float& left,
                    const float& right,
                    const float& zNear,
                    const float& zFar) {
  (*this) = Matrix4::ZERO;
  float width = right - left;
  float height = top - bottom;
  m[0][0] = 2.0f / width;
  m[1][1] = 2.0f / height;

#if GAPI_MATHTYPE == GAPI_GL
#if HANDSYSTEM == HANDSYS_LH
  m[2][2] = 2.0f / (zFar - zNear);
#elif HANDSYSTEM == HANDSYS_RH
  m[2][2] = -2.0f / (zNear - zFar);
#endif

  // TODO: Missing M[3][0] ???
  // TODO: Missing M[3][1] ???

  m[3][0] = -(right + left) / (right - left);
  m[3][1] = -(top + bottom) / (top - bottom);
  m[3][2] = -(zFar + zNear) / (zFar - zNear);

#elif GAPI_MATHTYPE == GAPI_DX
#if HANDSYSTEM == HANDSYS_LH
  m[2][2] = 1.0f / (zFar - zNear);
#elif HANDSYSTEM == HANDSYS_RH
  m[2][2] = 1.0f / (zNear - zFar);
#endif

  m[3][2] = zNear / (zNear - zFar);
#endif

  m[3][3] = 1.0f;

  return *this;
}

Matrix4&
Matrix4::perspective(const float width,
                     const float height,
                     const float zNear,
                     const float zFar,
                     const float FOVangle) {
  const float aspect = width / height;

  const float FOVrads = Math::DEG2RAD * FOVangle;
  const float halfFOVrads = FOVrads * 0.5f;

  const float rangeInv = 1.0f / (zFar - zNear);

#if GAPI_MATHTYPE == GAPI_GL
  const float f = 1.0f / std::tanf(halfFOVrads);

  *this = Matrix4(f / aspect,      0.0f,    0.0f,                              0.0f,
                  0.0f,            f,       0.0f,                              0.0f,
                  0.0f,            0.0f,    (zFar + zNear) * rangeInv,        -1.0f,
                  0.0f,            0.0f,    (2.0f * zFar * zNear) * rangeInv,  0.0f);
#elif GAPI_MATHTYPE == GAPI_DX
  const float f = 1.0f / std::tanf(halfFOVrads);

  *this = Matrix4(f / aspect,     0.0f,        0.0f,                       0.0f,
                  0.0f,           f,           0.0f,                       0.0f,
                  0.0f,           0.0f,        zFar * rangeInv,            1.0f,
                  0.0f,           0.0f,       -zNear * zFar * rangeInv,    0.0f);
#endif
  return *this;
//   float FOVrads = Math::DEG2RAD * FOVangle;
// 
//   float halfFOVrads = FOVrads * 0.5;
//   // float plane0[4] = { 1.0f / std::tanf(halfFOV), 0.0f,                                0.0f,                           0.0f };
//   // float plane1[4] = { 0.0f,                      width / std::tanf(halfFOV) / height, 0.0f,                           0.0f };
//   // float plane2[4] = { 0.0f,                      0.0f,                                zFar / (zFar - zNear),          1.0f };
//   // float plane3[4] = { 0.0f,                      0.0f,                                -zNear * zFar / (zFar - zNear), 0.0f };
//   // columns[0] = { plane0[0], plane0[1], plane0[2], plane0[3] }; // Column 1
//   // columns[1] = { plane1[0], plane1[1], plane1[2], plane1[3] }; // Column 2
//   // columns[2] = { plane2[0], plane2[1], plane2[2], plane2[3] }; // Column 3
//   // columns[3] = { plane3[0], plane3[1], plane3[2], plane3[3] }; // Column 4
// 
//   *this = Matrix4(1.0f / std::tanf(halfFOVrads), 0.0f,                                    0.0f,                  0.0f,
//                   0.0f,                          width / std::tanf(halfFOVrads) / height, 0.0f,                  0.0f,
//                   0.0f,                          0.0f,                                    zFar / (zFar - zNear), - zNear * zFar / (zFar - zNear),
//                   0.0f,                          0.0f,                                    1.0f,                  0.0f);
// 
// 
//   return *this;
}


// TODO: check if this is the correct way to set the transform matrix
Vector3f
Matrix4::transformPosition(const Vector3f& v) const {
  return Vector3f(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] /* * 1.0f */,
                  m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] /* * 1.0f */,
                  m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] /* * 1.0f */);
}

// TODO: This is not the correct way to set the direction matrix
Vector3f
Matrix4::transformDirection(const Vector3f& v) const {
  return Vector3f(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z /* + m[3][0] * 0.0f */,
                  m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z /* + m[3][1] * 0.0f */,
                  m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z /* + m[3][2] * 0.0f */);
}

// TODO: check if this is the correct way to set the transform matrix
Vector4f
Matrix4::transformPositionV4(const Vector4f& v) const {
  return Vector4f(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w /* * 1.0f */,
                  m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w /* * 1.0f */,
                  m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w /* * 1.0f */,
                  m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w /* * 1.0f */);
}

// TODO: This is not the correct way to set the direction matrix
Vector4f
Matrix4::transformDirectionV4(const Vector4f& v) const {
  return Vector4f(m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z /* + m[3][0] * 0.0f */,
                  m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z /* + m[3][1] * 0.0f */,
                  m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z /* + m[3][2] * 0.0f */,
                  m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z /* + m[3][2] * 0.0f */);
}

void
Matrix4::translate(const Vector3f& translation) {
  m[3][0] += translation.x;
  m[3][1] += translation.y;
  m[3][2] += translation.z;
  m[3][3] = 1.0f;
}

void
Matrix4::translate(const float& x, const float& y, const float& z) {
  m[3][0] += x;
  m[3][1] += y;
  m[3][2] += z;
  m[3][3] = 1.0f;
}

void
Matrix4::rotateX(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(angle, 0.0f, 0.0f));

  setRotation(q * t);
}

void
Matrix4::rotateY(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(0.0f, angle, 0.0f));

  setRotation(q * t);
}

void
Matrix4::rotateZ(const float& angle) {
  Quaternion t(this->getRotationMatrix());
  Quaternion q(Euler(0.0f, 0.0f, angle));

  setRotation(q * t);
}

void
Matrix4::rotate(const float& angle, const Vector3f& axis) {
  float cos = Math::cos(Math::DEG2RAD * angle);
  float sin = Math::sin(Math::DEG2RAD * angle);
  float omc = 1.0f - cos;  // "One minus cos"
  float x = axis.x, y = axis.y, z = axis.z;
  // Normalize the axis vector
  float length = sqrt(x * x + y * y + z * z);
  if (length != 1.0f && length != 0.0f) {
    x /= length; y /= length; z /= length;
  }

  Matrix4 rot = Matrix4::IDENTITY;

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
Matrix4::rotate(const float& angle, const float& x, const float& y, const float& z) {
  return rotate(angle, Vector3f(x, y, z));
}

void
Matrix4::rotate(const Quaternion& rotation) {
  Quaternion t(getRotationMatrix());

  setRotation(rotation * t);
}

void
Matrix4::rotate(const Vector3f& delta) {
  Quaternion t = Quaternion(getRotationMatrix());
  Quaternion q = Quaternion(Euler(delta));

  setRotation(q * t);
}

void
Matrix4::scale(const Vector3f& scale) {
  m[0][0] *= scale.x;
  m[1][1] *= scale.y;
  m[2][2] *= scale.z;
}

void
Matrix4::scale(const float& x, const float& y, const float& z) {
  m[0][0] *= x;
  m[1][1] *= y;
  m[2][2] *= z;
}

void
Matrix4::scale(const float& scale) {
  m[0][0] *= scale;
  m[1][1] *= scale;
  m[2][2] *= scale;
}

void
Matrix4::setPosition(const Vector3f& position) {
  m[3][0] = position.x;
  m[3][1] = position.y;
  m[3][2] = position.z;
  m[3][3] = 1.0f;
}

void
Matrix4::setRotation(const Vector3f& rotation) {
  Matrix4 T = getTranslationMatrix();
  Matrix4 R = Matrix4::IDENTITY;
  Matrix4 S = getScaleMatrix();

  R.rotate(rotation);

  *this = T * (R * S);
}

void
Matrix4::setRotation(const Quaternion& rotation) {
  Matrix4 T = getTranslationMatrix();
  Matrix4 R = rotation.getMatrix4Rotation();
  Matrix4 S = getScaleMatrix();

  *this = T * (R * S);
}

void
Matrix4::setScale(const Vector3f& scale) {
  Matrix4 T = getTranslationMatrix();
  Matrix4 R = getRotationMatrix();
  Matrix4 S = Matrix4::IDENTITY;

  S.m[0][0] = scale.x;
  S.m[1][1] = scale.y;
  S.m[2][2] = scale.z;

  *this = T * (R * S);
}

void
Matrix4::setScale(const float& newScale) {
  Matrix4 T = getTranslationMatrix();
  Matrix4 R = getRotationMatrix();
  Matrix4 S = Matrix4::IDENTITY;

  S.scale(newScale);

  *this = T * (R * S);
}

Vector3f
Matrix4::getForwardVector() const {
  return Vector3f(m[2][0], 
                  m[2][1], 
                  m[2][2]).normalized();
}

Vector3f
Matrix4::getRightVector() const {
  return Vector3f(m[0][0], 
                  m[0][1], 
                  m[0][2]).normalized();
}

Vector3f
Matrix4::getUpVector() const {
  return Vector3f(m[1][0], 
                  m[1][1], 
                  m[1][2]).normalized();
}

Vector3f
Matrix4::getPosition() const {
  return Vector3f(m[3][0], 
                  m[3][1], 
                  m[3][2]);
}

Euler
Matrix4::getEulerRotation() const {
  return Euler(*this);
}

Quaternion
Matrix4::getQuatRotation() const {
  return Quaternion(*this);
}

Vector3f
Matrix4::getScale() const {
  return Vector3f(std::sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
                  std::sqrt(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
                  std::sqrt(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]));
}

Matrix4
Matrix4::getTranslationMatrix() const {
  Matrix4 translationMatrix = Matrix4::IDENTITY;
  translationMatrix.m[3][0] = m[3][0];
  translationMatrix.m[3][1] = m[3][1];
  translationMatrix.m[3][2] = m[3][2];

  return translationMatrix;
}

Matrix4
Matrix4::getRotationMatrix() const {
  const Vector3f& scale = getScale();

  CY_ASSERT(!Math::isNearSame(scale.x, 0.0f) &&
            !Math::isNearSame(scale.y, 0.0f) &&
            !Math::isNearSame(scale.z, 0.0f),
            Utils::format("Trying to get rotation matrix with a scale component of 0",
                          this->toString()).c_str());

  Matrix4 rotationMatrix = *this;
  rotationMatrix.setPosition(Vector3f::ZERO);

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

Matrix4
Matrix4::getScaleMatrix() const {
  const Vector3f& scale = getScale();

  CY_ASSERT(!Math::isNearSame(scale.x, 0.0f) &&
            !Math::isNearSame(scale.y, 0.0f) &&
            !Math::isNearSame(scale.z, 0.0f),
            Utils::format("Trying to get scale matrix with a scale component of 0",
                          this->toString()).c_str());

  Matrix4 rotationMatrix = Matrix4::IDENTITY;
  rotationMatrix.m[0][0] = scale.x;
  rotationMatrix.m[1][1] = scale.y;
  rotationMatrix.m[2][2] = scale.z;

  return rotationMatrix;
}

const Matrix3&
Matrix4::subMatrix() const {
  return Matrix3(m[0][0], m[1][0], m[2][0],
                 m[0][1], m[1][1], m[2][1],
                 m[0][2], m[1][2], m[2][2]);
}

String
Matrix4::toString() {
  return Utils::format("[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n", 
                        m[0][0], m[1][0], m[2][0], m[3][0],
                        m[0][1], m[1][1], m[2][1], m[3][1],
                        m[0][2], m[1][2], m[2][2], m[3][2],
                        m[0][3], m[1][3], m[2][3], m[3][3]);
}

void
Matrix4::removeScaleFromRotation(Vector3f scale, Matrix3& rotation) const {
  rotation.m[0][0] /= scale.x; rotation.m[0][1] /= scale.x; rotation.m[0][2] /= scale.x;
  rotation.m[1][0] /= scale.y; rotation.m[1][1] /= scale.y; rotation.m[1][2] /= scale.y;
  rotation.m[2][0] /= scale.z; rotation.m[2][1] /= scale.z; rotation.m[2][2] /= scale.z;
}

const Matrix4 Matrix4::ZERO     = Matrix4(0.0f);

const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);

} // namespace CYLLENE_SDK