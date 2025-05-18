#pragma once
#include "cyCorePrerequisites.h"

#include <cyEvent.h>

namespace CYLLENE_SDK {

  class BBeing;

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
                eLIGHT2D,
                eLIGHT3D,
                eLIGHT2DFREEFORM,
                eLIGHT2DGLOBAL,
                eLIGHT2DSPOT,
                eLIGHT2DSPRITE,
                eLIGHT3DAREA,
                eLIGHT3DDIRECTIONAL,
                eLIGHT3DPOINT,
                eLIGHT3DSPOT,
                eMESHRENDERER,
                eSHADER,
                eSPRITE,
                eTEXT,
                eTRANSFORM);
  }
  
class CY_CORE_EXPORT CComponent
{
public:

  CComponent() = default;

  CComponent(const COMPONENT_TYPE::E& type) : m_type(type) { }

  virtual ~CComponent() = default;

  void 
  setOwner(BBeing* owner) {
    m_owner = owner;
  }

  const COMPONENT_TYPE::E& getType() { return m_type; }

  virtual COMPONENT_TYPE::E staticType() { 
    CY_ASSERT(true && "IMPLEMENT THIS"); 
    return COMPONENT_TYPE::E::eNONE;
  }

  virtual void 
  update(const float& delta) {}

  virtual const String
  toString() = 0;

public:

  Event<void> m_onUpdate;

protected:

  COMPONENT_TYPE::E m_type = COMPONENT_TYPE::E::eNONE;

  BBeing* m_owner = nullptr;
};
// 
// class CY_CORE_EXPORT ModelComponent : public Component
// {
// public:
// 
//   ModelComponent() : Component(ModelComponent::staticType()) {}
// 
//   ~ModelComponent() = default;
// 
//   virtual COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eMODEL; }
// };
// 
// class CY_CORE_EXPORT ShaderComponent : public Component
// {
// public:
// 
//   ShaderComponent() : Component(ShaderComponent::staticType()) {}
// 
//   ~ShaderComponent() = default;
// 
//   virtual COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eSHADER; }
// };

}
