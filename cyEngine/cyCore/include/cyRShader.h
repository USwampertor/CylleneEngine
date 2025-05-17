#pragma once

#include "cyCorePrerequisites.h"

#include "cyRResource.h"

namespace CYLLENE_SDK {
  
class CY_CORE_EXPORT RShader : public RResource {

public:

  RShader() : RResource(RShader::staticType()) {}

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


