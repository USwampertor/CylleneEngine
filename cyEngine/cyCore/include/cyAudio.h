#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

namespace CYLLENE_SDK {
class CY_CORE_EXPORT AudioResource : public Resource {

public:

  AudioResource() : Resource(AudioResource::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eAUDIO;
  }
};
}

