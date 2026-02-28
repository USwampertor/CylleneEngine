#pragma once

#include "cyCorePrerequisites.h"

#include "cyAudioBackend.h"
#include "cyCComponent.h"
#include "cyRay.h"

namespace CYLLENE_SDK {

  class AudioRay : public Ray
  {
    AudioRay() : Ray() {}
  
    AudioRay(const Vector3f& origin, const Vector3f& direction)
      : Ray(origin, direction) {
      m_origin = origin;
      m_direction = direction.normalized();
    }
  
  private:
  
    float m_energy[AUDIO_NUM_BANDS];
  
  };

class CAudioMaterial : public CComponent {

  CAudioMaterial() : CComponent(CAudioMaterial::staticType()) {}

  CAudioMaterial(const std::array<float, AUDIO_NUM_BANDS>& abs,
                 const std::array<float, AUDIO_NUM_BANDS>& trans,
                 float scatter = 0.0f,
                 float diffScale = 1.0f)
    : CComponent(CAudioMaterial::staticType()),
      m_absorption(abs),
      m_transmission(trans),
      m_scattering(scatter),
      m_diffractionScale(diffScale) {
        computeReflection();
    }


  ~CAudioMaterial() = default;
  
  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eAUDIOMATERIAL; }

  virtual const String
  toString() override {
    return Utils::format("Audio Material with properties");
  }

  void 
  computeReflection();

  void
  applyReflection(float energy[AUDIO_NUM_BANDS]) const;

  void
  applyTransmission(float energy[AUDIO_NUM_BANDS]) const;

private:

  Array<float, AUDIO_NUM_BANDS> m_absorption = { 0.0f };

  Array<float, AUDIO_NUM_BANDS> m_transmission = { 0.0f };

  Array<float, AUDIO_NUM_BANDS> m_reflection = { 1.0f };

  float m_scattering = 0.0f;

  float m_diffractionScale = 1.0f;

  float m_transmissionLoss = 1.0f;

};


}
