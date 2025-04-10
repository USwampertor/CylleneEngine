#include "cyMatrix4.h"
#include "cyUtilities.h"

#include "cyMatrix2x2.h"
#include "cyMatrix3x3.h"
#include "cyMath.h"

#define LH 0
#define RH 1

#define OpenGL  0
#define DirectX 1

#define HandSystrem LH
#define GraphicsAPI OpenGL

namespace CYLLENE_SDK {
Matrix4::Matrix4(const float& value) {
  memset(&_m, static_cast<int32>(0), sizeof(_m));

  if (0 != value) {
    _m.m00 = _m.m11 = _m.m22 = _m.m33 = 1.0f;
  }
}

Matrix4::Matrix4(const Matrix4& other)
  : _m(other._m) {}

Matrix4::Matrix4(const Matrix3x3& other) {
  m[0][0] = other.m[0][0]; m[1][0] = other.m[1][0]; m[2][0] = other.m[2][0]; m[3][0] = 0;
  m[0][1] = other.m[0][1]; m[1][1] = other.m[1][1]; m[2][1] = other.m[2][1]; m[3][1] = 0;
  m[0][2] = other.m[0][1]; m[1][2] = other.m[1][1]; m[2][2] = other.m[2][1]; m[3][2] = 0;
  m[0][3] = 0;             m[1][3] = 0;             m[2][3] = 0;             m[3][3] = 0;
}

Matrix4::Matrix4(const Matrix2x2& other) {
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
  Matrix4 result;
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      result.m[c][r] = 0;
      for (int k = 0; k < 4; k++) {
        result.m[c][r] += m[k][r] * b.m[c][k];
      }
    }
  }
  return result;
}

Matrix4&
Matrix4::operator+=(const Matrix4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] += b.m[c][r];
    }
  }
  return *this;
}

Matrix4&
Matrix4::operator-=(const Matrix4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] -= b.m[c][r];
    }
  }
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
  memset(&_m, static_cast<int32>(value), sizeof(_m));

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
  
  Matrix4 T = Matrix4::IDENTITY; // Translation
  T.setPosition(position);

  Matrix4 R = rotation.toMat4(); // Rotation (from quaternion)
  Matrix4 S = Matrix4::IDENTITY;  // Scale
  S.setScale(scale);

  // Column-major: T × R × S
  Matrix4 M = T * (R * S);
  *this = M;

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
#if HandSystem == LH
  Vector3f front = (target - eye).normalized();
#elif HandSystem == RH
  Vector3f front = (eye - target).normalized();
#endif

  Vector3f right = Vector3f::cross(worldUp, front).normalized();
  Vector3f up = Vector3f::cross(front, right).normalized();

#if HandSystem == LH
  float A = -Vector3f::dot(right, eye);
  float B = -Vector3f::dot(up, eye);
  float C = -Vector3f::dot(front, eye);
#elif HandSystem == RH
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

#if GraphicsAPI == OpenGL
#if HandSystem == LH
  m[2][2] = 2.0f / (zFar - zNear);
#elif HandSystem == RH
  m[2][2] = -2.0f / (zNear - zFar);
#endif

  // TODO: Missing M[3][0] ???
  // TODO: Missing M[3][1] ???

  m[3][0] = 1; // -(right+left)/(right-left);
  m[3][1] = 1; // -(top+bottom)/(top-bottom);
  m[3][2] = -(zFar + zNear) / (zFar - zNear);

#elif GraphicsAPI == DirectX
#if HandSystem == LH
  m[2][2] = 1.0f / (zFar - zNear);
#elif HandSystem == RH
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

#if GraphicsAPI == OpenGL
#if HandSystem == LH
  m[2][2] = 2.0f / (zFar - zNear);
#elif HandSystem == RH
  m[2][2] = -2.0f / (zNear - zFar);
#endif

  // TODO: Missing M[3][0] ???
  // TODO: Missing M[3][1] ???

  m[3][0] = -(right + left) / (right - left);
  m[3][1] = -(top + bottom) / (top - bottom);
  m[3][2] = -(zFar + zNear) / (zFar - zNear);

#elif GraphicsAPI == DirectX
#if HandSystem == LH
  m[2][2] = 1.0f / (zFar - zNear);
#elif HandSystem == RH
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
                     const float FOV) {

  float halfFOV = FOV * 0.5;
  float plane0[4] = { 1.0f / std::tanf(halfFOV), 0.0f,                                0.0f,                           0.0f };
  float plane1[4] = { 0.0f,                      width / std::tanf(halfFOV) / height, 0.0f,                           0.0f };
  float plane2[4] = { 0.0f,                      0.0f,                                zFar / (zFar - zNear),          1.0f };
  float plane3[4] = { 0.0f,                      0.0f,                                -zNear * zFar / (zFar - zNear), 0.0f };


  columns[0] = { plane0[0], plane0[1], plane0[2], plane0[3] }; // Column 1
  columns[1] = { plane1[0], plane1[1], plane1[2], plane1[3] }; // Column 2
  columns[2] = { plane2[0], plane2[1], plane2[2], plane2[3] }; // Column 3
  columns[3] = { plane3[0], plane3[1], plane3[2], plane3[3] }; // Column 4

  return *this;

}


// TODO: This is not the correct way to set the transform matrix
Vector3f
Matrix4::transformPosition(const Vector3f& v) const {
  return Vector3f(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
                  m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                  m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
}

// TODO: This is not the correct way to set the direction matrix
Vector3f
Matrix4::transformDirection(const Vector3f& v) const {
  return Vector3f(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                  m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                  m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
}

void
Matrix4::translate(const Vector3f& translation) {
  m[3][0] += translation.x;
  m[3][1] += translation.y;
  m[3][2] += translation.z;
}

void
Matrix4::setPosition(const Vector3f& position) {
  m[3][0] = position.x;
  m[3][1] = position.y;
  m[3][2] = position.z;
  m[3][3] = 1.0f;
}

void
Matrix4::rotateX(const float& angle) {
  float c = Math::cos(Math::DEG2RAD * angle);
  float s = Math::sin(Math::DEG2RAD * angle);
  Matrix4 temp = *this;

  m[1][0] = c * temp.m[1][0] + s * temp.m[2][0]; m[2][1] = c * temp.m[2][0] - s * temp.m[1][0];
  m[1][1] = c * temp.m[1][1] + s * temp.m[2][1]; m[2][2] = c * temp.m[2][1] - s * temp.m[1][1];
  m[1][2] = c * temp.m[1][2] + s * temp.m[2][2]; m[2][3] = c * temp.m[2][2] - s * temp.m[1][2];
  

}

void
Matrix4::rotateY(const float& angle) {

  float c = Math::cos(angle * Math::DEG2RAD);
  float s = Math::sin(angle * Math::DEG2RAD);
  Matrix4 temp = *this;

#if HandSystem == LH
  m[0][0] = temp.m[0][0] * c + temp.m[2][0] * s; m[2][0] =  temp.m[2][0] * c - temp.m[0][0] * s;
  m[0][1] = temp.m[0][1] * c + temp.m[2][1] * s; m[2][2] =  temp.m[2][1] * c - temp.m[0][1] * s;
  m[0][2] = temp.m[0][2] * c + temp.m[2][2] * s; m[2][1] =  temp.m[2][2] * c - temp.m[0][2] * s;
#elif HandSystem == RH
  m[0][0] = temp.m[0][0] * c - temp.m[2][0] * s; m[2][0] = temp.m[2][0] * c + temp.m[0][0] * s;
  m[0][1] = temp.m[0][1] * c - temp.m[2][1] * s; m[2][2] = temp.m[2][1] * c + temp.m[0][1] * s;
  m[0][2] = temp.m[0][2] * c - temp.m[2][2] * s; m[2][1] = temp.m[2][2] * c + temp.m[0][2] * s;
#endif 

}

void
Matrix4::rotateZ(const float& angle) {

  float s = Math::sin(angle * Math::DEG2RAD);
  float c = Math::cos(angle * Math::DEG2RAD);
  Matrix4 temp = *this;

#if HandSystem == LH
  m[0][0] = c * temp.m[0][0] + s * temp.m[1][0]; m[1][0] = c * temp.m[1][0] - s * temp.m[0][0];
  m[0][1] = c * temp.m[0][1] + s * temp.m[1][1]; m[1][1] = c * temp.m[1][1] - s * temp.m[0][1];
  m[0][2] = c * temp.m[0][2] + s * temp.m[1][2]; m[1][2] = c * temp.m[1][2] - s * temp.m[0][2];
#elif HandSystem == RH
  m[0][0] = c * temp.m[0][0] - s * temp.m[1][0]; m[1][0] = c * temp.m[1][0] + s * temp.m[0][0];
  m[0][1] = c * temp.m[0][1] - s * temp.m[1][1]; m[1][1] = c * temp.m[1][1] + s * temp.m[0][1];
  m[0][2] = c * temp.m[0][2] - s * temp.m[1][2]; m[1][2] = c * temp.m[1][2] + s * temp.m[0][2];
#endif
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

  Matrix4 rot;

#if HandSystem == LH
  rot.m[0][0] = cos + x * x * omc;     rot.m[1][0] = x * y * omc - z * sin; rot.m[2][0] = x * z * omc + y * sin;
  rot.m[0][1] = y * x * omc + z * sin; rot.m[1][1] = cos + y * y * omc;     rot.m[2][1] = y * z * omc - x * sin;
  rot.m[0][2] = z * x * omc - y * sin; rot.m[1][2] = z * y * omc + x * sin; rot.m[2][2] = cos + z * z * omc;
#elif HandSystem == RH
  rot.m[0][0] = cos + x * x * omc;     rot.m[1][0] = x * y * omc + z * sin; rot.m[2][0] = x * z * omc - y * sin;
  rot.m[0][1] = y * x * omc - z * sin; rot.m[1][1] = cos + y * y * omc;     rot.m[2][1] = y * z * omc + x * sin;
  rot.m[0][2] = z * x * omc + y * sin; rot.m[1][2] = z * y * omc - x * sin; rot.m[2][2] = cos + z * z * omc;
#endif
  *this = *this * rot;

}


void
Matrix4::rotate(const float& angle, const float& x, const float& y, const float& z) {
  return rotate(angle, Vector3f(x, y, z));
}

void
Matrix4::rotate(const Quaternion& rotation) {
  Matrix4 rotationMatrix = rotation.toMat3();
  *this *= rotationMatrix;
}

void
Matrix4::setRotation(const Vector3f& rotation) {
  Quaternion q;
  q.fromEuler(Euler(rotation));
  Matrix4 temp = q.toMat3();
  *this = temp;
}

void
Matrix4::setRotation(const Quaternion& rotation) {
  *this = rotation.toMat3();
}

void
Matrix4::setScale(const float& newScale) {
  m[0][0] = newScale;
  m[1][1] = newScale;
  m[2][2] = newScale;
}

void
Matrix4::setScale(const Vector3f& scale) {
  m[0][0] = scale.x;
  m[1][1] = scale.y;
  m[2][2] = scale.z;
}

void
Matrix4::scale(const Vector3f& scale) {
  m[0][0] *= scale.x;
  m[1][1] *= scale.y;
  m[2][2] *= scale.z;
}

void
Matrix4::scale(const float& scale) {
  m[0][0] *= scale;
  m[1][1] *= scale;
  m[2][2] *= scale;
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
Matrix4::getForwardVector() const {
  return Vector3f(m[2][0], 
                  m[2][1], 
                  m[2][2]).normalized();
}

Vector3f
Matrix4::getPosition() const {
  return Vector3f(m[3][0], 
                  m[3][1], 
                  m[3][2]);
}

Matrix3x3
Matrix4::subMatrix() {
  return Matrix3x3(m[0][0], m[1][0], m[2][0],
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

const Matrix4 Matrix4::ZERO     = Matrix4(0.0f);

const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);

} // namespace CYLLENE_SDK