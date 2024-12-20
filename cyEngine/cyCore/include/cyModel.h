#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

namespace CYLLENE_SDK {

class MeshResource;
class SkeletonResource;

class CY_CORE_EXPORT ModelResource : public Resource
{
public:
  ModelResource() : Resource(ModelResource::staticType()) {}
  
  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMODEL; 
  }

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  bool
  hasBones();

public:

  Vector<SharedPointer<MeshResource>> m_meshes;

  SharedPointer<SkeletonResource> m_skeleton;

  bool m_hasSkeleton;

};

}

