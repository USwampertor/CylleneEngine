#pragma once
#include "cyCorePrerequisites.h"

#include "cyCComponent.h"
#include <cyVector3f.h>
#include "cyRMesh.h"

namespace CYLLENE_SDK {

class CParticleEmitter : public CComponent
{
public:
  CParticleEmitter() : CComponent(CParticleEmitter::staticType()) {}

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::ePARTICLEEMITTER; }

  virtual const String 
  toString() override {
    String toReturn;

    return toReturn;
  }

  virtual void
  applyTransformChanges(const Matrix4& newTransform) override {}

  float m_amount;

  float m_unitLifeTime;

  bool m_loop;
  
  bool m_usesGravity;

  float m_duration;

  Vector3f m_speed;

  SPtr<RMesh> m_mesh;

};

}

