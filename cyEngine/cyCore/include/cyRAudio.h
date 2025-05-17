#pragma once

#include "cyCorePrerequisites.h"
#include "cyRResource.h"

namespace CYLLENE_SDK {
class CY_CORE_EXPORT RAudio : public RResource {

public:

  RAudio() : RResource(RAudio::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eAUDIO;
  }
};
}

