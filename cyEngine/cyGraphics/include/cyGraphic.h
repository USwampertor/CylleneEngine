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
  virtual ~Graphic() = default;

};

}