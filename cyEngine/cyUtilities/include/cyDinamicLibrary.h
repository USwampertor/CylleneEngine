#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyFileSystem.h"

namespace CYLLENE_SDK {


  namespace DYNLIBRESULT {
    BETTER_ENUM(E, uint32, eLOADED = 0, eALREADYLOADED, eFAIL )
  }

  class DynamicLibrary {
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
    loadSymbol();

    const String&
    getName() const {
      return m_name;
    }

  private:
    String m_name;

    void* m_data = nullptr;

  };
}
