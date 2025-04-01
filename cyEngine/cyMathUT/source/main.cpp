
// Defining values for unit testing
#include <cyCrashHandler.h>
#include <cyColor.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix4x4.h>
#include <cyTime.h>
#include <cyUnitTesting.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

// Using namespace for ease of use
using namespace CYLLENE_SDK;

// Helper function to compare matrices
bool matrixEquals(const Matrix4x4& a, const Matrix4x4& b) {
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      if (std::abs(a.m[col][row] - b.m[col][row]) > Math::KINDASMALLNUMBER)
        return false;
    }
  }
  return true;
}

// Helper to create translation matrix
Matrix4x4 makeTranslationMatrix(float x, float y, float z) {
  Matrix4x4 m = Matrix4x4::IDENTITY;
  m.m[3][0] = x;
  m.m[3][1] = y;
  m.m[3][2] = z;
  return m;
}

/*
 *	@brief  Unit Testing main for Math
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[]) {

  CrashHandler::startUp();
  Logger::startUp();
  Time::startUp();

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}

TEST_CASE("[math] Modf") {
  CHECK(3 == Math::mod(3, 4));
  CHECK(0 != Math::mod(3, 4));
}

TEST_CASE("[math] Testing fast math functions") {

  float sin = 0.0f;

  float fastSin = 0.0f;

  Benchmark b;

  b.run("sin", [&]() {
    sin = Math::sin(1.0f);
    DONOTOPTIMIZE(sin);
  });

  b.run("fast sin", [&]() {
    fastSin = Math::fastsin(1.0f);
    DONOTOPTIMIZE(fastSin);
  });

  CHECK(sin == fastSin);

  float cos = 0.0f;

  float fastCos = 0.0f;

  b.run("cos", [&]() {
    cos = Math::cos(0.0f);
    DONOTOPTIMIZE(cos);
  });

  b.run("fast cos", [&]() {
    fastCos = Math::fastcos(0.0f);
    DONOTOPTIMIZE(fastCos);
  });

  CHECK(cos == fastCos);

  float tan = 0.0f;

  float fastTan = 0.0f;

  b.run("tan", [&]() {
    cos = Math::tan(0.0f);
    DONOTOPTIMIZE(tan);
  });

  b.run("fast tan", [&]() {
    fastCos = Math::fasttan(0.0f);
    DONOTOPTIMIZE(fastTan);
  });

  CHECK(tan == fastTan);

}

TEST_CASE("[vector2i] Testing vector2 functionality") {
//   Vector2i v0(0, 0);
//   CHECK(v0[0] == 0);
//   v0 = { 2, 4 };
//   
//   v0.normalize();
//   CHECK(v0.magnitude() == 0);
// 
// 
//   Vector2i v1(-5, -6);
//   float d = Vector2i::distance(v0, v1);
//   CHECK(d == 3.61f);
}

TEST_CASE("[vector2f] Testing vector2 functionality") {
  Vector2f v0(0, 0);
  CHECK(v0[0] == 0);
  v0 = { 2, 4 };

  CHECK(v0.x == 2);
  CHECK(v0.y == 4);

  v0.normalize();
  CHECK(Math::isNearSame(v0.magnitude(), 1.0f, Math::EPSILONF));

  v0.setValues(5, 6);
  
  CHECK(v0.getLowest() == Math::min(v0.x, v0.y));

  CHECK(v0.getHighest() == Math::max(v0.x, v0.y));

  CHECK(v0.sqrMagnitude() == 61);
  CHECK(v0.magnitude() == Math::sqrt(61.0f));

  BENCHMARK("normalized", [&]() {
    DONOTOPTIMIZE(v0.normalized());
  });

  BENCHMARK("quick normalized", [&]() {
    DONOTOPTIMIZE(v0.qNormalized());
  });
  bool areSame = Vector2f::isNearlySame(v0.normalized(), v0.qNormalized(), 0.0001f);
  CHECK(areSame);

  Vector2f v1(7, 8);
  float d = Vector2f::distance(v0, v1);
  CHECK(Math::isNearSame(d, 2.82842712f, Math::EPSILONF));
}

TEST_CASE("[vector3f] Testing vector5 functionality") {
  Vector3f v0(0, 0, 0);
  CHECK(v0[0] == 0);
  v0 = { 2, 4, 5 };

  CHECK(v0.x == 2);
  CHECK(v0.y == 4);
  CHECK(v0.z == 5);

  v0.normalize();
  CHECK(Math::isNearSame(v0.magnitude(), 1.0f, Math::EPSILONF));

  v0.setValues(5, 6, 0);

  CHECK(v0.getLowest() == 0);

  CHECK(v0.getHighest() == 6);

  CHECK(v0.sqrMagnitude() == 61);
  CHECK(v0.magnitude() == Math::sqrt(61.0f));

  BENCHMARK("normalized", [&]() {
    DONOTOPTIMIZE(v0.normalized());
  });

  BENCHMARK("quick normalized", [&]() {
    DONOTOPTIMIZE(v0.qNormalized());
  });
  bool areSame = Vector3f::areNearlySame(v0.normalized(), v0.qNormalized(), 0.0001f);
  CHECK(areSame);

  Vector3f v1(7, 8, 10);
  float d = Vector3f::distance(v0, v1);
  CHECK(Math::isNearSame(d, 10.39230484f, Math::EPSILONF));
}

TEST_CASE("[vector4f] Testing vector4 functionality") {

}

TEST_CASE("[quaternion] Testing quaternion functionality") {

}

TEST_CASE("[color] Testing color to hex") {
  Color c1;
  c1.setUint(33, 127, 63, 127);
  uint32 hex1 = c1.toHexValue();
  CHECK(hex1 == 0x217f3f7f);
  Color comp1;
  comp1.fromHex(hex1);
  CHECK(c1 == comp1);
  Color c2;
  c2.setFloat(0, 0, 0, 1);
  CHECK(c2.toHexValue() == 0x000000ff);
}

TEST_CASE("Matrix Construction") {
  SUBCASE("Default Constructor") {
    Matrix4x4 m(0);
    CHECK(matrixEquals(m, Matrix4x4::ZERO));
  }

  SUBCASE("Identity Matrix") {
    Matrix4x4 m = Matrix4x4::IDENTITY;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        CHECK(m.m[i][j] == (i == j ? 1.0f : 0.0f));
      }
    }
  }
}

TEST_CASE("Matrix Operations") {
  Matrix4x4 a(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  Matrix4x4 b(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);

  SUBCASE("Matrix Addition") {
    Matrix4x4 result = a + b;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        CHECK(result.m[i][j] == a.m[i][j] + b.m[i][j]);
      }
    }
  }

  SUBCASE("Matrix Multiplication") {
    Matrix4x4 result = a * b;
    // Manually calculated expected value for [0][0]
    CHECK(result.m[0][0] == doctest::Approx(1 * 16 + 2 * 12 + 3 * 8 + 4 * 4));
  }

  SUBCASE("Matrix Transpose") {
    Matrix4x4 transposed = a;
    transposed.transpose();
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        CHECK(transposed.m[i][j] == a.m[j][i]);
      }
    }
  }
}

TEST_CASE("Transformations") {
  SUBCASE("Translation") {
    Matrix4x4 m = Matrix4x4::IDENTITY;
    m.translate(Vector3f(2, 3, 4));
    Matrix4x4 expected = makeTranslationMatrix(2, 3, 4);
    CHECK(matrixEquals(m, expected));
  }

  SUBCASE("Rotation (Z-Axis)") {
    Matrix4x4 m;
    m.rotateZ(Math::PI / 2); // 90 degrees

    // Expected rotation matrix
    Matrix4x4 expected;
    expected.m[0][0] = 0;  expected.m[0][1] = 1;
    expected.m[1][0] = -1; expected.m[1][1] = 0;
    expected.m[2][2] = 1;  expected.m[3][3] = 1;

    CHECK(matrixEquals(m, expected));
  }

  SUBCASE("Scaling") {
    Matrix4x4 m;
    m.setScale(Vector3f(2, 3, 4));
    CHECK(m.m[0][0] == 2.0f);
    CHECK(m.m[1][1] == 3.0f);
    CHECK(m.m[2][2] == 4.0f);
  }
}

TEST_CASE("View and Projection") {
  SUBCASE("View Matrix") {
    Vector4f eye(0, 0, 5, 1);
    Vector4f target(0, 0, 0, 1);
    Vector4f up(0, 1, 0, 0);

    Matrix4x4 view;
    view.View(eye, target, up);

    // Should translate world backwards along Z
    CHECK(view.m[3][2] == doctest::Approx(-5.0f));
  }

  SUBCASE("Perspective Projection") {
    Matrix4x4 proj;
    proj.Perspective(800, 600, 0.1f, 100.0f, Math::PI / 2);

    // Check perspective divide term
    CHECK(proj.m[2][3] == doctest::Approx(-1.0f));
  }
}

TEST_CASE("Basis Vectors") {
  Matrix4x4 m;
  m.rotateY(Math::PI / 4); // 45 degrees

  SUBCASE("Forward Vector") {
    Vector3f fwd = m.getForwardVector();
    CHECK(fwd.x == doctest::Approx(sqrt(2) / 2));
    CHECK(fwd.z == doctest::Approx(-sqrt(2) / 2));
  }

  SUBCASE("Right Vector") {
    Vector3f right = m.getRightVector();
    CHECK(right.x == doctest::Approx(1.0f));
  }
}

TEST_CASE("Edge Cases") {
  SUBCASE("Inverse of Singular Matrix") {
    Matrix4x4 singular; // All zeros
    CHECK_THROWS(singular.inversed());
  }

  SUBCASE("Transform Position with Zero Matrix") {
    Matrix4x4 zero = Matrix4x4::ZERO;
    Vector3f v(1, 2, 3);
    CHECK(zero.transformPosition(v) == Vector3f(0, 0, 0));
  }
}