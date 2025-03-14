/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyRNG.h
 * @author 	Marco "Swampy" Millan
 * @date 	  2024/11/12
 * @brief 	A RNG base class so we can have different types of 
 *          random number generators inside the Random class
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyRandomPrerequisites.h"

namespace CYLLENE_SDK {

/*
 *	@class RNG	
 *	@brief A RNG base class. This only should be used to create other RNG child classes
 *
 */
class RNG
{
public:

  /*
   *	@brief	generates a new random number and returns it. Each child class
   *          defines how the number is generated
   *  @return	a new random number
   */
  virtual float 
  next() = 0;
};
}


