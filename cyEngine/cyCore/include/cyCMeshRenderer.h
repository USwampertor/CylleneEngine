#pragma once
#include "cyCorePrerequisites.h"
#include "cyCComponent.h"

#include "cyRModel.h"
#include "cyRMesh.h"

#include "cyRMaterialInstance.h"

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
    if (m_mesh) {
      createMaterialInstance();
    }
  }

  ~CMeshRenderer() = default;

  void
  setMesh(const SPtr<RMesh>& newMesh) {
    m_mesh = newMesh;
    createMaterialInstance();
  }

  void
  setModel(const SPtr<RModel>& newModel);

  void
  createMaterialInstance();

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eMESHRENDERER; }

  virtual const String
  toString() override {
    String toReturn = "";
    return toReturn;
  }

  SPtr<RMesh> m_mesh = nullptr;
  bool m_castsShadows = false;


  WPtr<RMaterialInstance> m_materialInstance;

};


}

