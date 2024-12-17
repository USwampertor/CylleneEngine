#pragma once

#include "cyCorePrerequisites.h"

#include "cyResource.h"

namespace CYLLENE_SDK {
  
class CY_CORE_EXPORT ShaderResource : public Resource {

public:

  ShaderResource() : Resource(ShaderResource::staticType()) {}

  ShaderResource(const Path& newFile)
    : Resource(newFile), m_isBlob(false) {
    m_type = ShaderResource::staticType();
  }

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eSHADER;
  }

  virtual void
  setData(void*) override {

  }

  bool m_isBlob;
};
}


