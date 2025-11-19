#pragma once

#include "cyCorePrerequisites.h"
#include "cyRResource.h"

#include <cyTime.h>

namespace CYLLENE_SDK {
class CY_CORE_EXPORT RAudio : public RResource {

public:

  RAudio() : RResource(RAudio::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eAUDIO;
  }

  double
  getDuration(const DELTA_TYPE::E& in);

private:

  int
  m_sampleRate = 0;

  int 
  m_channels = 0;

  Vector<float> 
  m_samples;

  



};
}

