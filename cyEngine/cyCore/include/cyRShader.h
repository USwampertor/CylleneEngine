#pragma once

#include "cyCorePrerequisites.h"

#include "cyRResource.h"
#include <better-enums/enum.h>


namespace CYLLENE_SDK {
  
  namespace SHADER_TYPE
  {
    BETTER_ENUM(E, int32, 
                UNKNOWN = -1, 
                VERTEX = 0, 
                PIXEL = 1, 
                GEOMETRY = 2, 
                COMPUTE = 3, 
                TDOMAIN = 4,
                THULL = 5,
                TCONTROL = 6, 
                TEVAL = 7, 
                UNIFIED = 8, 
                RAYTRACING = 9 );
  }

  namespace SHADER_LANGUAGE
  {
    BETTER_ENUM(E, int32, UNKNOWN, HLSL, GLSL, CYSL);
  }


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

  SHADER_TYPE::E
  getShaderType() { return m_shaderType; }

public:

  /**
   * The text that makes up the shader
   */
  String m_data;

  /**
   * if its a blob of data
   */
  bool m_isBlob;

  /**
   * The type of shader this is intended to be used for
   */
  SHADER_TYPE::E m_shaderType = SHADER_TYPE::E::UNKNOWN;

};
}


