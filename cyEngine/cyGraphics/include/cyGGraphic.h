#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{

  namespace PRIMITIVE_TOPOLOGY
  {
    BETTER_ENUM(E, int32,
      eUNKNOWN = -1,
      ePOINTLIST = 0,
      eLINELIST = 1,
      eLINESTRIP = 2,
      eTRIANGLELIST = 3,
      eTRIANGLESTRIP = 4,
      eLINELIST_ADJ = 5,
      eLINESTRIP_ADJ = 6,
      eTRIANGLELIST_ADJ = 7,
      eTRIANGLESTRIP_ADJ = 8
    );
  }

  namespace GRAPHIC_TYPE
  {
    BETTER_ENUM(E, int32,
      eUNKNOWN = -1,
      eNONE = 0,
      eMODEL = 1,
      eMATERIAL = 2,
      eTEXTURE = 3,
      eSHADER = 5,
      eRENDER_TARGET = 5,
      eVERTEX_BUFFER = 6,
      eINDEX_BUFFER = 7,
      eCONSTANT_BUFFER = 8,
      eINPUT_LAYOUT = 9
    );
  }

class CY_GRAPHICS_EXPORT GGraphic
{
public:
  /*
   * You shouldn't be doing this but hey, to each their own 
   */
  GGraphic() = default;
  
  virtual ~GGraphic() {}

  // static GRAPHIC_TYPE::E
  // staticType() {
  //   CY_ASSERT(true && "Graphic::staticType() should never be called");
  //   return GRAPHIC_TYPE::E::eNONE;
  // }

  virtual void*
  get() = 0;

  virtual void
  set(void*) = 0;

  // const GRAPHIC_TYPE::E& getType() { return m_type; }

protected:

  // GRAPHIC_TYPE::E m_type = GRAPHIC_TYPE::E::eUNKNOWN;


};

}