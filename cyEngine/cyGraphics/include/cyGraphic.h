#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{

  namespace PRIMITIVE_TOPOLOGY
  {
    BETTER_ENUM(E, uint32,
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

class Graphic
{
public:
  /*
   * You shouldn't be doing this but hey, to each their own 
   */
  Graphic() = default;
  
  virtual ~Graphic() = default;

  virtual void*
  get() = 0;

  virtual void
  set(void*) = 0;



};

}