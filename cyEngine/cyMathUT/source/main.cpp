
// Defining values for unit testing
#include <cyCrashHandler.h>
#include <cyColor.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyTime.h>
#include <cyUnitTesting.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

// Using namespace for ease of use
using namespace CYLLENE_SDK;


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
