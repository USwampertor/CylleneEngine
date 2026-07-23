#include "cyCrashHandler.h"

#if CY_PLATFORM == CY_PLATFORM_LINUX

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>

#include "cyFileSystem.h"
#include "cyLogger.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {

struct CrashHandler::Data
{
  Mutex mutex;
};

CrashHandler::CrashHandler() {
  m_data = cy_new<Data>();
}

CrashHandler::~CrashHandler() {
  cy_delete(m_data);
}

String
CrashHandler::getStackTrace() {
  void* buffer[CY_MAX_STACKTRACE_DEPTH];
  int32 numFrames = backtrace(buffer, CY_MAX_STACKTRACE_DEPTH);
  char** symbols = backtrace_symbols(buffer, numFrames);

  StringStream result;
  for (int32 i = 0; i < numFrames; ++i) {
    result << symbols[i] << std::endl;
  }

  ::free(symbols);
  return result.str();
}

int32
CrashHandler::createReport(void* exception) const {
  (void)exception;

  logErrorAndStackTrace("Unhandled exception (Linux)", getStackTrace());
  Logger::instance().dump();

  Path folderPath = CrashHandler::instance().getCrashFolder();
  Path p = CrashHandler::instance().createDump("Unhandled exception (Linux)", getStackTrace());

  return 0;
}

}

#endif
