/**
 * @file cyDLLLoader.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for DLLLoader.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

namespace CYLLENE_SDK {
/**
 * @struct DLLLoader
 * @brief Helper for loading DLL symbols at runtime.
 */
struct CY_UTILITY_EXPORT DLLLoader {
 public:
 
  /**
   * @brief Default constructor.
   */
  DLLLoader() = default;

  /**
   * @brief Default destructor.
   */
  ~DLLLoader() = default;

  /*
   *	@brief	gets a Symbol from a DLL given and the name of the function	
   *	@param  const String& path the path of the DLL
   *	@param  const String& functionName the name of the symbol
   *	@param  bool isOSDll is the Dll you want to load part of the OS or a custom
   *          one?
   *	@return void* with the information of the symbol, nullptr if there was
   *          any error retrieving the information
   */
  static void*
  load(const String& path, const String& functionName = String("create"), bool isOSDll = true);

};
}

