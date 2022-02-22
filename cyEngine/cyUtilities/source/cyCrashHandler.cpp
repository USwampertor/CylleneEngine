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
#include "cyLogger.h"
#include "cyTime.h"

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
  const String CrashHandler::m_crashFolder  = "Crash Reports";
  const String CrashHandler::m_crashLog     = "CylleneEngine_crashlog";
  const String CrashHandler::m_errorMessage = "A fatal error has occurred comrade!";
  void
  CrashHandler::init() {
  }

  Path
  CrashHandler::getCrashFolder() {
    File errorFolder = FileSystem::open(FileSystem::getWorkingDirectory().fullPath() +
                                        "/" + 
                                        m_crashFolder);
    FileSystem::createFolder(errorFolder.path());
    Path p(errorFolder.path());
    return p;
  }

  void
  CrashHandler::logErrorAndStackTrace(const String& message, 
                                      const String& stackTrace) const {
    StringStream msg;
    msg << m_errorMessage << std::endl << std::endl;
    msg << message << std::endl;
    msg << "Stack Trace" << std::endl;
    msg << stackTrace;

    Logger::instance().logError(msg.str(), LOG_CHANNEL::E::eSYSTEM);
  }

  void
  CrashHandler::logErrorAndStackTrace(const String& type, 
                                      const String& strDescription, 
                                      const String& strFunction, 
                                      const String& strFile, 
                                      uint32 nLine) const {
    StringStream errorMessage;
    errorMessage << "  - Error: " << type << std::endl;
    errorMessage << "  - Description: " << strDescription << std::endl;
    errorMessage << "  - In function: " << strFunction << std::endl;
    errorMessage << "  - In file: " << strFile << ":" << nLine;
    logErrorAndStackTrace(errorMessage.str(), getStackTrace());
  }

  Path
  CrashHandler::createDump(const String& message,
                           const String& stackTrace) {
    StringStream msg;
    msg << m_errorMessage << std::endl << std::endl;
    msg << message << std::endl;
    msg << "Stack Trace" << std::endl;
    msg << stackTrace;
    String dumpPath = FileSystem::getWorkingDirectory().fullPath() +
                      "/Crash Reports/CylleneDump" + 
                      Time::instance().now().toString("%Y%m%d_%H%M%S") +
                      ".ccr";
    File dump = FileSystem::createFile(dumpPath);
    if (!dump.writeFile(msg.str())) {
      Logger::instance().logError("Error writing dump file");
    }
    return Path(dump.path());
  }

  void
  CrashHandler::openCrashHandlerApp(const String& params) {
    Utils::open(String(FileSystem::getWorkingDirectory().fullPath() + 
#if _DEBUG
      String("/cyCrashHandler.exe")),
#else
      String("/cyCrashHandlerd.exe")),
#endif
                params);
  }

  void
  CrashHandler::shutdown() {

  }

}
