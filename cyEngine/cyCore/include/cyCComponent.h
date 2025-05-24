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

  virtual void 
  onCreate() {}

  virtual void 
  onInit() {}

  virtual void 
  onDestroy() {}

  virtual void 
  init() {}

public:

  Event<void> m_onUpdate;

protected:

  COMPONENT_TYPE::E m_type = COMPONENT_TYPE::E::eNONE;

  BBeing* m_owner = nullptr;
};


}
