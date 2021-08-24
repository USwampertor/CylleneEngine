#include "cyMatrix4x4.h"
#include "cyUtilities.h"

#include "cyMatrix3x3.h"
#include "cyPlatformMath.h"

namespace CYLLENE_SDK {
  Matrix4x4::Matrix4x4(const float& value) {
    memset(this, static_cast<float>(value), sizeof(Matrix4x4));
  }

  Matrix4x4::Matrix4x4(const Matrix4x4& other)
    : _m(other._m) {}

  Matrix4x4::Matrix4x4(const float& v00, const float& v01, const float& v02, const float v03,
                       const float& v10, const float& v11, const float& v12, const float v13,
                       const float& v20, const float& v21, const float& v22, const float v23,
                       const float& v30, const float& v31, const float& v32, const float v33) {
    _m.m00 = v00; _m.m01 = v01; _m.m02 = v02; _m.m03 = v03;
    _m.m10 = v10; _m.m11 = v11; _m.m12 = v12; _m.m13 = v13;
    _m.m20 = v20; _m.m21 = v21; _m.m22 = v22; _m.m23 = v23;
    _m.m30 = v30; _m.m31 = v31; _m.m32 = v32; _m.m33 = v33;
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
    for (uint32 i = 0; i < 4; ++i) {
      for (uint32 j = 0; j < 4; ++j) {
        for (uint32 k = 0; k < 4; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
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
    *this = Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                      m[0][1], m[1][1], m[2][1], m[3][1],
                      m[0][2], m[1][2], m[2][2], m[3][2],
                      m[0][3], m[1][3], m[2][3], m[3][3]);
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
    float det = this->determinant();
    CY_ASSERT(det != 0.0f &&
              Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    
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
    
    temp.transpose();

    temp *= PlatformMath::pow(det, -1.0f);
    return temp;
  }

  void
  Matrix4x4::inverse() {
    CY_ASSERT(this->determinant() != 0.0f &&
              Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    Matrix3x3 temp(0);
    Matrix4x4 tmp2 = *this;

    temp.m[0][0] = m[1][1]; temp.m[0][1] = m[1][2]; temp.m[0][2] = m[1][3];
    temp.m[1][0] = m[2][1]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[0][0] = temp.determinant();

    temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][2]; temp.m[0][2] = m[1][3];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[1][0] = temp.determinant();

    temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][1]; temp.m[0][2] = m[1][3];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    tmp2.m[2][0] = temp.determinant();

    temp.m[0][0] = m[1][0]; temp.m[0][1] = m[1][1]; temp.m[0][2] = m[1][2];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][2];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    tmp2.m[3][0] = temp.determinant();

    temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[2][1]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[0][1] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][2]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[1][1] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    tmp2.m[2][1] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    temp.m[1][0] = m[2][0]; temp.m[1][1] = m[2][1]; temp.m[1][2] = m[2][2];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    tmp2.m[3][1] = temp.determinant();

    temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][1]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[3][1]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[0][2] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][2]; temp.m[2][2] = m[3][3];
    tmp2.m[1][2] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][3];
    tmp2.m[2][2] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][2];
    temp.m[2][0] = m[3][0]; temp.m[2][1] = m[3][1]; temp.m[2][2] = m[3][2];
    tmp2.m[3][2] = temp.determinant();

    temp.m[0][0] = m[0][1]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][1]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[2][1]; temp.m[2][1] = m[2][2]; temp.m[2][2] = m[2][3];
    tmp2.m[0][3] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][2]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][2]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][2]; temp.m[2][2] = m[2][3];
    tmp2.m[1][3] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][3];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][3];
    temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][1]; temp.m[2][2] = m[2][3];
    tmp2.m[2][3] = temp.determinant();

    temp.m[0][0] = m[0][0]; temp.m[0][1] = m[0][1]; temp.m[0][2] = m[0][2];
    temp.m[1][0] = m[1][0]; temp.m[1][1] = m[1][1]; temp.m[1][2] = m[1][2];
    temp.m[2][0] = m[2][0]; temp.m[2][1] = m[2][1]; temp.m[2][2] = m[2][2];
    tmp2.m[3][3] = temp.determinant();

    *this = tmp2;
    *this *= (1 / this->determinant());
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
}