#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

namespace CYLLENE_SDK {
class CY_CORE_EXPORT Shader : public Resource {
public:

  virtual RESOURCE_TYPE::E
  getType() override { return RESOURCE_TYPE::E::eSHADER; }

};
}
