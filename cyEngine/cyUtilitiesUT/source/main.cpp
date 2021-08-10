/**
 * 
 */
#include <cyCrashHandler.h>
#include <cyLogger.h>
#include <cyUtilities.h>
#include <cyMath.h>
#include <cyFileSystem.h>
using namespace CYLLENE_SDK;

int
main() {
  Logger::startUp();
  CrashHandler::startUp();

  try
  {
    Path p("../");

    // Start engine
    std::cout << p.fullPath();

  }
  catch (const std::exception& e)
  {
    CrashHandler::instance().createReport();
  }

}