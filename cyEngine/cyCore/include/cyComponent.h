#pragma once
#include "cyCorePrerequisites.h"

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

  virtual ~Component() = default;

  virtual COMPONENT_TYPE::E staticType() = 0;
};

class ModelComponent : public Component
{
public:

  ModelComponent() = default;

  ~ModelComponent() = default;

  virtual COMPONENT_TYPE::E staticType() override { return COMPONENT_TYPE::E::eMODEL; }
};

class ShaderComponent : public Component
{
public:

  ShaderComponent() = default;

  ~ShaderComponent() = default;

  virtual COMPONENT_TYPE::E staticType() override { return COMPONENT_TYPE::E::eSHADER; }
};

}
