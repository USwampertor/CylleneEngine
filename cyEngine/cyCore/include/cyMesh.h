#pragma once

#include "cyCorePrerequisites.h"

#include "cyResource.h"
#include "cyVertex.h"

namespace CYLLENE_SDK {


class CY_CORE_EXPORT MeshResource : public Resource {
  
public:

  MeshResource() : Resource(MeshResource::staticType()) {}
  
  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMODEL; 
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;
public:

  Vector<Vertex> m_vertices;

};

}
