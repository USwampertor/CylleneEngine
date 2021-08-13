#include "cyMatrix2x2.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {
  Matrix2x2::Matrix2x2(const int32& value) {
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
    Matrix2x2 temp = ZERO;
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        temp.m[i][j] = m[i][j] + b.m[i][j];
      }
    }
    return temp;
  }

  Matrix2x2
  Matrix2x2::operator-(const Matrix2x2& b) {
    Matrix2x2 temp = ZERO;
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        temp.m[i][j] = m[i][j] - b.m[i][j];
      }
    }
    return temp;
  }

  Matrix2x2
  Matrix2x2::operator*(const Matrix2x2& b) {
    Matrix2x2 temp = ZERO;
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        for (uint32 k = 0; k < 3; ++k) {
          temp.m[i][j] += m[i][k] * b.m[k][j];
        }
      }
    }
    return temp;
  }

  Matrix2x2&
  Matrix2x2::operator+=(const Matrix2x2& b) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] += b.m[i][j];
      }
    }
    return *this;
  }

  Matrix2x2&
  Matrix2x2::operator-=(const Matrix2x2& b) {
    for (uint32 i = 0; i < 2; ++i) {
      for (uint32 j = 0; j < 2; ++j) {
        m[i][j] -= b.m[i][j];
      }
    }
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

  }

  void
  Matrix2x2::zero() {

  }

  Matrix2x2
  Matrix2x2::transposed() const {

  }

  void
  Matrix2x2::transpose() {

  }

  void
  Matrix2x2::setValues(const float& value) {

  }

  void
  Matrix2x2::setValues(const float& v00, const float& v01,
                       const float& v10, const float& v11) {

  }

  Matrix2x2
  Matrix2x2::inversed() {

  }

  void
  Matrix2x2::inverse() {

  }

  const float
  Matrix2x2::determinant() const {

  }

  String
  Matrix2x2::toString() {
  
  }

  const Matrix2x2 Matrix2x2::ZERO     = Matrix2x2(0.0f);

  const Matrix2x2 Matrix2x2::IDENTITY = Matrix2x2(1.0f, 0.0f, 1.0f, 0.0f);
}