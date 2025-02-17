/**
 * 
 */

#include <conio.h>

#include <cyArgumentParser.h>
#include <cyCrashHandler.h>
#include <cyColor.h>
#include <cyEvent.h>
#include <cyException.h>
#include <cyFileSystem.h>
#include <cyJSON.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyRandom.h>
#include <cySmartPointers.h>
#include <cyTime.h>
#include <cyQuaternion.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

// Defining values for unit testing
#include <cyUnitTesting.h>

// Using namespace for ease of use
using namespace CYLLENE_SDK;


/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[])
{
  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}

TEST_CASE("[module] testing module startup") {
  CrashHandler::startUp();
  CHECK(CrashHandler::isStarted());
  SmartPointers::startUp();
  CHECK(CrashHandler::isStarted());
  Logger::startUp();
  CHECK(Logger::isStarted());
  Time::startUp();
  CHECK(Time::isStarted());
}

TEST_CASE("[benchmark] Testing benchmark system") {
  uint64 x = 1;
  BENCHMARK("test", [&]() {
    DONOTOPTIMIZE(x += 1);
  });
}

TEST_CASE("[math] Modf") {
  CHECK(3 == Math::mod(3, 4));
  CHECK(0 != Math::mod(3, 4));
}

TEST_CASE("[random] testing random module") {
  Random::init();


  float value = 0.0f;
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::get<float>();
  }
  Time::instance().update();

  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getNormalized();
    CHECK(value <= 1.0f);
  }
  Time::instance().update();
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getRanged<float>(876.0f, 1000.0f);
    CHECK((value <= 1000.0f && value >= 876.0f));
  }
  Time::instance().update();
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = static_cast<float>(Random::getRanged<uint32>(0, 10));
    CHECK((value <= 10));
  }
  Time::instance().update();
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

TEST_CASE("[pointers] Testing smart pointers") {
  
}

TEST_CASE("[vector2i] Testing vector2 functionality") {
  Vector2i v(0, 0);
  CHECK(v[0] == 0);
}

TEST_CASE("[vector2f] Testing vector2 functionality") {

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



// Old main
/*
int32
main(int argc, const char* argv[]) {

  try
  {
#if _DEBUG
    printf("DEBUG\n");
#else
    printf("RELEASE\n");
#endif
    if (!initModules()) {
      CY_EXCEPT(UnitTestException, "Modules did not initialize");
    }
    Logger::instance().log("TEST", 
                           LOG_VERBOSITY::E::eDEFAULT, 
                           LOG_CHANNEL::E::eDEFAULT, 
                           LOG_OUTPUT::E::eSPLASH | LOG_OUTPUT::E::eSCREEN);
  }
  catch (Exception e)
  {
    std::cout << e.what();
  }
  std::cout << "End of CyUtilities unit test..." << std::endl;
  return 0;
}
*/

