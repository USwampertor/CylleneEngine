#include "cyDLLLoader.h"

#include "cyFileSystem.h"
#include "cyLogger.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX
# include <dlfcn.h>
#endif

namespace CYLLENE_SDK {

void*
DLLLoader::load(Stringview path, Stringview functionName, bool isOSDll) {

#if CY_PLATFORM == CY_PLATFORM_WIN32

  String libPath = isOSDll ? "" : FileSystem::getWorkingDirectory().fullPath() + "/";
  libPath += path;
  Path p(libPath);
  HINSTANCE myDll = LoadLibrary(p.fullPath().c_str());
  if (!myDll) {
    uint64 err = GetLastError();
    String errorString = Utils::format("Could not find dll at given path: %s \nError code: %d", path.data(), err);
    std::cout << errorString << std::endl;
    Logger::instance().logError(errorString, LOG_CHANNEL::E::eSYSTEM);
    return nullptr;
  }
  std::cout << "Loading " << path << "..." << std::endl;

  Logger::instance().logDebug(Utils::format("Loading %s...",path.data()), LOG_CHANNEL::E::eSYSTEM);

  auto t_api = GetProcAddress(myDll, functionName.data());
  if (!t_api) {

    CY_ASSERT(t_api == nullptr && "NO PLUGIN WITH GIVEN NAME");

    FreeLibrary(myDll);
    return nullptr;
  }
#elif CY_PLATFORM == CY_PLATFORM_LINUX

  String libPath = isOSDll ? "" : FileSystem::getWorkingDirectory().fullPath() + "/";
  libPath += path;
  void* myLib = dlopen(libPath.c_str(), RTLD_LAZY);
  if (!myLib) {
    String errorString = Utils::format("Could not find library at given path: %s \nError: %s", path.data(), dlerror());
    std::cout << errorString << std::endl;
    Logger::instance().logError(errorString, LOG_CHANNEL::E::eSYSTEM);
    return nullptr;
  }
  std::cout << "Loading " << path << "..." << std::endl;

  Logger::instance().logDebug(Utils::format("Loading %s...",path.data()), LOG_CHANNEL::E::eSYSTEM);

  auto t_api = dlsym(myLib, functionName.data());
  if (!t_api) {

    CY_ASSERT(t_api == nullptr && "NO PLUGIN WITH GIVEN NAME");

    dlclose(myLib);
    return nullptr;
  }
#endif
  return static_cast<void*>(t_api);
}

}