#pragma once
#include "cyCorePrerequisites.h"
#include "cyCComponent.h"

#include "cyRMesh.h"

#include <cyUtilities.h>


namespace CYLLENE_SDK
{

class CY_CORE_EXPORT CMeshRenderer : public CComponent
{
public:

  // TODO: Check how to pass 
  CMeshRenderer(const SPtr<RMesh>& newMesh = nullptr)
    : CComponent(CMeshRenderer::staticType()) {
    m_mesh = newMesh;
  }

  ~CMeshRenderer() = default;

  void
  setMesh(const SPtr<RMesh>& newMesh) {
    m_mesh = newMesh;
  }

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eMESHRENDERER; }

  virtual const String
  toString() override {
    String toReturn = "";
    return toReturn;
  }

  SPtr<RMesh> m_mesh = nullptr;

  bool m_castsShadows = false;
};


}

