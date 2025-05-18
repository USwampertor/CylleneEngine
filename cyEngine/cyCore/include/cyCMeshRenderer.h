#pragma once
#include "cyCorePrerequisites.h"
#include "cyCComponent.h"
#include "cyRMesh.h"

namespace CYLLENE_SDK
{

class CMeshRenderer : CComponent
{
public:

  CMeshRenderer(SPtr<RMesh> mesh = nullptr)
    : CComponent(CMeshRenderer::staticType()) {
    m_mesh = mesh;
  }



  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eMESHRENDERER; }

  virtual const String
  toString() override {
    String toReturn;
    return toReturn;
  }

  SPtr<RMesh> m_mesh;

  bool m_castsShadows;
};

}

