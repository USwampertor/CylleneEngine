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
    File f = FileSystem::open("../resources/NICE COCK.png");
    if (f.exists())
    {
      std::cout << f.fileName();
    }

  }
  catch (const std::exception& e)
  {
    CrashHandler::instance().createReport();
  }

}