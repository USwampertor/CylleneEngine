/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	main.cpp
 * @author 	Marco "Swampy" Millan
 * @date 	2022/09/06
 * @brief main file
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#include "cyOmniverseUTPrerequisites.h"

#include <cyCrashHandler.h>
#include <cyException.h>
#include <cyLogger.h>
#include <cyOmniverse.h>
#include <cyTime.h>

using namespace CYLLENE_SDK;

int 
main(int argc, char* argv[]) {

  CrashHandler::startUp();
  CrashHandler::instance().init();
  Logger::startUp();
  Logger::instance().init();
  Time::startUp();
  Time::instance().init();

  Omniverse::startUp();
  try
  {
    String url = "omniverse://localhost/Projects/CylleneEngine/Test.usd";
    std::cout << Utils::format("%s", url.c_str()) << std::endl;
    if (!Omniverse::instance().init(true, OMNILOGLEVEL::E::eVERBOSE)) {
      CY_EXCEPT(UnitTestException, "Omniverse was not able to startup");
    }
    Omniverse::instance().createUSD(url);
  }
  catch (const Exception& e)
  {
    std::cout << e.what();
  }
  std::cout << "End of test..." << std::endl;
  Omniverse::shutDown();
  return 0;
}