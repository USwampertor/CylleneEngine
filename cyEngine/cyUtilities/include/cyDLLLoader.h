/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyDynamicLibrary.h
 * @author Marco "Swampy" Millan
 * @date 2019/07/28 2019
 * @brief
 *
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyModule.h"

namespace CYLLENE_SDK {
struct DLLLoader {
 public:
 
  /**
   * Default constructor
   */
  DLLLoader() = default;

  /**
   * Default destructor
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
  load(const String& path, const String& functionName = "create", bool isOSDll = true);

};
}

