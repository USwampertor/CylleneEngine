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

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();

  Matrix2x2 test1(1, 2, 3, 4);
  Matrix2x2 test2(5, 6, 7, 8);
  Matrix3x3 test3(331, 34, 1, 357, 7, 13, 2, -38, 63);
  Matrix3x3 test4(1.0f/7.0f, 4.0f /21.0f, -5.0f /21.0f, 5.0f /7.0f, -1.0f /21.0f, -4.0f /21.0f, -2.0f, 0.0f, 1.0f);
  Matrix4x4 test5(1.0f, 9.0f, 5.0f, 21.0f, 4.0f, 12.0f, 5.0f, 3.0f, 4.0f, 6.0f, 11.0f, 2.0f, 45.0f, 7.0f, 6.0f, 4.0f);
  Matrix4x4 test6(12.0f, 3.0f, 4.0f, 5.0f, 2.0f, 13.0f, 4.0f, 5.0f, 2.0f, 3.0f, 14.0f, 5.0f, 2.0f, 3.0f, 4.0f, 15.0f);
  
  try
  {

  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    CrashHandler::instance().createReport(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}