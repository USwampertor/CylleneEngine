/**
 * 
 */
#include <cyFileSystem.h>
#include <cyVector2f.h>

#include <conio.h>

using namespace CYLLENE_SDK;

int
main() {

    Vector2f a(0, 0);
    a.normalized();
  try
  {
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
  }
  _getch();
  return 0;
}