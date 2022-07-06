#pragma once
#include "cyCorePrerequisites.h"

namespace CYLLENE_SDK {

  namespace COMPONENT_TYPE
  {
    BETTER_ENUM(E, uint32,
                eNONE = 0, // SHOULD NEVER BE THIS TYPE
                eTRANSFORM,
                eMODEL,
                eAUDIOSOURCE,
                eCOLLIDER3D,
                eCOLLIDER2D);
  }
  
class Component
{
public:

  Component() = default;

  virtual ~Component() = default;

  virtual COMPONENT_TYPE::E isType() = 0;
};

class ModelComponent : public Component
{
public:

  ModelComponent() = default;

  ~ModelComponent() = default;

  virtual COMPONENT_TYPE::E isType() override { return COMPONENT_TYPE::E::eMODEL; }

  

};

}
