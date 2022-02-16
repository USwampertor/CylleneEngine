/*********************************************/
/*
 * @file 	cyRandom
 * @author	Marco "Swampertor" Millan
 * @date	3/09/2021
 * @brief	a Random number generator that uses several systems:
 *        as base, it uses xOrShift for fast pseudorandom numbers
 *        it can also use ziggurath to generate numbers 
 *        ALSO it can create CPRNG if you want to do
 *        something cryptographically safe.
 *        The init generates a pseudorandom number using 
 *        the mersenne twist approach
 *
 */
/******************************************** */
#pragma once

#include <random>
#include <chrono>

#include "cyUtilitiesPrerequisites.h"
#include "cyZiggurat.h"
#include "cyVector2f.h"
#include "cyVector2i.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {
  struct CY_UTILITY_EXPORT Random
  {
  public:

    static void 
    init() {
      std::mt19937 random(0x1976F3A4);
      set(random());
    }

    static uint32
    get();

    static float
    getRangeFloat(float min, float max);

    static int32
    getRangeInt32(int32 min, int32 max);

    static uint32
    getRangeUint32(uint32 max);

    static void
    set(uint32 newSeed);

    static float
    getNormalized();

    static Vector2i
    getNormVector2i();

    static Vector2f
    getNormVector2f();

    static Vector3f
    getNormVector3f();

    static Vector2i
    getVector2i(float thickness);

    static Vector2f
    getVector2f(float thickness);

    static Vector3f
    getVector3f(float thickness);

  private:

    static uint32 m_seed[4];


    static Ziggurat<double> m_ziggurat;
  };

}
