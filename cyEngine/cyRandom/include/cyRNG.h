/**
 * @file cyRNG.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for RNG.
 */
#pragma once

#include "cyRandomPrerequisites.h"

namespace CYLLENE_SDK {

/**
 *	@class RNG	
 *	@brief A RNG base class. This only should be used to create other RNG child classes
 *
 */
class RNG
{
public:

  /**
   *	@brief	generates a new random number and returns it. Each child class
   *          defines how the number is generated
   *  @return	a new random number
   */
  virtual float 
  next() = 0;
};
}


