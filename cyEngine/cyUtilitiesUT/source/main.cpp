/**
 * 
 */

#include <conio.h>

#include <cyArgumentParser.h>
#include <cyCrashHandler.h>
#include <cyEvent.h>
#include <cyException.h>
#include <cyFileSystem.h>
#include <cyJSON.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyRandom.h>
#include <cyQuaternion.h>
#include <cyTime.h>

using namespace CYLLENE_SDK;

int32
main(int argc, const char* argv[]) {
  CrashHandler::startUp();
  CrashHandler::instance().init();
  Random::init();
  Random::init();

  Logger::startUp();
  Logger::instance().init();

  Time::startUp();
  Time::instance().init();

  Logger::instance().log("test", 
    LOG_VERBOSITY::E::eDEFAULT, 
    LOG_CHANNEL::E::eDEFAULT, 
    LOG_OUTPUT::E::eSPLASH | LOG_OUTPUT::E::eSCREEN);

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

    ArgumentParser parser;

    parser.parse(argc, argv);

    for (auto& flag : parser.m_flagMap)
    {
      for (auto& value : flag.second)
      {
        std::cout << value << std::endl;
      }
    }

    std::cout << FileSystem::exists(FileSystem::getWorkingDirectory().fullPath() + "/Shaders/DoesntExist.hlsl") << std::endl;
    CY_EXCEPT(UnitTestException, "This is part of the test");
  }
  catch (const Exception& e)
  {
    std::cout << e.what();
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}