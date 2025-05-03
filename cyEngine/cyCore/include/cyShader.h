#pragma once

#include "cyCorePrerequisites.h"

#include "cyResource.h"

namespace CYLLENE_SDK {
  
class CY_CORE_EXPORT ShaderResource : public Resource {

public:

  ShaderResource() : Resource(ShaderResource::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eSHADER;
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

public:

  /**
   * The text that makes up the shader
   */
  String m_data;

  /**
   * if its a blob of data
   */
  bool m_isBlob;
};
}


