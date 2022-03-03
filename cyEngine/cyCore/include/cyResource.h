#pragma once

#include "cyCorePrerequisites.h"



namespace CYLLENE_SDK {

  namespace RESOURCE_TYPE {
    BETTER_ENUM(E, uint32, 
      eNONE,
      eMESH, 
      eAUDIO, 
      eTEXTURE );
  }
  
class CY_CORE_EXPORT Resource {
  
  Resource() = default;

  virtual ~Resource();

};
}
