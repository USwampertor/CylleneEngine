/**
 * 
 */
#include <cyFileSystem.h>
using namespace CYLLENE_SDK;

int
main() {

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
  }

}