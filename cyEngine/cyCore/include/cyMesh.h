#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

namespace CYLLENE_SDK {


class CY_CORE_EXPORT MeshResource : public Resource {
  
public:

  MeshResource() : Resource(MeshResource::staticType()) {}
  
  MeshResource(const Path& newFile) 
    : Resource(newFile) {
    m_type = MeshResource::staticType();
  }

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMODEL; 
  }

  virtual void
  setData(void*) override {

  }

};

}
