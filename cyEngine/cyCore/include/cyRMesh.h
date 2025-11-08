#pragma once

#include "cyCorePrerequisites.h"

#include "cyRMaterial.h"
#include "cyRResource.h"
#include "cyVertex.h"

namespace CYLLENE_SDK {


class CY_CORE_EXPORT RMesh : public RResource {
  
public:

  RMesh();
  
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

  // Optional: material name bound to this mesh (filled by importer)
  String m_materialName;

  UnorderedMap<String, String> m_importData;
};

}
