#pragma once

#include "cyCorePrerequisites.h"

#include "cyMaterial.h"
#include "cyResource.h"
#include "cyVertex.h"

namespace CYLLENE_SDK {


class CY_CORE_EXPORT MeshResource : public Resource {
  
public:

  MeshResource() : Resource(MeshResource::staticType()) {}
  
  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMESH; 
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

public:

  Vector<Vertex> m_vertexBuffer;

  Vector<uint32> m_indexBuffer;

  SPtr<MaterialResource> m_material;
};

}
