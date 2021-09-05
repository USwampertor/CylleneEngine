#include "cyMatrix2x2.h"
#include "cyUtilities.h"
#include "cyMath.h"
namespace CYLLENE_SDK {
  Matrix2x2::Matrix2x2(const float& value) {
    memset(this, value, sizeof(Matrix2x2));
  }

  Matrix2x2::Matrix2x2(const Matrix2x2& other)
    : _m(other._m) {}

  Matrix2x2::Matrix2x2(const float& v00, const float& v01,
                       const float& v10, const float& v11) {
    _m.m00 = v00; _m.m01 = v01;
    _m.m10 = v10; _m.m11 = v11;
  }

  Matrix2x2
  Matrix2x2::operator+(const Matrix2x2& b) {
    // Matrix2x2 temp = ZERO;
    // for (uint32 i = 0; i < 2; ++i) {
    //   for (uint32 j = 0; j < 2; ++j) {
    //     temp.m[i][j] = m[i][j] + b.m[i][j];
    //   }
    // }
    // return temp;

    return Matrix2x2(_m.m00 + b._m.m00, _m.m01 + b._m.m01,
                     _m.m10 + b._m.m10, _m.m11 + b._m.m11);

  }

  Matrix2x2
  Matrix2x2::operator-(const Matrix2x2& b) {
    // Matrix2x2 temp = ZERO;
    // for (uint32 i = 0; i < 2; ++i) {
    //   for (uint32 j = 0; j < 2; ++j) {
    //     temp.m[i][j] = m[i][j] - b.m[i][j];
    //   }
    // }
    // return temp;
  
    return Matrix2x2(_m.m00 - b._m.m00, _m.m01 - b._m.m01,
                     _m.m10 - b._m.m10, _m.m11 - b._m.m11);
  }

  Matrix2x2
  Matrix2x2::operator*(const Matrix2x2& b) {
    Matrix2x2 temp = ZERO;
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        for (uint32 k = 0; k < 2; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    return temp;
  }

  Matrix2x2&
  Matrix2x2::operator+=(const Matrix2x2& b) {
    _m.m00 += b._m.m00; _m.m01 += b._m.m01;
    _m.m10 += b._m.m10; _m.m11 += b._m.m11;

    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator-=(const Matrix2x2& b) {
    _m.m00 -= b._m.m00; _m.m01 -= b._m.m01;
    _m.m10 -= b._m.m10; _m.m11 -= b._m.m11;

    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator*=(const Matrix2x2& b) {
    Matrix2x2 temp = ZERO;
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        for (uint32 k = 0; k < 2; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    *this = temp;
    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator+=(const float& value) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] += value;
      }
    }
    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator-=(const float& value) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] -= value;
      }
    }
    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator*=(const float& value) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] *= value;
      }
    }
    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator/=(const float& value) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] /= value;
      }
    }
    return *this;
  }

  bool
  Matrix2x2::operator==(const Matrix2x2& b) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        if (m[i][j] != b.m[i][j]) { return false; }
      }
    }
    return true;
  }

  void
  Matrix2x2::identity() {
    *this = Matrix2x2::IDENTITY;
  }

  void
  Matrix2x2::zero() {
    *this = Matrix2x2::ZERO;
  }

  Matrix2x2
  Matrix2x2::transposed() const {
    return Matrix2x2(m[0][0], m[1][0],
                     m[0][1], m[1][1]);
  }

  void
  Matrix2x2::transpose() {
    *this = this->transposed();
  }

  Matrix2x2
  Matrix2x2::cofactored() const {
    return Matrix2x2(m[1][1], -m[1][0],
                     -m[0][1], m[0][0]);
  }

  void
  Matrix2x2::cofactor() {
    *this = this->cofactored();
  }

  void
  Matrix2x2::setValues(const float& value) {
    m[0][0] = m[0][1] = 
    m[1][0] = m[1][1] = value;
  }

  void
  Matrix2x2::setValues(const float& v00, const float& v01,
                       const float& v10, const float& v11) {
    m[0][0] = v00; m[0][1] = v01;
    m[1][0] = v10; m[1][1] = v11;
  }

  Matrix2x2
  Matrix2x2::inversed() {
    CY_ASSERT(this->determinant() != 0.0f &&
              Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    Matrix2x2 temp = ZERO;
    temp = this->cofactored();
    temp.transpose();
    // temp.m[0][0] = m[1][1]; temp.m[0][1] *= -1.0f;
    // temp.m[1][0] *= -1.0f; temp.m[1][1] = m[0][0];
    temp *= Math::pow(this->determinant(), -1.0f);

    return temp;
  }

  void
  Matrix2x2::inverse() {
    *this = this->inversed();
    // CY_ASSERT(this->determinant() != 0.0f &&
    //           Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
    // // m[0][0] = m[1][1]; m[0][1] *= -1.0f;
    // // m[1][0] *= -1.0f; m[1][1] = m[0][0];
    // this->cofactor();
    // this->transpose();
    // *this /= this->determinant();
  }

  const float
  Matrix2x2::determinant() const {
    return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
  }

  String
  Matrix2x2::toString() {
    return Utils::format("[(%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f)] \n", 
                         m[0][0], m[0][1], 
                         m[1][0], m[1][1]);
  }

  const Matrix2x2 Matrix2x2::ZERO     = Matrix2x2(0.0f);

  const Matrix2x2 Matrix2x2::IDENTITY = Matrix2x2(1.0f, 0.0f, 
                                                  0.0f, 1.0f);
}