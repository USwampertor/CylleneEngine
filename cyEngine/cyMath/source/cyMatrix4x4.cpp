#include "cyMatrix4x4.h"
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
Matrix4x4::Matrix4x4(const float& value) {
  memset(this, static_cast<int32>(value), sizeof(Matrix4x4));
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
  : _m(other._m) {}

Matrix4x4::Matrix4x4(const Matrix3x3& other) {
  _m.m00 = other._m.m00; _m.m01 = other._m.m01; _m.m02 = other._m.m02; _m.m03 = 0;
  _m.m10 = other._m.m10; _m.m11 = other._m.m11; _m.m12 = other._m.m12; _m.m13 = 0;
  _m.m20 = other._m.m20; _m.m21 = other._m.m21; _m.m22 = other._m.m22; _m.m23 = 0;
  _m.m30 = 0;            _m.m31 = 0;            _m.m32 = 0;            _m.m33 = 0;
}

Matrix4x4::Matrix4x4(const Matrix2x2& other) {
  _m.m00 = other._m.m00; _m.m01 = other._m.m01; _m.m02 = 0;            _m.m03 = 0;
  _m.m10 = other._m.m10; _m.m11 = other._m.m11; _m.m12 = 0;            _m.m13 = 0;
  _m.m20 = 0;            _m.m21 = 0;            _m.m22 = 0;            _m.m23 = 0;
  _m.m30 = 0;            _m.m31 = 0;            _m.m32 = 0;            _m.m33 = 0;
}

Matrix4x4::Matrix4x4(const float& v00, const float& v10, const float& v20, const float v30,
                     const float& v01, const float& v11, const float& v21, const float v31,
                     const float& v02, const float& v12, const float& v22, const float v32,
                     const float& v03, const float& v13, const float& v23, const float v33) {
  _m.m00 = v00; _m.m10 = v10; _m.m20 = v20; _m.m30 = v30;
  _m.m01 = v01; _m.m11 = v11; _m.m21 = v21; _m.m31 = v31;
  _m.m02 = v02; _m.m12 = v12; _m.m22 = v22; _m.m32 = v32;
  _m.m03 = v03; _m.m13 = v13; _m.m23 = v23; _m.m33 = v33;
}

Matrix4x4
Matrix4x4::operator+(const Matrix4x4& b) {
  Matrix4x4 temp;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      temp.m[i][j] = m[i][j] + b.m[i][j];
    }
  }
  return temp;

}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4& b) {
  Matrix4x4 temp;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      temp.m[i][j] = m[i][j] - b.m[i][j];
    }
  }
  return temp;
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4& b) {
  Matrix4x4 temp = ZERO;
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      for (uint32 k = 0; k < 4; ++k) {
        temp.m[i][j] += m[i][k] * b.m[k][j];
      }
    }
  }
  return temp;

//   Matrix4x4 temp = ZERO;
//   for (uint32 col = 0; col < 4; ++col) {
//     for (uint32 row = 0; row < 4; ++row) {
//       for (uint32 k = 0; k < 4; ++k) {
//         temp.m[col][row] += m[k][row] * b.m[col][k];  // Note index order
//       }
//     }
//   }
//   return temp;
}

Matrix4x4&
Matrix4x4::operator+=(const Matrix4x4& b) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] += b.m[i][j];
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator-=(const Matrix4x4& b) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] -= b.m[i][j];
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator*=(const Matrix4x4& b) {
  Matrix4x4 temp = ZERO;
  for (uint32 col = 0; col < 4; ++col) {
    for (uint32 row = 0; row < 4; ++row) {
      for (uint32 k = 0; k < 4; ++k) {
        temp.m[col][row] += m[k][row] * b.m[col][k];  // Note index order
      }
    }
  }
  *this = temp;
  return *this;
}

Matrix4x4&
Matrix4x4::operator+=(const float& value) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] += value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator-=(const float& value) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] -= value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator*=(const float& value) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] *= value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator/=(const float& value) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      m[i][j] /= value;
    }
  }
  return *this;
}

bool
Matrix4x4::operator==(const Matrix4x4& b) {
  for (uint32 i = 0; i < 4; ++i) {
    for (uint32 j = 0; j < 4; ++j) {
      if (m[i][j] != b.m[i][j]) { return false; }
    }
  }
  return true;
}

void
Matrix4x4::identity() {
  *this = IDENTITY;
}

void
Matrix4x4::zero() {
  *this = ZERO;
}

Matrix4x4
Matrix4x4::transposed() const {
  return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                   m[0][1], m[1][1], m[2][1], m[3][1],
                   m[0][2], m[1][2], m[2][2], m[3][2],
                   m[0][3], m[1][3], m[2][3], m[3][3]);
}

void
Matrix4x4::transpose() {
  *this = this->transposed();
}

Matrix4x4
Matrix4x4::cofactored() const {
  Matrix4x4 temp;

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
Matrix4x4::cofactor() {
  *this = this->cofactored();
}

void
Matrix4x4::setValues(const float& value) {
  m[0][0] = m[0][1] = m[0][2] = m[0][3] = 
  m[1][0] = m[1][1] = m[1][2] = m[1][3] =
  m[2][0] = m[2][1] = m[2][2] = m[2][3] =
  m[3][0] = m[3][1] = m[3][2] = m[3][3] = value;
}

void
Matrix4x4::setValues(const float& v00, const float& v01, const float& v02, const float v03,
                      const float& v10, const float& v11, const float& v12, const float v13,
                      const float& v20, const float& v21, const float& v22, const float v23,
                      const float& v30, const float& v31, const float& v32, const float v33) {
  _m.m00 = v00; _m.m01 = v01; _m.m02 = v02; _m.m03 = v03;
  _m.m10 = v10; _m.m11 = v11; _m.m12 = v12; _m.m13 = v13;
  _m.m20 = v20; _m.m21 = v21; _m.m22 = v22; _m.m23 = v23;
  _m.m30 = v30; _m.m31 = v31; _m.m32 = v32; _m.m33 = v33;
}

Matrix4x4
Matrix4x4::inversed() {
  CY_ASSERT(this->determinant() != 0.0f &&
            Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    
  Matrix4x4 temp = *this;
  temp.cofactor();
  temp.transpose();

  temp *= Math::pow(this->determinant(), -1.0f);
  return temp;
  
}

void
Matrix4x4::inverse() {
  *this = this->inversed();

}

const float
Matrix4x4::determinant() const {
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
  
Matrix4x4&
Matrix4x4::View(const Vector4f& Eye,
                const Vector4f& Target,
                const Vector4f& WorldUp) {
#if HandSystem == LH
  Vector3f look = (Target - Eye).normalized();
#elif HandSystem == RH
  Vector3f look = (Eye - Target).normalized();
#endif

  Vector3f right = Vector3f::cross(WorldUp, look).normalized();
  Vector3f up = Vector3f::cross(look, right).normalized();

#if HandSystem == LH
  float A = -Vector3f::dot(right, Eye);
  float B = -Vector3f::dot(up, Eye);
  float C = -Vector3f::dot(look, Eye);
#elif HandSystem == RH
  float A = Vector3f::dot(right, Eye);
  float B = Vector3f::dot(up, Eye);
  float C = Vector3f::dot(look, Eye);
#endif

  (*this).columns[0] = Vector4f(right.x, up.x, look.x, 0.0f);
  (*this).columns[1] = Vector4f(right.y, up.y, look.y, 0.0f);
  (*this).columns[2] = Vector4f(right.z, up.z, look.z, 0.0f);
  (*this).columns[3] = Vector4f(A,       B,    C,      1.0f);

  return *this;
}
  
Matrix4x4&
Matrix4x4::Orthogonal(const float Width,
                      const float Height,
                      const float ZNear,
                      const float ZFar) {
  (*this) = Matrix4x4::ZERO;

  (*this).m[0][0] = 2.0f / Width;
  (*this).m[1][1] = 2.0f / Height;

#if GraphicsAPI == OpenGL
#if HandSystem == LH
  (*this).m[2][2] = -2.0f / (ZFar - ZNear);
#elif HandSystem == RH
  (*this).m[2][2] = -2.0f / (ZNear - ZFar);
#endif

  (*this).m[3][2] = -(ZFar + ZNear) / (ZFar - ZNear);

#elif GraphicsAPI == DirectX
#if HandSystem == LH
  (*this).m[2][2] = 1.0f / (ZFar - ZNear);
#elif HandSystem == RH
  (*this).m[2][2] = 1.0f / (ZNear - ZFar);
#endif

  (*this).m[3][2] = ZNear / (ZNear - ZFar);
#endif

  (*this).m[3][3] = 1.0f;

  return *this;
}

Matrix4x4&
Matrix4x4::Perspective(const float Width,
                        const float Height,
                        const float ZNear,
                        const float ZFar,
                        const float FOV) {
  float aspect = Height / Width;

  float yScale = 1.0f / Math::tan(FOV * 0.5f);
  float xScale = yScale * aspect;

  float FarMNear = (ZFar - ZNear);

  *this = Matrix4x4::ZERO;

#if GraphicsAPI == OpenGL
  (*this).m[0][0] = xScale;
  (*this).m[1][1] = yScale;
  (*this).m[2][2] = -(ZFar + ZNear) / FarMNear;
#if HandSystem == LH
  (*this).m[2][3] = 1.0f;
  (*this).m[3][2] = 2.0f * (ZFar * ZNear) / FarMNear;
#elif HandSystem == RH
  (*this).m[2][3] = -1.0f;
  (*this).m[3][2] = -2.0f * (ZFar * ZNear) / FarMNear;
#endif

#elif GraphicsAPI == DirectX
  (*this).m[0][0] = xScale;
  (*this).m[1][1] = yScale;
  (*this).m[2][2] = ZFar / FarMNear;
#if HandSystem == LH
  (*this).m[3][2] = 1.0f;
  (*this).m[2][3] = -(ZNear * ZFar) / FarMNear;
#elif HandSystem == RH
  (*this).m[3][2] = -1.0f;
  (*this).m[2][3] = (ZNear * ZFar) / FarMNear;
#endif
#endif

  return *this;
}



Vector3f
Matrix4x4::transformPosition(const Vector3f& v) const {
  Vector3f temp;
  temp.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
  temp.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
  temp.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];
  return temp;
}

Vector3f
Matrix4x4::transformDirection(const Vector3f& v) const {
  Vector3f temp;
  temp.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z;
  temp.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z;
  temp.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z;
  return temp;
}

void
Matrix4x4::translate(const Vector3f& translation) {
  m[3][0] += translation.x;
  m[3][1] += translation.y;
  m[3][2] += translation.z;
}

void
Matrix4x4::setPosition(const Vector3f& position) {
  m[3][0] = position.x;
  m[3][1] = position.y;
  m[3][2] = position.z;
}

void
Matrix4x4::rotateX(const float& angle) {
  float c = Math::cos(angle);
  float s = Math::sin(angle);
  Matrix4x4 temp = *this;
  m[1][1] = c * temp.m[1][1] - s * temp.m[2][1];
  m[1][2] = s * temp.m[1][1] + c * temp.m[2][1];
  m[2][1] = -s * temp.m[1][1] + c * temp.m[2][1];
  m[2][2] = c * temp.m[1][1] + s * temp.m[2][1];
}

void
Matrix4x4::rotateY(const float& angle) {
  float c = Math::cos(angle);
  float s = Math::sin(angle);
  
  Matrix4x4 temp = *this;
  
  m[0][0] = c * temp.m[0][0] + s * temp.m[2][0];
  m[0][2] = -s * temp.m[0][0] + c * temp.m[2][0];

  m[2][0] = s * temp.m[0][0] + c * temp.m[2][0];
  m[2][2] = c * temp.m[0][0] - s * temp.m[2][0];
}

void
Matrix4x4::rotateZ(const float& angle) {
  float c = Math::cos(angle);
  float s = Math::sin(angle);

  Matrix4x4 temp = *this;
  
  m[0][0] = c * temp.m[0][0] - s * temp.m[1][0];
  m[1][0] = s * temp.m[0][0] + c * temp.m[1][0];

  m[0][0] = c * temp.m[0][1] - s * temp.m[1][1];
  m[1][0] = s * temp.m[0][1] + c * temp.m[1][1];

  // m[0][1] = s * temp.m[0][0] + c * temp.m[1][0];
  // m[1][1] = c * temp.m[0][0] + s * temp.m[1][0];
}

void
Matrix4x4::rotate(const Vector3f& rotation) {
  Quaternion q;
  q.fromEuler(Euler(rotation), 0);
  Matrix4x4 temp = q.getRotationMatrix();
  *this *= temp;
}

void
Matrix4x4::rotate(const Quaternion& rotation) {
  Matrix4x4 rotationMatrix = rotation.getRotationMatrix();
  *this *= rotationMatrix;
}

void
Matrix4x4::setRotation(const Vector3f& rotation) {
  Quaternion q;
  q.fromEuler(Euler(rotation), 0);
  Matrix4x4 temp = q.getRotationMatrix();
  *this = temp;
}

void
Matrix4x4::setRotation(const Quaternion& rotation) {
  *this = rotation.getRotationMatrix();
}

void
Matrix4x4::setScale(const float& newScale) {
  m[0][0] = newScale;
  m[1][1] = newScale;
  m[2][2] = newScale;
}

void
Matrix4x4::setScale(const Vector3f& scale) {
  m[0][0] = scale.x;
  m[1][1] = scale.y;
  m[2][2] = scale.z;
}

void
Matrix4x4::scale(const Vector3f& scale) {
  m[0][0] *= scale.x;
  m[1][1] *= scale.y;
  m[2][2] *= scale.z;
}

void
Matrix4x4::scale(const float& scale) {
  m[0][0] *= scale;
  m[1][1] *= scale;
  m[2][2] *= scale;
}

Vector3f
Matrix4x4::getForwardVector() const {
  return Vector3f(m[0][2], m[1][2], m[2][2]).normalized();
}

Vector3f
Matrix4x4::getRightVector() const {
  return Vector3f(m[0][0], m[1][0], m[2][0]).normalized();
}

Vector3f
Matrix4x4::getUpVector() const {
  return Vector3f(m[0][1], m[1][1], m[2][1]).normalized();
}

Vector3f
Matrix4x4::getPosition() const {
  return Vector3f(m[3][0], m[3][1], m[3][2]);
}

Matrix3x3
Matrix4x4::subMatrix() {
  return Matrix3x3(m[0][0], m[0][1], m[0][2],
                   m[1][0], m[1][1], m[1][2],
                   m[2][0], m[2][1], m[2][2]);
}

String
Matrix4x4::toString() {
  return Utils::format("[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f), \t (%2.2f)]\n", 
                        m[0][0], m[0][1], m[0][2], m[0][3],
                        m[1][0], m[1][1], m[1][2], m[1][3],
                        m[2][0], m[2][1], m[2][2], m[2][3],
                        m[3][0], m[3][1], m[3][2], m[3][3]);
}

const Matrix4x4 Matrix4x4::ZERO     = Matrix4x4(0.0f);

const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 0.0f, 1.0f);

} // namespace CYLLENE_SDK