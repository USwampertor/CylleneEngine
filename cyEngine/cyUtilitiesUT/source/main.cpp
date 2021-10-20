/**
 * 
 */

#include <conio.h>

#include <cyJSON.h>
#include <cyQuaternion.h>

#include <cyCrashHandler.h>
#include <cyMath.h>

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();
  Quaternion q;

  try
  {
    q.fromEuler(Euler(Math::PI * 1.5f, Math::PI, 0), 0);
    std::cout << q.toString() << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    CrashHandler::instance().createReport(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}