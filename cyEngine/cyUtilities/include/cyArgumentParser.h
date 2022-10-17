/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file    cyArgumentParser.h
 * @author 	Marco "Swampy" Millan
 * @date 	  2022/02/28
 * @brief 	An argument parser system so you can pass parameters at execution 
 *          and be able to check what to do with each one
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyUtilities.h"

namespace CYLLENE_SDK {
  /*
   *	@class  ArgumentParser
   *	@brief  An object that you can use to map parameters and actions and parse 
   *          execution parameters.
   */
class CY_UTILITY_EXPORT ArgumentParser
{
 public:
  /**
   * Default constructor
   */
  ArgumentParser() = default;

  /**
   * Default destructor
   */
  ~ArgumentParser() = default;
  
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
   *	@brief	  Adds an argument to the map of arguments to check while parsing
   *	@param	  const String& newArgument the new argument to add
   *	@return   true if the insertion was successful, false if there was already 
   *            that argument in the map
   */
  bool
  addArgument(const String& newArgument);


  /*
   *	@brief		returns the parameter binded to the argument keyWord
   *	@param	  const String& parameter the keyWord to find 
   *	@return   the information binded to the keyword, if none, it will return
   *            a blank string
   */
  String
  getParameter(const String& parameter);

  /**
   * @brief the map that holds the keywords with the arguments passed
   */
  Map<String, String> m_argumentMap;

};
}
