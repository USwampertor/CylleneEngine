#include "cyMatrix2.h"
#include "cyUtilities.h"
#include "cyMath.h"
namespace CYLLENE_SDK {
Matrix2::Matrix2(const float& value) {
  memset(this, static_cast<int32>(value), sizeof(Matrix2));
}

Matrix2::Matrix2(const Matrix2& other)
  : _m(other._m) {}

Matrix2::Matrix2(const float& m00, const float& m10,
                  const float& m01, const float& m11) {
  _m.m00 = m00; _m.m10 = m10;
  _m.m01 = m01; _m.m11 = m11;
}

Matrix2
Matrix2::operator+(const Matrix2& b) {
  // Matrix2x2 temp = ZERO;
  // for (uint32 i = 0; i < 2; ++i) {
  //   for (uint32 j = 0; j < 2; ++j) {
  //     temp.m[i][j] = m[i][j] + b.m[i][j];
  //   }
  // }
  // return temp;

  return Matrix2(_m.m00 + b._m.m00, _m.m10 + b._m.m10,
                  _m.m01 + b._m.m01, _m.m11 + b._m.m11);

}

Matrix2
Matrix2::operator-(const Matrix2& b) {
  // Matrix2x2 temp = ZERO;
  // for (uint32 i = 0; i < 2; ++i) {
  //   for (uint32 j = 0; j < 2; ++j) {
  //     temp.m[i][j] = m[i][j] - b.m[i][j];
  //   }
  // }
  // return temp;
  
  return Matrix2(_m.m00 - b._m.m00, _m.m10 - b._m.m10,
                  _m.m01 - b._m.m01, _m.m11 - b._m.m11);
}

Matrix2
Matrix2::operator*(const Matrix2& b) {
  Matrix2 tmp = ZERO;
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      tmp.m[c][r] = 0;
      for (uint32 k = 0; k < 2; ++k) {
        tmp.m[c][r] += m[k][r] * b.m[c][k];
      }
    }
  }
  return tmp;
}

Matrix2&
Matrix2::operator+=(const Matrix2& b) {
  *this = *this + b;
  return *this;
}

Matrix2&
Matrix2::operator-=(const Matrix2& b) {
  *this = *this - b;
  return *this;
}

Matrix2&
Matrix2::operator*=(const Matrix2& b) {
  *this = *this * b;
  return *this;
}

Matrix2&
Matrix2::operator+=(const float& value) {
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      m[c][r] += value;
    }
  }
  return *this;
}

Matrix2&
Matrix2::operator-=(const float& value) {
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      m[c][r] -= value;
    }
  }
  return *this;
}

Matrix2&
Matrix2::operator*=(const float& value) {
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      m[c][r] *= value;
    }
  }
  return *this;
}

Matrix2&
Matrix2::operator/=(const float& value) {
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      m[c][r] /= value;
    }
  }
  return *this;
}

bool
Matrix2::operator==(const Matrix2& b) {
  for (uint32 c = 0; c < 2; ++c) {
    for (uint32 r = 0; r < 2; ++r) {
      if (m[c][r] != b.m[c][r]) { return false; }
    }
  }
  return true;
}

void
Matrix2::identity() {
  *this = Matrix2::IDENTITY;
}

void
Matrix2::zero() {
  *this = Matrix2::ZERO;
}

Matrix2
Matrix2::transposed() const {
  return Matrix2(m[0][0], m[0][1],
                  m[1][0], m[1][1]);
}

void
Matrix2::transpose() {
  *this = this->transposed();
}

Matrix2
Matrix2::cofactored() const {
  return Matrix2(m[1][1], -m[0][1],
                -m[1][0],  m[0][0]);
}

void
Matrix2::cofactor() {
  *this = this->cofactored();
}

void
Matrix2::setValues(const float& value) {
  m[0][0] = m[1][0] = 
  m[0][1] = m[1][1] = value;
}

void
Matrix2::setValues(const float& m00, const float& m10,
                   const float& m01, const float& m11) {
  m[0][0] = m00; m[1][0] = m10;
  m[0][1] = m01; m[1][1] = m11;
}

Matrix2
Matrix2::inversed() {
  CY_ASSERT(this->determinant() != 0.0f &&
            Utils::format("The determinant for matrix \n%s is 0!", this->toString()).c_str());
  Matrix2 temp = ZERO;
  temp = this->cofactored();
  temp.transpose();
  temp *= Math::pow(this->determinant(), -1.0f);

  return temp;
}

void
Matrix2::inverse() {
  *this = this->inversed();
}

const float
Matrix2::determinant() const {
  return (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
}

String
Matrix2::toString() {
  return Utils::format("[(%2.2f), \t (%2.2f)]\n[(%2.2f), \t (%2.2f)] \n", 
                        m[0][0], m[1][0], 
                        m[0][1], m[1][1]);
}

const Matrix2 Matrix2::ZERO     = Matrix2(0.0f);

const Matrix2 Matrix2::IDENTITY = Matrix2(1.0f, 0.0f, 
                                          0.0f, 1.0f);
}