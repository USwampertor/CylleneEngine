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
# include "cyWindows.h"
# include <Psapi.h>
# include <winnt.h>
# include <iomanip>

# pragma comment(lib, "psapi.lib")
# pragma comment(lib, "dbghelp.lib")

#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )
#endif

namespace CYLLENE_SDK {
  void
  CrashHandler::init() {
    m_crashFolder = "Crash Reports";
    m_crashLog = "Cyllene_crash";
    m_errorMessage = "A fatal error has occurred comrade!";
  }

  void
  CrashHandler::createReport(StdException exception) {

    // WINDOWS OS SPECIFIC CALL FUNCTIONS
#if CY_PLATFORM == CY_PLATFORM_WIN32

    
    // LINUX OS SPECIFIC CALL FUNCTIONS
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

    createMiniDump();
  }

  void
  CrashHandler::createReport(void* exception) const {

  }

  void
  CrashHandler::createReport(const String& type,
                             const String& description,
                             const String& errorFunction,
                             const String& file, 
                             uint32 line) const {

  }

  String
  CrashHandler::getPlatformStack() {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    auto lastError = GetLastError();
#endif
    return "";
  }

  String
  CrashHandler::getStack() {
    return "";
  }

  void
  CrashHandler::showCallStack() {
  }

  Path
  CrashHandler::getCrashFolder() {
    File errorFolder = FileSystem::open(FileSystem::getWorkingDirectory().fullPath() + "/" + m_crashFolder);
    FileSystem::createFolder(errorFolder.path());
    Path p(errorFolder.path());
    return p;
  }

  void
  CrashHandler::createMiniDump() {
    File dump = FileSystem::open("./dump.txt");
    String output = "Cyllene Engine has failed";
    dump.writeFile(output);
  }

  void
  CrashHandler::openCrashHandlerApp() {
    system(String(FileSystem::getWorkingDirectory().fullPath() + String("/CrashHandler.exe")).c_str());
  }

  void
  CrashHandler::shutdown() {

  }

}