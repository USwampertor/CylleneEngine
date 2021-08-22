#include "cyMatrix3x3.h"

#include "cyUtilities.h"

namespace CYLLENE_SDK {
  Matrix3x3::Matrix3x3(const float& value) {
    memset(this, static_cast<float>(value), sizeof(Matrix3x3));
  }

  Matrix3x3::Matrix3x3(const Matrix3x3& other)
    : _m(other._m) {}

  Matrix3x3::Matrix3x3(const float& v00, const float& v01, const float& v02,
                       const float& v10, const float& v11, const float& v12,
                       const float& v20, const float& v21, const float& v22) {
    _m.m00 = v00; _m.m01 = v01; _m.m02 = v02;
    _m.m10 = v10; _m.m11 = v11; _m.m12 = v12;
    _m.m20 = v20; _m.m21 = v21; _m.m22 = v22;
  }

  Matrix3x3
  Matrix3x3::operator+(const Matrix3x3& b) {
    Matrix3x3 temp;
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        temp.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return temp;

  }

  Matrix3x3
  Matrix3x3::operator-(const Matrix3x3& b) {
    Matrix3x3 temp;
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        temp.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return temp;
  }

  Matrix3x3
  Matrix3x3::operator*(const Matrix3x3& b) {
    Matrix3x3 temp = ZERO;
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        for (uint32 k = 0; k < 3; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    return temp;
  }

  Matrix3x3&
  Matrix3x3::operator+=(const Matrix3x3& b) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] += b.m[i][j];
      }
    }
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator-=(const Matrix3x3& b) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] -= b.m[i][j];
      }
    }
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator*=(const Matrix3x3& b) {
    Matrix3x3 temp = ZERO;
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        for (uint32 k = 0; k < 3; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    *this = temp;
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator+=(const float& value) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] += value;
      }
    }
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator-=(const float& value) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] -= value;
      }
    }
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator*=(const float& value) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] *= value;
      }
    }
    return *this;
  }

  Matrix3x3&
  Matrix3x3::operator/=(const float& value) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        m[i][j] /= value;
      }
    }
    return *this;
  }

  bool
  Matrix3x3::operator==(const Matrix3x3& b) {
    for (uint32 i = 0; i < 3; ++i) {
      for (uint32 j = 0; j < 3; ++j) {
        if (m[i][j] != b.m[i][j]) { return false; }
      }
    }
    return true;
  }

  void
  Matrix3x3::identity() {
    *this = IDENTITY;
  }

  void
  Matrix3x3::zero() {
    *this = ZERO;
  }

  Matrix3x3
  Matrix3x3::transposed() const {
    return Matrix3x3(m[0][0], m[1][0], m[2][0],
                     m[0][1], m[1][1], m[2][1],
                     m[0][2], m[1][2], m[2][2]);
  }

  void
  Matrix3x3::transpose() {
    *this = Matrix3x3(m[0][0], m[1][0], m[2][0],
                      m[0][1], m[1][1], m[2][1],
                      m[0][2], m[1][2], m[2][2]);
  }

  void
  Matrix3x3::setValues(const float& value) {
    m[0][0] = m[0][1] = m[0][2] = 
    m[1][0] = m[1][1] = m[1][2] =
    m[2][0] = m[2][1] = m[2][2] = value;
  }

  void
  Matrix3x3::setValues(const float& v00, const float& v01, const float& v02,
                       const float& v10, const float& v11, const float& v12,
                       const float& v20, const float& v21, const float& v22) {
    _m.m00 = v00; _m.m01 = v01; _m.m02 = v02;
    _m.m10 = v10; _m.m11 = v11; _m.m12 = v12;
    _m.m20 = v20; _m.m21 = v21; _m.m22 = v22;
  }

  Matrix3x3
  Matrix3x3::inversed() {
    CY_ASSERT(this->determinant() != 0.0f &&
              Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    Matrix3x3 temp = *this;
    // We assign the cofactored matrices on the corresponding space when transposing
    // This way we save ourselves a function call
    temp.m[0][0] =  ((m[1][1] * m[2][2]) - (m[2][1] * m[1][2]));
    temp.m[1][0] = -((m[1][0] * m[2][2]) - (m[2][0] * m[1][2]));
    temp.m[2][0] =  ((m[1][0] * m[2][1]) - (m[2][0] * m[1][1]));
    temp.m[0][1] = -((m[0][1] * m[2][2]) - (m[2][1] * m[0][2]));
    temp.m[1][1] =  ((m[0][0] * m[2][2]) - (m[2][0] * m[0][2]));
    temp.m[2][1] = -((m[0][0] * m[2][1]) - (m[2][0] * m[0][1]));
    temp.m[0][2] =  ((m[0][1] * m[1][2]) - (m[1][1] * m[0][2]));
    temp.m[1][2] = -((m[0][0] * m[1][2]) - (m[1][0] * m[0][2]));
    temp.m[2][2] =  ((m[0][0] * m[1][1]) - (m[1][0] * m[0][1]));

    temp /= this->determinant();
    return temp;
  }

  void
  Matrix3x3::inverse() {
    CY_ASSERT(this->determinant() != 0.0f &&
              Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    Matrix3x3 temp = *this;
    // We assign the cofactored matrices on the corresponding space when transposing
    // This way we save ourselves a function call
    m[0][0] =  ((temp.m[1][1] * temp.m[2][2]) - (temp.m[2][1] * temp.m[1][2]));
    m[1][0] = -((temp.m[1][0] * temp.m[2][2]) - (temp.m[2][0] * temp.m[1][2]));
    m[2][0] =  ((temp.m[1][0] * temp.m[2][1]) - (temp.m[2][0] * temp.m[1][1]));
    m[0][1] = -((temp.m[0][1] * temp.m[2][2]) - (temp.m[2][1] * temp.m[0][2]));
    m[1][1] =  ((temp.m[0][0] * temp.m[2][2]) - (temp.m[2][0] * temp.m[0][2]));
    m[2][1] = -((temp.m[0][0] * temp.m[2][1]) - (temp.m[2][0] * temp.m[0][1]));
    m[0][2] =  ((temp.m[0][1] * temp.m[1][2]) - (temp.m[1][1] * temp.m[0][2]));
    m[1][2] = -((temp.m[0][0] * temp.m[1][2]) - (temp.m[1][0] * temp.m[0][2]));
    m[2][2] =  ((temp.m[0][0] * temp.m[1][1]) - (temp.m[1][0] * temp.m[0][1]));

    *this /= this->determinant();
  }

  const float
  Matrix3x3::determinant() const {
    return (((m[0][0] * m[1][1] * m[2][2]) +
             (m[1][0] * m[2][1] * m[0][2]) + 
             (m[2][0] * m[0][1] * m[1][2])) -

            ((m[0][2] * m[1][1] * m[2][0]) + 
             (m[1][2] * m[2][1] * m[0][0]) + 
             (m[2][2] * m[0][1] * m[1][0])));
  }

  String
  Matrix3x3::toString() {
    return Utils::format("[(%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f), \t (%2.2f)] \n", 
                         m[0][0], m[0][1], m[0][2],
                         m[1][0], m[1][1], m[1][2],
                         m[2][0], m[2][1], m[2][2]);
  }

  const Matrix3x3 Matrix3x3::ZERO     = Matrix3x3(0.0f);

  const Matrix3x3 Matrix3x3::IDENTITY = Matrix3x3(1.0f, 0.0f, 0.0f, 
                                                  0.0f, 1.0f, 0.0f, 
                                                  0.0f, 0.0f, 1.0f);
}
