#pragma once

#include "cyRandomPrerequisites.h"
#include "cyRNG.h"

namespace CYLLENE_SDK {

class MT : public RNG
{
public:
  virtual float
    next() override {
    uint32 seed = m_generator();
    return static_cast<float>(seed);
  }
  std::mt19937 m_generator;

};

}

