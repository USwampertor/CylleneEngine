/**
 * @file cyDinamicLibrary.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for DinamicLibrary.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {

  namespace DYNLIBRESULT {
    BETTER_ENUM(E, uint32, 
                eSUCCESS = 0,
                eLOADING,
                eLOADED, 
                eFAIL );
  }

/**
 * @class DynamicLibrary
 * @brief Runtime wrapper for loading libraries and resolving symbols.
 */
class CY_UTILITY_EXPORT DynamicLibrary {
public:
  /**
   * @brief Constructs and loads a dynamic library by name.
   * @param libName Library name or path.
   */
  DynamicLibrary(Stringview libName) {
    m_name = libName;
    m_data = nullptr;
    load();
  }

  /**
   * @brief Default destructor.
   */
  ~DynamicLibrary() = default;

  /**
   * @brief Loads the library.
   * @return Load result.
   */
  DYNLIBRESULT::E
  load();

  /**
   * @brief Unloads the library.
   * @return Unload result.
   */
  DYNLIBRESULT::E
  unload();

  /**
   * @brief Resolves a symbol from the loaded library.
   * @param symbol Symbol name.
   * @return Pointer to resolved symbol, or null on failure.
   */
  void*
  loadSymbol(Stringview symbol);

  /**
   * @brief Returns the library name.
   * @return Library name.
   */
  Stringview
  getName() const {
    return m_name;
  }

private:
  /**
   * @brief Library identifier or path.
   */
  String m_name;

  /**
   * @brief Opaque platform library handle.
   */
  void* m_data = nullptr;

};
}

