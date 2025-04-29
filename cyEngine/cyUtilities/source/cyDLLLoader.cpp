#include "cyDLLLoader.h"

#include "cyFileSystem.h"
#include "cyLogger.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

namespace CYLLENE_SDK {

  // typedef void* (*PLUGIN_FUNCTION)();
  void*
  DLLLoader::load(const String& path, const String& functionName, bool isOSDll) {
    // PLUGIN_FUNCTION t_api;

#if CY_PLATFORM == CY_PLATFORM_WIN32 

    String libPath = isOSDll ? "" : FileSystem::getWorkingDirectory().fullPath() + "/";
    libPath += path;
    Path p(libPath);
    HINSTANCE myDll = LoadLibrary(p.fullPath().c_str());
    if (!myDll) {
      uint64 err = GetLastError();
      String errorString = "Could not find dll at given path: " + path + "\nError code: " + Utils::toString(err);
      std::cout << errorString << std::endl;
      Logger::instance().logError(errorString, LOG_CHANNEL::E::eSYSTEM);
      return nullptr;
    }
    std::cout << "Loading " << path << "..." << std::endl;
    Logger::instance().logDebug("Loading" + path + "...", LOG_CHANNEL::E::eSYSTEM);

    auto t_api = /*(PLUGIN_FUNCTION)*/GetProcAddress(myDll, functionName.c_str());
    if (!t_api) {

      CY_ASSERT(t_api == nullptr && "NO PLUGIN WITH GIVEN NAME");

      FreeLibrary(myDll);
      return nullptr; 
    }
#elif

#endif
    return static_cast<void*>(t_api);
  }
}