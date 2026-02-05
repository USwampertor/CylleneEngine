#pragma once
#include "cyCorePrerequisites.h"

#include <cyEvent.h>
#include <cyMatrix4.h>


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
                eCOLLIDER2D,
                eCOLLIDER3D,
                eLIGHTAMBIENT,
                eLIGHT2D,
                eLIGHT3D,
                eMESHRENDERER,
                eMATERIALINSTANCE,
                ePARTICLEEMITTER,
                eSHADER,
                eSPRITE,
                eTEXT,
                eTRANSFORM);
  }
  
class CY_CORE_EXPORT CComponent
{
public:

  CComponent() {
    m_onDestroy += [this]() {
      this->m_owner.reset();
    };
  }

  CComponent(const COMPONENT_TYPE::E& type) : m_type(type) { }

  virtual ~CComponent() = default;

  void 
  setOwner(WPtr<BBeing> owner);

  const COMPONENT_TYPE::E& getType() { return m_type; }

  virtual COMPONENT_TYPE::E staticType() { 
    CY_ASSERT(true && "IMPLEMENT THIS"); 
    return COMPONENT_TYPE::E::eNONE;
  }

  virtual void 
  update(const float& delta) {}

  virtual const String
  toString() = 0;

  /*
   *	@brief	DEPRECATED: Use onCreate Event instead
   *	@param
   *  @return
   */
  virtual void 
  onCreate() {}

  /*
   *	@brief	DEPRECATED: Use onInit Event instead
   *	@param
   *  @return
   */
  virtual void 
  onInit() {}

  /*
   *	@brief	DEPRECATED: Use onDestroy Event instead
   *	@param		
   *  @return	
   */
  virtual void 
  onDestroy() {
    // m_owner.reset();
  }

  virtual void 
  init() {}

  WPtr<BBeing>&
  getOwner() {
    return m_owner;
  }

  virtual void
  applyTransformChanges(const Matrix4& newTransform) = 0; 

public:

  Event<void> m_onUpdate;

  Event<void> m_onCreate;

  Event<void> m_onInit;

  Event<void> m_onDestroy;

  Event<void, const Matrix4&> m_onTransformChanged;

protected:

  COMPONENT_TYPE::E m_type = COMPONENT_TYPE::E::eNONE;

  WPtr<BBeing> m_owner;
};


}
