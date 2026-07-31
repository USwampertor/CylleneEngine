#include "cyDinamicLibrary.h"
#include "cyLogger.h"
#include "cyUtilities.h"
#include "cyFileSystem.h"
#include "cyAssert.h"

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX
# include <dlfcn.h>
#endif

namespace CYLLENE_SDK {
  DYNLIBRESULT::E
  DynamicLibrary::load() {

#if CY_PLATFORM == CY_PLATFORM_WIN32
    auto newData = static_cast<void*>(LoadLibrary(m_name.c_str()));
#else
    auto newData = dlopen(m_name.c_str(), RTLD_LAZY);
#endif
    if (!newData) {
#if CY_PLATFORM == CY_PLATFORM_WIN32
      uint64 err = GetLastError();
      String errorString = "Could not find dll at given path: " + m_name + "\nError code: " + Utils::toString(err);
#else
      String errorString = "Could not load library at given path: " + m_name + "\nError: " + dlerror();
#endif
      std::cout << errorString << std::endl;
      Logger::instance().logError(errorString, LOG_CHANNEL::E::eSYSTEM);
      return DYNLIBRESULT::E::eFAIL;
    }
    if (m_data == newData) {
      Logger::instance().logError("This library was already loaded", LOG_CHANNEL::E::eSYSTEM);
#if CY_PLATFORM == CY_PLATFORM_WIN32
      FreeLibrary(static_cast<HINSTANCE>(newData));
#else
      dlclose(newData);
#endif
      return DYNLIBRESULT::E::eLOADED;
    }
    m_data = newData;
#if CY_PLATFORM == CY_PLATFORM_WIN32
    FreeLibrary(static_cast<HINSTANCE>(newData));
#else
    dlclose(newData);
#endif
    return DYNLIBRESULT::E::eSUCCESS;

  }

  DYNLIBRESULT::E
  DynamicLibrary::unload() {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    FreeLibrary(static_cast<HINSTANCE>(m_data));
#else
    dlclose(m_data);
#endif
    return DYNLIBRESULT::E::eSUCCESS;
  }

  void*
  DynamicLibrary::loadSymbol(Stringview symbol) {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    auto t_api = GetProcAddress(static_cast<HINSTANCE>(m_data), symbol.data());
    if (!t_api) {

      CY_ASSERT(t_api == nullptr && 
                Utils::format("THE LIBRARY DOES NOT CONTAIN THE SYMBOL %s", symbol.data()).c_str());

      return nullptr;
    }
#else
    auto t_api = dlsym(m_data, symbol.data());
    if (!t_api) {

      CY_ASSERT(t_api == nullptr && 
                Utils::format("THE LIBRARY DOES NOT CONTAIN THE SYMBOL %s", symbol.data()).c_str());

      return nullptr;
    }
#endif
    return t_api;
  }

}