#include "cyDinamicLibrary.h"
#include "cyLogger.h"
#include "cyUtilities.h"
#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

namespace CYLLENE_SDK {
  DYNLIBRESULT::E
  DynamicLibrary::load() {

#if CY_PLATFORM == CY_PLATFORM_WIN32
    auto newData = static_cast<void*>(LoadLibrary(m_name.c_str()));
#else
    auto newData;
#endif
    if (!newData) {
      uint64 err = GetLastError();
      String errorString = "Could not find dll at given path: " + m_name + "\nError code: " + Utils::toString(err);
      std::cout << errorString << std::endl;
      Logger::instance().logError(errorString, LOG_CHANNEL::E::eSYSTEM);
      return DYNLIBRESULT::E::eFAIL;
    }
    if (m_data == newData) {
      Logger::instance().logError("This library was already loaded", LOG_CHANNEL::E::eSYSTEM);
      FreeLibrary(static_cast<HINSTANCE>(newData));
      return DYNLIBRESULT::E::eALREADYLOADED;
    }
    m_data = newData;
    FreeLibrary(static_cast<HINSTANCE>(newData));
    return DYNLIBRESULT::E::eSUCCESS;

  }

  DYNLIBRESULT::E
  DynamicLibrary::unload() {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    FreeLibrary(static_cast<HINSTANCE>(m_data));
#endif
    return DYNLIBRESULT::E::eSUCCESS;
  }

  void*
  DynamicLibrary::loadSymbol(const String& symbol) {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    auto t_api = /*(PLUGIN_FUNCTION)*/GetProcAddress(static_cast<HINSTANCE>(m_data), symbol.c_str());
    if (!t_api) {

      CY_ASSERT(t_api == nullptr && 
                Utils::format("THE LIBRARY DOES NOT CONTAIN THE SYMBOL %s", symbol).c_str());

      return nullptr;
    }
#else

#endif
    return t_api;
  }

}