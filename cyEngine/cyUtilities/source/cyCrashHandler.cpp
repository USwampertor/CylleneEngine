/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyCrashHandler.h
 * @author Marco "Swampy" Millan
 * @date 8/6/2021
 * @brief
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#include "cyCrashHandler.h"

#include "cyFileSystem.h"
#include "cyUtilities.h"

#include <stdexcept>


#if CY_PLATFORM == CY_PLATFORM_WIN32
#include <windows.h>
#endif

namespace CYLLENE_SDK {
  void
  CrashHandler::init() {

  }

  void
  CrashHandler::createReport(Exception exception) {

    // WINDOWS OS SPECIFIC CALL FUNCTIONS
#if CY_PLATFORM == CY_PLATFORM_WIN32

    
    // LINUX OS SPECIFIC CALL FUNCTIONS
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

    createMiniDump();
  }

  void
  CrashHandler::createMiniDump() {
    File dump = FileSystem::open("./dump.txt");
    String output = "Cyllene Engine has failed";
    dump.writeFile(output);
    std::cout << "Finished Writing";
  }

  void
  CrashHandler::openCrashHandler() {

  }

  void
  CrashHandler::shutdown() {

  }

}