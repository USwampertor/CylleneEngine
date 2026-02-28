
/**
 * @file cyThread.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains Random Number generator based on the MT system.
 */
#pragma once

#include "cyRandomPrerequisites.h"
#include "cyRNG.h"

namespace CYLLENE_SDK {

using MersenneTwister = std::mt19937;
using MersenneTwister64 = std::mt19937_64;

/**
 * @class MT
 * @brief a Mersenne Twister algorith based Random Number generator
 */
class MT : public RNG
{
public:

  virtual float
  next() override {
    uint32 seed = m_generator();
    return static_cast<float>(seed);
  }

  MersenneTwister&
  getGenerator() {
    return m_generator;
  }

public:

  /**
   * @brief a Mersenne Twister number generator
   */
  MersenneTwister m_generator;

};

}

