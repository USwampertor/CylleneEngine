/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	main.cpp
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/21
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#include <cyBeing.h>
#include <cyCoreUTPrerequisites.h>
#include <cyCrashHandler.h>
#include <cyGameMode.h>
#include <cyLogger.h>
#include <cyTime.h>
#include <cyUnitTesting.h>
#include <cyVector2f.h>
#include <cyWindow.h>


using namespace CYLLENE_SDK;

int32
main(int32 argc, char* argv[]) {

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

#define CLASSNAME(x) #x

TEST_CASE("[Being] Creation of beings") {
  CHECK(Being::getClassName() == CLASSNAME(Being));
  CHECK(GameMode::getClassName() == CLASSNAME(GameMode));
}