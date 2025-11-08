#pragma once

#include "cyCorePrerequisites.h"
#include "cyRResource.h"

namespace CYLLENE_SDK {

class RMesh;
class RSkeleton;

class CY_CORE_EXPORT RModel : public RResource
{
public:
  RModel() : RResource(RModel::staticType()) {}
  
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

  Vector<SPtr<RMesh>> m_meshes;

  SPtr<RSkeleton> m_skeleton;

  bool m_hasSkeleton = false;

};

}

