#pragma once
#include "cyCorePrerequisites.h"
#include <cyEvent.h>

namespace CYLLENE_SDK {

  namespace COMPONENT_TYPE
  {
    BETTER_ENUM(E, uint32,
                eNONE = 0, // SHOULD NEVER BE THIS TYPE
                eANIMATOR,
                eAUDIOLISTENER,
                eAUDIOSOURCE,
                eCAMERA,
                eEMITTER,
                eCOLLIDER2D,
                eCOLLIDER3D,
                eLIGHTAMBIENT,
                eLIGHT3DAREA,
                eLIGHT3DDIRECTIONAL,
                eLIGHT3DPOINT,
                eLIGHT3DSPOT,
                eLIGHT2DFREEFORM,
                eLIGHT2DSPOT,
                eLIGHT2DSPRITE,
                eLIGHT2DGLOBAL,
                eMATERIAL,
                eMODEL,
                eSHADER,
                eSPRITE,
                eTEXT,
                eTRANSFORM);
  }
  
class Component
{
public:

  Component() = default;

  Component(const COMPONENT_TYPE::E& type) : m_type(type) {}

  virtual ~Component() = default;

  virtual COMPONENT_TYPE::E staticType() = 0;

  virtual void Update(const float& delta) = 0;

public:

  Event<void> m_onUpdate;

private:

  COMPONENT_TYPE::E m_type;
};

class ModelComponent : public Component
{
public:

  ModelComponent() : Component(ModelComponent::staticType()) {}

  ~ModelComponent() = default;

  virtual COMPONENT_TYPE::E staticType() override { return COMPONENT_TYPE::E::eMODEL; }
};

class ShaderComponent : public Component
{
public:

  ShaderComponent() : Component(ShaderComponent::staticType()) {}

  ~ShaderComponent() = default;

  virtual COMPONENT_TYPE::E staticType() override { return COMPONENT_TYPE::E::eSHADER; }
};

}
