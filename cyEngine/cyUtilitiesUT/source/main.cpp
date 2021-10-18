/**
 * 
 */

#include <conio.h>

#include <cyCrashHandler.h>
#include <cyMatrix2x2.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyUtilities.h>
#include <cyVector2f.h>
#include <cyVector2i.h>
#include <cyQuaternion.h>
#include <cyFileSystem.h>

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();

  
  try
  {
    Path root = FileSystem::getBasePath();
    std::cout << root.driveLetter() << " this is the drive" << std::endl;
    std::cout << (root.pointsToContent() ? "It does" : "It does not" ) << " point to content" << std::endl;
    std::cout << root.path() << " this is the path" << std::endl;
    std::cout << root.fullPath() << " this is the full path" << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    CrashHandler::instance().createReport(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}