
// Defining values for unit testing
#include <cyCrashHandler.h>
#include <cyCapsule.h>
#include <cyColor.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix4.h>
#include <cyMatrix3.h>
#include <cyOBB.h>
#include <cyTime.h>
#include <cyUnitTesting.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>
#include <cyPoint.h>
#include <cyRect.h>

#undef near
#undef far

// Using namespace for ease of use
using namespace CYLLENE_SDK;

// Helper function to compare matrices
bool matrixEquals(const Matrix4& a, const Matrix4& b) {
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      if (std::abs(a.m[col][row] - b.m[col][row]) > Math::KINDASMALLNUMBER)
        return false;
    }
  }
  return true;
}

// Helper to create translation matrix
Matrix4 makeTranslationMatrix(float x, float y, float z) {
  Matrix4 m = Matrix4::IDENTITY;
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

TEST_CASE("[vector3f] Testing vector3 functionality") {
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

TEST_SUITE("Matrix4 Tests") {
  TEST_CASE("Construction and Identity") {
    Matrix4 m1;
    m1.identity();

    CHECK(m1 == Matrix4::IDENTITY);
    CHECK(m1.m[0][0] == 1.0f);
    CHECK(m1.m[1][1] == 1.0f);
    CHECK(m1.m[2][2] == 1.0f);
    CHECK(m1.m[3][3] == 1.0f);

    Matrix4 m2(0.0f);
    CHECK(m2 == Matrix4::ZERO);
  }

  TEST_CASE("Matrix Multiplication") {
    Matrix4 m1(1, 2, 3, 4,
      5, 6, 7, 8,
      9, 10, 11, 12,
      13, 14, 15, 16);

    Matrix4 m2(16, 15, 14, 13,
      12, 11, 10, 9,
      8, 7, 6, 5,
      4, 3, 2, 1);

    Matrix4 result = m1 * m2;
    Matrix4 expected(80, 70, 60, 50,
      240, 214, 188, 162,
      400, 358, 316, 274,
      560, 502, 444, 386);

    CHECK(result == expected);
  }

  TEST_CASE("Translation") {
    Matrix4 m;
    m.identity();
    m.translate(Vector3f(5, 3, 2));

    CHECK(m.m[3][0] == 5.0f);
    CHECK(m.m[3][1] == 3.0f);
    CHECK(m.m[3][2] == 2.0f);

    Vector3f pos = m.getPosition();
    CHECK(pos.x == 5.0f);
    CHECK(pos.y == 3.0f);
    CHECK(pos.z == 2.0f);
  }

  TEST_CASE("Rotation") {
    SUBCASE("Fixed X-Axis Rotation by 90 degrees") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotateX(Math::degToRad(90.0f));

      Vector3f forward = m.getForwardVector();
      Vector3f right = m.getRightVector();
      Vector3f up = m.getUpVector();

      CHECK(forward.x == doctest::Approx(0.0f));
      CHECK(forward.y == doctest::Approx(1.0f));
      CHECK(forward.z == doctest::Approx(0.0f));

      CHECK(right.x == doctest::Approx(1.0f));
      CHECK(right.y == doctest::Approx(0.0f));
      CHECK(right.z == doctest::Approx(0.0f));

      CHECK(up.x == doctest::Approx(0.0f));
      CHECK(up.y == doctest::Approx(0.0f));
      CHECK(up.z == doctest::Approx(-1.0f));
    }

    SUBCASE("Arbitrary X-Axis Rotation by -33 degrees") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotate(-33.0f, 1.0f, 0.0f, 0.0f);

      Vector3f forward = m.getForwardVector();
      Vector3f right = m.getRightVector();
      Vector3f up = m.getUpVector();

      CHECK(forward.x == doctest::Approx(0.0f));
      CHECK(forward.y == doctest::Approx(0.54464f));
      CHECK(forward.z == doctest::Approx(0.83867));

      CHECK(right.x == doctest::Approx(1.0f));
      CHECK(right.y == doctest::Approx(0.0f));
      CHECK(right.z == doctest::Approx(0.0f));

      CHECK(up.x == doctest::Approx(0.0f));
      CHECK(up.y == doctest::Approx(0.83867f));
      CHECK(up.z == doctest::Approx(-0.54464f));
    }

    SUBCASE("Fixed Y-Axis Rotation by 90 degrees") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotateY(Math::degToRad(90.0f));

      Vector3f forward = m.getForwardVector();
      Vector3f right = m.getRightVector();
      Vector3f up = m.getUpVector();

      CHECK(forward.x == doctest::Approx(-1.0f));
      CHECK(forward.y == doctest::Approx(0.0f));
      CHECK(forward.z == doctest::Approx(0.0f));

      CHECK(right.x == doctest::Approx(0.0f));
      CHECK(right.y == doctest::Approx(0.0f));
      CHECK(right.z == doctest::Approx(1.0f));

      CHECK(up.x == doctest::Approx(0.0f));
      CHECK(up.y == doctest::Approx(1.0f));
      CHECK(up.z == doctest::Approx(0.0f));
    }

    SUBCASE("Fixed Z-Axis Rotation by 90 degrees") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotateZ(Math::degToRad(90.0f));

      Vector3f forward = m.getForwardVector();
      Vector3f right = m.getRightVector();
      Vector3f up = m.getUpVector();

      CHECK(forward.x == doctest::Approx(0.0f));
      CHECK(forward.y == doctest::Approx(0.0f));
      CHECK(forward.z == doctest::Approx(1.0f));

      CHECK(right.x == doctest::Approx(0.0f));
      CHECK(right.y == doctest::Approx(-1.0f));
      CHECK(right.z == doctest::Approx(0.0f));

      CHECK(up.x == doctest::Approx(1.0f));
      CHECK(up.y == doctest::Approx(0.0));
      CHECK(up.z == doctest::Approx(0.0f));
    }

    SUBCASE("Fixed Z-Axis Rotation by 50 degrees") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotateZ(Math::degToRad(50.0f));

      Vector3f forward = m.getForwardVector();
      Vector3f right = m.getRightVector();
      Vector3f up = m.getUpVector();

      CHECK(forward.x == doctest::Approx(0.0f));
      CHECK(forward.y == doctest::Approx(0.0f));
      CHECK(forward.z == doctest::Approx(1.0f));

      CHECK(right.x == doctest::Approx(0.64279f));
      CHECK(right.y == doctest::Approx(-0.76604f));
      CHECK(right.z == doctest::Approx(0.0f));

      CHECK(up.x == doctest::Approx(0.76604f));
      CHECK(up.y == doctest::Approx(0.64279f));
      CHECK(up.z == doctest::Approx(0.0f));
    }

    SUBCASE("Arbitrary Axis Rotation") {
      Matrix4 m = Matrix4::IDENTITY;
      m.rotate(90.0f, Vector3f(1, 1, 0).normalized());

      Vector3f forward = m.getForwardVector();
      CHECK(forward.x == doctest::Approx(0.7071f).epsilon(0.01f));
      CHECK(forward.y == doctest::Approx(-0.7071f).epsilon(0.01f));
      CHECK(forward.z == doctest::Approx(0.0f).epsilon(0.01f));
    }
  }

  TEST_CASE("Scaling") {
    Matrix4 m;
    m.identity();
    m.setScale(Vector3f(2, 3, 4));

    Vector3f s = m.getScale();

    CHECK(m.m[0][0] == 2.0f);
    CHECK(m.m[1][1] == 3.0f);
    CHECK(m.m[2][2] == 4.0f);

    CHECK(s.x == 2.0f);
    CHECK(s.y == 3.0f);
    CHECK(s.z == 4.0f);

    m.scale(Vector3f(0.5f, 1.0f, 0.25f));

    CHECK(m.m[0][0] == 1.0f);
    CHECK(m.m[1][1] == 3.0f);
    CHECK(m.m[2][2] == 1.0f);

    s = m.getScale();

    CHECK(s.x == 1.0f);
    CHECK(s.y == 3.0f);
    CHECK(s.z == 1.0f);
  }

  TEST_CASE("Extract Rotation (Quaternion) from Matrix4") {
    SUBCASE("No Rotation (Identity)") {
      CYLLENE_SDK::Matrix4 m; // Identity matrix
      m.identity();

      CYLLENE_SDK::Quaternion q = m.getQuatRotation();
      CHECK(q.w == doctest::Approx(1.0f)); // Identity quaternion
      CHECK(q.x == doctest::Approx(0.0f));
      CHECK(q.y == doctest::Approx(0.0f));
      CHECK(q.z == doctest::Approx(0.0f));
    }

    SUBCASE("90-Degree Rotation Around Z") {
      CYLLENE_SDK::Matrix4 m;
      m.identity();
      m.setRotation(Vector3f(0, 0, Math::DEG2RAD * 90.0f)); // Rotate 90° around Z

      Quaternion q = m.getQuatRotation();
      // Expected quaternion for 90° around Z: (0, 0, sin(45°), cos(45°))
      CHECK(q.w == doctest::Approx(std::cos(Math::DEG2RAD * 45.0f)));
      CHECK(q.x == doctest::Approx(0.0f));
      CHECK(q.y == doctest::Approx(0.0f));
      CHECK(q.z == doctest::Approx(std::sin(Math::DEG2RAD * 45.0f)));
    }
  }

  TEST_CASE("Transformation Composition") {
    Quaternion rotation = Quaternion(Euler(0, Math::degToRad(90.0f), 0));
    Vector3f translation(5, 3, 0);
    Vector3f scale(3, 7, 9);

    Matrix4 transform;
    transform.setTransformMatrix(translation,
      rotation,
      scale);

    Vector3f transformPosition = transform.getPosition();
    CHECK(transformPosition.x == doctest::Approx(5.0f));
    CHECK(transformPosition.y == doctest::Approx(3.0f));
    CHECK(transformPosition.z == doctest::Approx(0.0f));

    Vector3f transformForward = transform.getForwardVector();
    CHECK(transformForward.x == doctest::Approx(-1.0f));
    CHECK(transformForward.y == doctest::Approx(0.0f));
    CHECK(transformForward.z == doctest::Approx(0.0f));

    Vector3f transformScale = transform.getScale();
    CHECK(transformScale.x == doctest::Approx(3.0f));
    CHECK(transformScale.y == doctest::Approx(7.0f));
    CHECK(transformScale.z == doctest::Approx(9.0f));
  }

  TEST_CASE("transform position") {
    Matrix4 transform;
    Quaternion rotation = Quaternion(Euler(0, Math::DEG2RAD * 90.0f, 0, EulOrdXYZs));
    Vector3f translation(5, 3, 0);
    Vector3f scale(1, 1, 1);
    transform.setTransformMatrix(translation,
      rotation,
      scale);
    Vector3f point = transform.transformPosition(Vector3f(1, 2, 3));

    // TODO: mising check
  }

  TEST_CASE("View Matrix") {
    Matrix4 view;
    view.view(Vector4f(0, 0, 5, 1),
      Vector4f(0, 0, 0, 1),
      Vector4f(0, 1, 0, 0));

    Vector3f forward = view.getForwardVector();
    CHECK(forward.x == doctest::Approx(0.0f));
    CHECK(forward.y == doctest::Approx(0.0f));
    CHECK(forward.z == doctest::Approx(-1.0f));
  }

  TEST_CASE("Projection Matrices") {
    SUBCASE("Orthographic") {
      Matrix4 ortho;
      ortho.orthogonal(800, 600, 0.1f, 100.0f);

      // Should transform z from [0.1, 100] to [-1, 1] in GAPI_GL
      Vector3f nearPoint = ortho.transformPosition(Vector3f(0, 0, 0.1f));
      Vector3f farPoint = ortho.transformPosition(Vector3f(0, 0, 100.0f));

      CHECK(nearPoint.z == doctest::Approx(-1.0f).epsilon(0.01f));
      CHECK(farPoint.z == doctest::Approx(1.0f).epsilon(0.01f));
    }

    SUBCASE("Perspective") {
      Matrix4 persp;
      persp.perspective(800, 600, 0.1f, 100.0f, 45.0f);

      // Should transform z properly with perspective divide
      Vector4f point = persp.transformPositionV4(Vector4f(0, 0, -5.0f, 1.0f));
      point /= point.w; // Perspective divide
      CHECK(point.z > 0.0f); // Should be in front of camera
    }
  }

  TEST_CASE("Matrix Inversion") {
    Matrix4 m(1, 0, 0, 5,
      0, 1, 0, 3,
      0, 0, 1, 0,
      0, 0, 0, 1);

    Matrix4 inv = m.inversed();
    Matrix4 identity = m * inv;

    // Should be very close to identity
    for (int c = 0; c < 4; c++) {
      for (int r = 0; r < 4; r++) {
        if (c == r) {
          CHECK(identity.m[c][r] == doctest::Approx(1.0f).epsilon(0.0001f));
        }
        else {
          CHECK(identity.m[c][r] == doctest::Approx(0.0f).epsilon(0.0001f));
        }
      }
    }
  }

  TEST_CASE("Matrix transformPosition (Translate only)") {
    Matrix4 m = Matrix4::IDENTITY;
    m.translate(Vector3f(15, 7, -2));

    Vector3f point(-5, 0, 1);
    Vector3f transformed = m.transformPosition(point);

    CHECK(transformed.x == doctest::Approx(10.0f));
    CHECK(transformed.y == doctest::Approx(7.0f));
    CHECK(transformed.z == doctest::Approx(-1.0f));

    Vector3f dir(1, -1, 0);
    Vector3f transformedDir = m.transformDirection(dir);

    CHECK(transformedDir.x == doctest::Approx(1.0f));
    CHECK(transformedDir.y == doctest::Approx(-1.0f));
    CHECK(transformedDir.z == doctest::Approx(0.0f));
  }

  TEST_CASE("Matrix transformPosition (Rotation only)") {
    Matrix4 m = Matrix4::IDENTITY;
    m.rotateY(Math::degToRad(90.0f));

    Vector3f point(33, 2, 0);
    Vector3f transformed = m.transformPosition(point);

    CHECK(transformed.x == doctest::Approx(0.0f));
    CHECK(transformed.y == doctest::Approx(2.0f));
    CHECK(transformed.z == doctest::Approx(33.0f));

    Vector3f dir(-1, -1, -1);
    dir.normalize();

    Vector3f transformedDir = m.transformDirection(dir);

    Vector3f expectedResult(1, -1, -1);
    expectedResult.normalize();

    CHECK(transformedDir.x == doctest::Approx(expectedResult.x));
    CHECK(transformedDir.y == doctest::Approx(expectedResult.y));
    CHECK(transformedDir.z == doctest::Approx(expectedResult.z));
  }

  TEST_CASE("Matrix transformPosition (Scale only)") {
    Matrix4 m = Matrix4::IDENTITY;
    m.scale(Vector3f(100.0f, 0.2f, 1.0f));

    Vector3f point(1.0f, 5.0f, 0.0f);
    Vector3f transformed = m.transformPosition(point);

    CHECK(transformed.x == doctest::Approx(100.0f));
    CHECK(transformed.y == doctest::Approx(1.0f));
    CHECK(transformed.z == doctest::Approx(0.0f));

    Vector3f dir(1.0f, 1.0f, 1.0f);
    dir.normalize();

    Vector3f transformedDir = m.transformDirection(dir);
    transformedDir.normalize();

    Vector3f expectedResult(100.0f, 0.2f, 1.0f);
    expectedResult.normalize();

    CHECK(transformedDir.x == doctest::Approx(expectedResult.x));
    CHECK(transformedDir.y == doctest::Approx(expectedResult.y));
    CHECK(transformedDir.z == doctest::Approx(expectedResult.z));
  }

  TEST_CASE("Matrix transformPosition (Translation + Rotation)") {
    Matrix4 m = Matrix4::IDENTITY;
    m.rotateZ(Math::degToRad(90.0f));
    m.translate(40.0f, -5.0f, 0.1);

    Vector3f point(1.0f, 2.0f, 3.0f);
    Vector3f transformed = m.transformPosition(point);

    CHECK(transformed.x == doctest::Approx(42.0f));
    CHECK(transformed.y == doctest::Approx(-6.0f));
    CHECK(transformed.z == doctest::Approx( 3.1f));

    Vector3f directionZ(0.0f, 0.0f, 1.0f);
    Vector3f transformedDirZ = m.transformDirection(directionZ);
    transformedDirZ.normalize();

    CHECK(transformedDirZ.x == doctest::Approx(0.0f));
    CHECK(transformedDirZ.y == doctest::Approx(0.0f));
    CHECK(transformedDirZ.z == doctest::Approx(1.0f));

    Vector3f direction(-1.0f, 1.0f, 0.0f);
    direction.normalize();
    Vector3f transformedDir = m.transformDirection(direction);
    transformedDir.normalize();
    Vector3f expectedDirection(1.0f, 1.0f, 0.0f);
    expectedDirection.normalize();

    CHECK(transformedDir.x == doctest::Approx(expectedDirection.x));
    CHECK(transformedDir.y == doctest::Approx(expectedDirection.y));
    CHECK(transformedDir.z == doctest::Approx(expectedDirection.z));
  }
}

TEST_SUITE("Primitive Tests") {
  TEST_CASE("OBB Collision Detection") {
    SUBCASE("Colliding OBBs (Overlapping)") {
      OBB a, b;

      // OBB A: Centered at (0,0,0), axis-aligned, size 2x2x2
      a.m_center = Vector3f(0, 0, 0);
      a.m_hExtents = Vector3f(1, 1, 1);
      a.m_orientation = Quaternion(0, 0, 0, 1);  // No rotation

      // OBB B: Centered at (0.5, 0, 0), slightly offset (should intersect)
      b.m_center = Vector3f(0.5f, 0, 0);
      b.m_hExtents = Vector3f(1, 1, 1);
      b.m_orientation = Quaternion(0, 0, 0, 1);  // No rotation

      CHECK(a.intersects(b) == true);  // Should collide
    }

    SUBCASE("Non-Colliding OBBs (Separated)") {
      OBB a, b;

      // OBB A: Centered at (0,0,0), axis-aligned, size 2x2x2
      a.m_center = Vector3f(0, 0, 0);
      a.m_hExtents = Vector3f(1, 1, 1);
      a.m_orientation = Quaternion(0, 0, 0, 1);  // No rotation

      // OBB B: Centered at (3.0, 0, 0), slightly offset (should intersect)
      b.m_center = Vector3f(3.0f, 0, 0);
      b.m_hExtents = Vector3f(1, 1, 1);
      b.m_orientation = Quaternion(0, 0, 0, 1);  // No rotation

      CHECK(a.intersects(b) == false);  // Should NOT collide
    }

    SUBCASE("Colliding Rotated OBBs") {
      OBB a, b;

      // OBB A: Centered at (0,0,0), axis-aligned, size 2x2x2
      a.m_center = Vector3f(0, 0, 0);
      a.m_hExtents = Vector3f(1, 1, 1);
      a.m_orientation = Quaternion(0, 0, 0, 1);  // No rotation

      // OBB B: Centered at (3.0, 0, 0), slightly offset (should intersect)
      b.m_center = Vector3f(1.5f, 0, 0);
      b.m_hExtents = Vector3f(0.5f, 0.5f, 0.5f);
      b.m_orientation = Quaternion(Euler(0, Math::DEG2RAD * 45.0f, 0, EulOrdXYZs));
      // 
      //     // OBB B: Centered at (1.5, 0, 0), rotated 45° around Y (should intersect)
      //     b.center = Vector3f(1.5f, 0, 0);
      //     b.extents = Vector3f(0.5f, 0.5f, 0.5f);
      //     float angle = 3.141592f / 4.0f;  // 45° in radians
      //     b.orientation = Quaternion(std::cos(angle / 2), 0, std::sin(angle / 2), 0);

      CHECK(a.intersects(b) == true);  // Should collide
    }
  }

  TEST_CASE("Capsule-Capsule Collision") {
    SUBCASE("Colliding Capsules (Overlapping)") {
      Capsule cap1 = { Vector3f(0, 0, 0), Vector3f(2, 0, 0), 1.0f };
      Capsule cap2 = { Vector3f(1, 0, 0), Vector3f(1, 2, 0), 1.0f };

      CHECK(cap1.intersects(cap2) == true); // Should collide
    }

    SUBCASE("Non-Colliding Capsules (Separated)") {
      Capsule cap1 = { Vector3f(0, 0, 0), Vector3f(2, 0, 0), 1.0f };
      Capsule cap2 = { Vector3f(3, 0, 0), Vector3f(5, 0, 0), 1.0f };

      CHECK(cap1.intersects(cap2) == false); // Should NOT collide
    }

    SUBCASE("Colliding Capsules (Touching)") {
      Capsule cap1 = { Vector3f(0, 0, 0), Vector3f(2, 0, 0), 1.0f };
      Capsule cap2 = { Vector3f(3, 0, 0), Vector3f(3, 2, 0), 1.0f };

      CHECK(cap1.intersects(cap2) == true); // Should collide (barely touching)
    }
  }

  TEST_CASE("Rect-Rect Collision") {
    Capsule r(Vector3f::ZERO, Vector3f::ZERO, 0);
  }
}
