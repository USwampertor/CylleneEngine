/*********************************************/
/*
 * @file 	cyRandom
 * @author	Marco "Swampertor" Millan
 * @date	3/09/2021
 * @brief	
 *
 */
/******************************************** */
#pragma once

#include <random>
#include <chrono>

#include "cyUtilitiesPrerequisites.h"
#include "cyZiggurat.h"

namespace CYLLENE_SDK {
  struct CY_UTILITY_EXPORT Random
  {
  public:

    static void 
    init() {
      std::mt19937_64 random;

    }

    void 
    next();

    void
    range();


  private:

    static float m_seed;


    static Ziggurat<double> m_ziggurat;
  };

}
