/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyDinamicLibrary.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/01
 * @brief 	An object to load libraries and get symbols from them
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyFileSystem.h"

namespace CYLLENE_SDK {


  namespace DYNLIBRESULT {
    BETTER_ENUM(E, uint32, eSUCCESS = 0, eALREADYLOADED, eFAIL )
  }

  class CY_UTILITY_EXPORT DynamicLibrary {
    DynamicLibrary(const String& libName) {
      m_name = libName;
      m_data = nullptr;
      load();
    }

    ~DynamicLibrary() = default;

    DYNLIBRESULT::E
    load();

    DYNLIBRESULT::E
    unload();

    void*
    loadSymbol(const String& symbol);

    const String&
    getName() const {
      return m_name;
    }

  private:
    String m_name;

    void* m_data = nullptr;

  };
}
