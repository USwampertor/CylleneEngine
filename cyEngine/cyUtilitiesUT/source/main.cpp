/**
 * 
 */
#include <cyCrashHandler.h>
#include <cyFileSystem.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMath.h>
#include <cyUtilities.h>

using namespace CYLLENE_SDK;

int
main() {
  Logger::startUp();
  CrashHandler::startUp();

  try
  {
    std::cout << PlatformMath::abs(2);
    // File f = FileSystem::open("../resources/NICE COCK.png");
    // if (f.exists())
    // {
    //   std::cout << f.fileName();
    // }

    std::cout << Math::abs(-1);

  }
  catch (const std::exception& e)
  {
    CrashHandler::instance().createReport();
  }

}