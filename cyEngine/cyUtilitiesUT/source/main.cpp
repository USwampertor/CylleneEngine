/**
 * 
 */

#include <conio.h>

#include <cyUtilities.h>
#include <cyMatrix2x2.h>
#include <cyMatrix3x3.h>

using namespace CYLLENE_SDK;

int
main() {
  Matrix2x2 test1(1, 2, 3, 4);
  Matrix2x2 test2(5, 6, 7, 8);
  Matrix3x3 test3(331, 34, 1, 357, 7, 13, 2, -38, 63);
  Matrix3x3 test4(1.0f/7.0f, 4.0f /21.0f, -5.0f /21.0f, 5.0f /7.0f, -1.0f /21.0f, -4.0f /21.0f, -2.0f, 0.0f, 1.0f);
  try
  {
    Matrix3x3 res = test3.inversed();
    // std::cout << test1.toString();
    // std::cout << test2.toString();
    // std::cout << test3.toString();
    // 
    // std::cout << Utils::toString(test1.determinant()) << std::endl;
    // std::cout << Utils::toString(test2.determinant()) << std::endl;
    // std::cout << (test1 + test2).toString();
    // std::cout << (test1 - test2).toString();
    // std::cout << (test1 * test2).toString();
    std::cout << test3.inversed().toString() << std::endl;
    std::cout << (test3.inversed() * test3).toString()  << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}