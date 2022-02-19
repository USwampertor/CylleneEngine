
#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
# include <Psapi.h>

//Disable warning in VS2015 that's not under my control
#pragma warning(disable : 4091)
#	include <DbgHelp.h>
#pragma warning(default : 4091)

#endif

#include "cyUtilitiesPrerequisites.h"
#include "cyDLLLoader.h"
#include "cyFileSystem.h"
#include "cyMath.h"
#include "cyCrashHandler.h"

namespace CYLLENE_SDK {
  String
  CrashHandler::getWindowsStackTrace() {
    return String("");
  }
}