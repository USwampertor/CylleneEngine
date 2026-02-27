/**
 * @file cyArgumentParser.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for ArgumentParser.
 */

#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyRegex.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {
/**
 * @class ArgumentParser
 * @brief Parses command-line style flags and their values.
 */
class CY_UTILITY_EXPORT ArgumentParser
{
 public:

  /**
   * @brief Default constructor.
   */
  ArgumentParser() = default;

  /**
   * @brief Default destructor.
   */
  ~ArgumentParser() = default;

  /*
   *	@brief		This function parses the information passed as a string.
   *            Internally, for each parameter that finds, it will fill the
   *            parameter map with the information passed on the params.
   *            This override is to simulate the main entry parameters
   *	@param		const int& amount of parameters
   *	@param		char* argv[] the string of arguments
   *  @return   nothing
   */
  void
  parse(const int& argc, const char* argv[]);

  
  /*
   *	@brief		This function parses the information passed as a string.
   *            Internally, for each parameter that finds, it will fill the
   *            parameter map with the information passed on the params
   *	@param		const String& parameters the string with the execution params
   *  @return   nothing
   */
  void
  parse(const String& parameters);

  /*
   *	@brief	  Adds a flag to the map of flags to check while parsing
   *	@param	  const String& newFlag the new flag to add
   *	@return   true if the insertion was successful, false if there was already 
   *            that flag in the map
   */
  bool
  addFlag(const String& newFlag);

  /*
   *	@brief	  Adds an value to the map of flags
   *	@param	  const String& newParameter the new value to add
   *	@return   nothing
   */
  void
  setFlagValue(const String& flag, const String& newParameter);

  /*
   *	@brief		returns the parameter binded to the argument keyWord
   *	@param	  const String& parameter the keyWord to find 
   *	@return   the information binded to the keyword, if none, it will return
   *            a blank string
   */
  const Vector<String>
  getFlagValues(const String& flag);

  /*
   *	@brief		Checks if there is a defined flag in the flag map
   *	@param	  const String& flag the keyWord to find
   *	@return   the information binded to the keyword, if none, it will return
   *            a blank string
   */
  bool
  hasFlag(const String& flag);

  /*
   *	@brief		Removes a flag from the map
   *	@param	  const String& flag the keyWord to find
   *	@return   true if the flag was existing already
   */
  bool
  removeFlag(const String& flag);

  /**
   * @brief Map of parsed flags and associated values.
   */
  Map<String, Vector<String>> m_flagMap;

};

}

