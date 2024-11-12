/**
 * 
 */

#include <conio.h>

#include <cyArgumentParser.h>
#include <cyCrashHandler.h>
#include <cyEvent.h>
#include <cyException.h>
#include <cyFileSystem.h>
#include <cyJSON.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyRandom.h>
#include <cyQuaternion.h>
#include <cySmartPointers.h>
#include <cyTime.h>

#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

using namespace CYLLENE_SDK;



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
  // Random::init();

  CrashHandler::startUp();
  CHECK(CrashHandler::isStarted());
  SmartPointers::startUp();
  CHECK(CrashHandler::isStarted());
  Logger::startUp();
  CHECK(Logger::isStarted());
  Time::startUp();
  CHECK(Time::isStarted());

}

TEST_CASE("[random] testing random module") {
  Random::init();
  float value = 0.0f;
  for (int i = 0; i < 1000000; ++i) {
    value = Random::get<float>();
  }
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getNormalized();
    CHECK(value <= 1.0f);
  }
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getRanged<float>(876.0f, 1000.0f);
    CHECK((value <= 1000.0f && value >= 876.0f));
  }
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getRanged<uint32>(0, 10);
    CHECK((value <= 10));
  }
}

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

