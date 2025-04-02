
// Defining values for unit testing
#include <cyCrashHandler.h>
#include <cyColor.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix4x4.h>
#include <cyMatrix3x3.h>
#include <cyTime.h>
#include <cyUnitTesting.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

#undef near
#undef far

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
TEST_SUITE("Matrix4x4 Tests") {
  TEST_CASE("Constructors") {
    SUBCASE("Default Constructor") {
      Matrix4x4 m(0);
      CHECK(m == Matrix4x4::ZERO);
    }

    SUBCASE("Value Constructor") {
      Matrix4x4 m(5.0f);
      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          CHECK(m.m[c][r] == 5.0f);
        }
      }
    }

    SUBCASE("Copy Constructor") {
      Matrix4x4 m1(1.0f);
      Matrix4x4 m2(m1);
      CHECK(m1 == m2);
    }

    SUBCASE("From Matrix3x3") {
      Matrix3x3 m3(1, 2, 3, 4, 5, 6, 7, 8, 9);
      Matrix4x4 m4(m3);
      CHECK(m4.m[0][0] == 1.0f);
      CHECK(m4.m[1][0] == 4.0f);
      CHECK(m4.m[2][0] == 7.0f);
      CHECK(m4.m[3][3] == 0.0f);
    }
  }

  TEST_CASE("Basic Operations") {
    Matrix4x4 m1( 1,  2,  3,  4, 
                  5,  6,  7,  8, 
                  9, 10, 11, 12, 
                 13, 14, 15, 16);
    Matrix4x4 m2(16, 15, 14, 13, 
                 12, 11, 10,  9, 
                  8,  7,  6,  5, 
                  4,  3,  2,  1);

    SUBCASE("Addition") {
      Matrix4x4 result = m1 + m2;
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          CHECK(result.m[i][j] == 17.0f);
        }
      }
    }

    SUBCASE("Subtraction") {
      Matrix4x4 result = m1 - m2;
      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          float expected = (1 + r * 4 + c) - (16 - r * 4 - c);
          CHECK(result.m[c][r] == doctest::Approx(expected));
        }
      }
    }

    SUBCASE("Multiplication") {
      Matrix4x4 identity = Matrix4x4::IDENTITY;
      Matrix4x4 result = m1 * identity;
     
      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          CHECK(result.m[c][r] == doctest::Approx(m1.m[c][r]));
        }
      }

      result = identity * m2;
      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          CHECK(result.m[c][r] == doctest::Approx(m2.m[c][r]));
        }
      }

      Matrix4x4 expectedProduct(
        80, 70, 60, 50,
        240, 214, 188, 162,
        400, 358, 316, 274,
        560, 502, 444, 386
      );
      std::cout << m1.toString() << std::endl;
      std::cout << m2.toString() << std::endl;
      result = m1 * m2;
      // CHECK(result == expectedProduct);
      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          CHECK(result.m[c][r] == doctest::Approx(expectedProduct.m[c][r]));
        }
      }

    }
  }

  TEST_CASE("Transformation Functions") {
    SUBCASE("Translation") {
      Matrix4x4 m;
      m.setPosition(Vector3f(1, 2, 3));
      CHECK(m.getPosition() == Vector3f(1, 2, 3));

      m.translate(Vector3f(4, 5, 6));
      CHECK(m.getPosition() == Vector3f(5, 7, 9));
    }

    SUBCASE("Rotation") {
      Matrix4x4 m(0);
      m.identity();
      m.rotateX(Math::PI / 2);
      Vector3f v = m.transformDirection(Vector3f(0, 1, 0));
      CHECK(v.x == doctest::Approx(0.0f));
      CHECK(v.y == doctest::Approx(0.0f));
      CHECK(v.z == doctest::Approx(1.0f));

      m.identity();
      m.rotateY(Math::PI / 2);
      v = m.transformDirection(Vector3f(0, 0, 1));
      CHECK(v.x == doctest::Approx(1.0f));
      CHECK(v.y == doctest::Approx(0.0f));
      CHECK(v.z == doctest::Approx(0.0f));

      m.identity();
      m.rotateZ(Math::PI / 2);
      v = m.transformDirection(Vector3f(1, 0, 0));
      CHECK(v.x == doctest::Approx(0.0f));
      CHECK(v.y == doctest::Approx(1.0f));
      CHECK(v.z == doctest::Approx(0.0f));
    }

    SUBCASE("Scaling") {
      Matrix4x4 m(0);
      m.setScale(Vector3f(2, 3, 4));
      Vector3f v = m.transformDirection(Vector3f(1, 1, 1));
      CHECK(v == Vector3f(2, 3, 4));

      m.scale(0.5f);
      v = m.transformDirection(Vector3f(1, 1, 1));
      CHECK(v == Vector3f(1, 1.5f, 2));
    }
  }

  TEST_CASE("Matrix Operations") {
    Matrix4x4 m(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 4, 5, 6, 1);

    SUBCASE("Transpose") {
      Matrix4x4 transposed = m.transposed();
      CHECK(transposed.m[0][0] == 1.0f);
      CHECK(transposed.m[1][0] == 0.0f);
      CHECK(transposed.m[0][1] == 0.0f);
      CHECK(transposed.m[3][0] == 4.0f);
    }

    SUBCASE("Determinant") {
      CHECK(m.determinant() == doctest::Approx(6.0f));

      Matrix4x4 singular(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
      CHECK(singular.determinant() == doctest::Approx(0.0f));
    }

    SUBCASE("Inverse") {
      Matrix4x4 inv = m.inversed();
      Matrix4x4 identity = m * inv;

      for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
          if (c == r) {
            CHECK(identity.m[c][r] == doctest::Approx(1.0f));
          }
          else {
            CHECK(identity.m[c][r] == doctest::Approx(0.0f));
          }
        }
      }
    }
  }

  TEST_CASE("Special Matrices") {
    SUBCASE("View Matrix") {
      Matrix4x4 view;
      view.View(Vector3f(0, 0, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

      Vector3f pos = view.transformPosition(Vector3f(0, 0, 0));
      CHECK(pos.x == doctest::Approx(0.0f));
      CHECK(pos.y == doctest::Approx(0.0f));
      CHECK(pos.z == doctest::Approx(5.0f));
    }

    SUBCASE("Perspective Projection") {
      Matrix4x4 proj(0);
      proj.Perspective(800, 600, 0.1f, 100.0f, Math::PI / 2);

      // Test near plane
      Vector3f nearV = proj.transformPosition(Vector3f(0, 0, -0.1f));
      CHECK(nearV.z == doctest::Approx(-1.0f));

      // Test far plane
      Vector3f farV = proj.transformPosition(Vector3f(0, 0, -100.0f));
      CHECK(farV.z == doctest::Approx(1.0f));


    }

    SUBCASE("Orthographic Projection") {
      Matrix4x4 ortho;
      ortho.Orthogonal(800, 600, 0.1f, 100.0f);

      Vector3f bottomLeft = ortho.transformPosition(Vector3f(-400, -300, -50));
      CHECK(bottomLeft.x == doctest::Approx(-1.0f));
      CHECK(bottomLeft.y == doctest::Approx(-1.0f));
      CHECK(bottomLeft.z == doctest::Approx(0.0f));
    }
  }

  TEST_CASE("Utility Functions") {
    Matrix4x4 m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

    SUBCASE("Identity") {
      m.identity();
      CHECK(m == Matrix4x4::IDENTITY);
    }

    SUBCASE("Zero") {
      m.zero();
      CHECK(m == Matrix4x4::ZERO);
    }

    SUBCASE("Get Vectors") {
      CHECK(m.getRightVector() == Vector3f(1, 2, 3).normalized());
      CHECK(m.getUpVector() == Vector3f(5, 6, 7).normalized());
      CHECK(m.getForwardVector() == Vector3f(9, 10, 11).normalized());
      CHECK(m.getPosition() == Vector3f(13, 14, 15));
    }

    SUBCASE("Submatrix") {
      Matrix3x3 sub = m.subMatrix();
      CHECK(sub.m[0][0] == 1.0f);
      CHECK(sub.m[1][1] == 6.0f);
      CHECK(sub.m[2][2] == 11.0f);
    }
  }
}
