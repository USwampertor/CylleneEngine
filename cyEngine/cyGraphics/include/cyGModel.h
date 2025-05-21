#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyGMesh.h"

namespace CYLLENE_SDK
{

struct CY_GRAPHICS_EXPORT GModel
{
  GModel() = default;
  
  ~GModel() = default;
  
  Vector<SPtr<GMesh>> m_meshes;

};

}