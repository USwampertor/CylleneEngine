#pragma once
#include "cyRResource.h"

namespace CYLLENE_SDK {

class CY_CORE_EXPORT RScript : public RResource
{
  RScript() : RResource(RScript::staticType()) {}

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

