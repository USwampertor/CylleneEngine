#pragma once
#include "cyResource.h"

namespace CYLLENE_SDK {

class CY_CORE_EXPORT ScriptResource : public Resource
{
  ScriptResource() : Resource(ScriptResource::staticType()) {}

  static RESOURCE_TYPE::E
    staticType() {
    return RESOURCE_TYPE::E::eSCRIPT;
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

};

}

