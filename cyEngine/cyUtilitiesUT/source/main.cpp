/**
 * 
 */

#include <conio.h>

#include <cyJSON.h>
#include <cyQuaternion.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyCrashHandler.h>
#include <cyMath.h>
#include <cyFileSystem.h>
#include <cyRandom.h>
#include <cyException.h>
#include <cyLogger.h>
#include <cyTime.h>

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();
  CrashHandler::instance().init();
  Random::init();
  Random::init();

  Logger::startUp();
  Logger::instance().init();

  Time::startUp();
  Time::instance().init();

  try
  {
#if _DEBUG
    printf("DEBUG\n");
#else
    printf("RELEASE\n");
#endif
    if (FileSystem::createFolder(FileSystem::getWorkingDirectory().fullPath() + "/Shaders")) {
      FileSystem::createFile(FileSystem::getWorkingDirectory().fullPath() + "/Shaders/MyShader.hlsl");
    }
    File shader = FileSystem::open(FileSystem::getWorkingDirectory().fullPath() + "/Shaders/MyShader.hlsl");
    if (shader.exists()) {
      if (shader.isFile()) {
        Path p(shader.path());
        std::cout << p.extension() << std::endl;
      }
    }

    std::cout << FileSystem::exists(FileSystem::getWorkingDirectory().fullPath() + "/Shaders/DoesntExist.hlsl") << std::endl;

    CY_EXCEPT(UnitTestException, "Test");
    // library_throw("Test Exception");
  }
  catch (const Exception& e)
  {
    std::cout << e.what();
    // library_handle_exception(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}