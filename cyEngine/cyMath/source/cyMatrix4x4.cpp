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
  // memset(this, static_cast<float>(value), 16*sizeof(this->m[0]));
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      m[c][r] = value;
    }
  }
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
  : _m(other._m) {}

Matrix4x4::Matrix4x4(const Matrix3x3& other) {
  m[0][0] = other.m[0][0]; m[1][0] = other.m[1][0]; m[2][0] = other.m[2][0]; m[3][0] = 0;
  m[0][1] = other.m[0][1]; m[1][1] = other.m[1][1]; m[2][1] = other.m[2][1]; m[3][1] = 0;
  m[0][2] = other.m[0][1]; m[1][2] = other.m[1][1]; m[2][2] = other.m[2][1]; m[3][2] = 0;
  m[0][3] = 0;             m[1][3] = 0;             m[2][3] = 0;             m[3][3] = 0;
}

Matrix4x4::Matrix4x4(const Matrix2x2& other) {
  m[0][0] = other.m[0][0]; m[1][0] = other.m[1][0]; m[2][0] = 0; m[3][0] = 0;
  m[0][1] = other.m[0][1]; m[1][1] = other.m[1][1]; m[2][1] = 0; m[3][1] = 0;
  m[0][2] = 0;             m[1][2] = 0;             m[2][2] = 0; m[3][2] = 0;
  m[0][3] = 0;             m[1][3] = 0;             m[2][3] = 0; m[3][3] = 0;
}

Matrix4x4::Matrix4x4(const float& m00, const float& m10, const float& m20, const float& m30,  // Row 0
                     const float& m01, const float& m11, const float& m21, const float& m31,  // Row 1
                     const float& m02, const float& m12, const float& m22, const float& m32,  // Row 2
                     const float& m03, const float& m13, const float& m23, const float& m33) {// Row 3
  m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
  m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
  m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
  m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
}

Matrix4x4
Matrix4x4::operator+(const Matrix4x4& b) {
  Matrix4x4 temp;
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      temp.m[c][r] = m[c][r] + b.m[c][r];
    }
  }
  return temp;

}

Matrix4x4
Matrix4x4::operator-(const Matrix4x4& b) {
  Matrix4x4 temp;
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      temp.m[c][r] = m[c][r] - b.m[c][r];
    }
  }
  return temp;
}

Matrix4x4
Matrix4x4::operator*(const Matrix4x4& b) {
  Matrix4x4 result;
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

Matrix4x4&
Matrix4x4::operator+=(const Matrix4x4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] += b.m[c][r];
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator-=(const Matrix4x4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] -= b.m[c][r];
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator*=(const Matrix4x4& b) {
  *this = *this * b;
  return *this;
}

Matrix4x4&
Matrix4x4::operator+=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] += value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator-=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] -= value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator*=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] *= value;
    }
  }
  return *this;
}

Matrix4x4&
Matrix4x4::operator/=(const float& value) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      m[c][r] /= value;
    }
  }
  return *this;
}

bool
Matrix4x4::operator==(const Matrix4x4& b) {
  for (uint32 c = 0; c < 4; ++c) {
    for (uint32 r = 0; r < 4; ++r) {
      if (!Math::isNearSame(m[c][r], b.m[c][r])) return false;
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
  return Matrix4x4(m[0][0], m[0][1], m[0][2], m[0][3],
                   m[1][0], m[1][1], m[1][2], m[1][3],
                   m[2][0], m[2][1], m[2][2], m[2][3],
                   m[3][0], m[3][1], m[3][2], m[3][3]);
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
Matrix4x4::setValues(const float& m00, const float& m10, const float& m20, const float& m30,  // Row 0
                     const float& m01, const float& m11, const float& m21, const float& m31,  // Row 1
                     const float& m02, const float& m12, const float& m22, const float& m32,  // Row 2
                     const float& m03, const float& m13, const float& m23, const float& m33) {// Row 3
  *this = Matrix4x4(m00, m10, m20, m30,
                    m01, m11, m21, m31,
                    m02, m12, m22, m32,
                    m03, m13, m23, m33);
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

  m[0][0] = right.x; m[0][1] = right.y; m[0][2] = right.z; m[0][3] = A;
  m[1][0] = up.x;    m[1][1] = up.y;    m[1][2] = up.z;    m[1][3] = B;
  m[2][0] = look.x;  m[2][1] = look.y;  m[2][2] = look.z;  m[2][3] = C;
  m[3][0] = 0.0f;    m[3][1] = 0.0f;    m[3][2] = 0.0f;    m[3][3] = 1.0f;

  return *this;
}
  
Matrix4x4&
Matrix4x4::Orthogonal(const float Width,
                      const float Height,
                      const float ZNear,
                      const float ZFar) {
  (*this) = Matrix4x4::ZERO;

  m[0][0] = 2.0f / Width;
  m[1][1] = 2.0f / Height;

#if GraphicsAPI == OpenGL
#if HandSystem == LH
  m[2][2] = -2.0f / (ZFar - ZNear);
#elif HandSystem == RH
  m[2][2] = -2.0f / (ZNear - ZFar);
#endif

  m[3][2] = -(ZFar + ZNear) / (ZFar - ZNear);

#elif GraphicsAPI == DirectX
#if HandSystem == LH
  m[2][2] = 1.0f / (ZFar - ZNear);
#elif HandSystem == RH
  m[2][2] = 1.0f / (ZNear - ZFar);
#endif

  m[3][2] = ZNear / (ZNear - ZFar);
#endif

  m[3][3] = 1.0f;

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
  m[0][0] = xScale;
  m[1][1] = yScale;
  m[2][2] = -(ZFar + ZNear) / FarMNear;
#if HandSystem == LH
  m[2][3] = 1.0f;
  m[3][2] = 2.0f * (ZFar * ZNear) / FarMNear;
#elif HandSystem == RH
  m[2][3] = -1.0f;
  m[3][2] = -2.0f * (ZFar * ZNear) / FarMNear;
#endif

#elif GraphicsAPI == DirectX
  m[0][0] = xScale;
  m[1][1] = yScale;
  m[2][2] = ZFar / FarMNear;
#if HandSystem == LH
  m[3][2] = 1.0f;
  m[2][3] = -(ZNear * ZFar) / FarMNear;
#elif HandSystem == RH
  m[3][2] = -1.0f;
  m[2][3] = (ZNear * ZFar) / FarMNear;
#endif
#endif

  return *this;
}



Vector3f
Matrix4x4::transformPosition(const Vector3f& v) const {
  return Vector3f(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
                  m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                  m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
}

Vector3f
Matrix4x4::transformDirection(const Vector3f& v) const {
  Vector3f temp;
  temp.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
  temp.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
  temp.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
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

  m[1][1] = c * temp.m[1][1] + s * temp.m[1][2];
  m[1][2] = -s * temp.m[1][1] + c * temp.m[1][2];
  m[2][1] = c * temp.m[2][1] + s * temp.m[2][2];
  m[2][2] = -s * temp.m[2][1] + c * temp.m[2][2];
}

void
Matrix4x4::rotateY(const float& angle) {
  float c = Math::cos(angle);
  float s = Math::sin(angle);
  Matrix4x4 temp = *this;

  m[0][0] = c * temp.m[0][0] - s * temp.m[0][2];
  m[0][2] = s * temp.m[0][0] + c * temp.m[0][2];
  m[2][0] = c * temp.m[2][0] - s * temp.m[2][2];
  m[2][2] = s * temp.m[2][0] + c * temp.m[2][2];
}

void
Matrix4x4::rotateZ(const float& angle) {
  float c = Math::cos(angle);
  float s = Math::sin(angle);
  Matrix4x4 temp = *this;

  m[0][0] = c * temp.m[0][0] - s * temp.m[1][0];
  m[0][1] = c * temp.m[0][1] - s * temp.m[1][1];
  m[1][0] = s * temp.m[0][0] + c * temp.m[1][0];
  m[1][1] = s * temp.m[0][1] + c * temp.m[1][1];
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
                        m[0][0], m[1][0], m[2][0], m[3][0],
                        m[0][1], m[1][1], m[2][1], m[3][1],
                        m[0][2], m[1][2], m[2][2], m[3][2],
                        m[0][3], m[1][3], m[2][3], m[3][3]);
}

const Matrix4x4 Matrix4x4::ZERO     = Matrix4x4(0.0f);

const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 0.0f, 1.0f);

} // namespace CYLLENE_SDK