#pragma once
#include "cyCorePrerequisites.h"

#include "cyCComponent.h"
#include <cyColor.h>

namespace CYLLENE_SDK {


  namespace LIGHT3DTYPE
  {
    BETTER_ENUM(E, uint32, AREA, DIRECTIONAL, DOT, GLOBAL, SPOTLIGHT);
  }

class CY_CORE_EXPORT CLight : CComponent
{
  CLight() : CComponent(CLight::staticType()) {}

  ~CLight() = default;

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eLIGHT3D; }

  virtual const String
    toString() override {
    String toReturn = m_type._to_string();
    return toReturn;
  }

public:

  LIGHT3DTYPE::E m_type = LIGHT3DTYPE::E::GLOBAL;

  float m_range;
  float m_intensity;
  Color m_color = Color::WHITE;
  bool m_castsShadows;
  
  // Dot/Point/Spotlight
  float m_falloff;
  float m_falloffType;

  // Spotlight
  float m_angle;
  float m_anglefalloff;
  float m_anglefalloffType;
};

}

