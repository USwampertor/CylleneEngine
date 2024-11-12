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

// Defining values for unit testing
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

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
  MESSAGE("Starting up needed modules for engine");
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
  MESSAGE("Check how much time does it take to create 1000000 numbers for each type");
  Random::init();


  float value = 0.0f;
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::get<float>();
  }
  Time::instance().update();
  MESSAGE(Utils::format("%2.10f", Time::instance().deltaTime()));

  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getNormalized();
    CHECK(value <= 1.0f);
  }
  Time::instance().update();
  MESSAGE(Utils::format("%2.10f", Time::instance().deltaTime()));
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = Random::getRanged<float>(876.0f, 1000.0f);
    CHECK((value <= 1000.0f && value >= 876.0f));
  }
  Time::instance().update();
  MESSAGE(Utils::format("%2.10f", Time::instance().deltaTime()));
  
  Time::instance().update();
  for (int i = 0; i < 1000000; ++i) {
    value = static_cast<float>(Random::getRanged<uint32>(0, 10));
    CHECK((value <= 10));
  }
  Time::instance().update();
  MESSAGE(Utils::format("%2.10f", Time::instance().deltaTime()));
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

