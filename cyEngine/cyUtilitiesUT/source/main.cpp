/**
 * 
 */
#include <cyCrashHandler.h>
#include <cyLogger.h>
#include <cyUtilities.h>

using namespace CYLLENE_SDK;

int
main() {
  Logger::startUp();
  CrashHandler::startUp();


  try
  {
    // Start engine

  }
  catch (const std::exception& e)
  {
    Logger::instance().logError(Utilities::format(" We found an error at: %s ", e.what()));
    CrashHandler::instance().createReport();
  }

}