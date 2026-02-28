#include "cyCAudioMaterial.h"

namespace CYLLENE_SDK {

void
CAudioMaterial::computeReflection() {
  for (int32 i = 0; i < AUDIO_NUM_BANDS; ++i) {
    m_reflection[i] = 1.0f - m_absorption[i] - m_transmission[i];
    m_reflection[i] = Math::max(0.0f, Math::min(1.0f, m_reflection[i]));
  }
}

void
CAudioMaterial::applyReflection(float energy[AUDIO_NUM_BANDS]) const {
  for (int32 i = 0; i < AUDIO_NUM_BANDS; ++i) {
    energy[i] *= m_reflection[i];
  }
}

void
CAudioMaterial::applyTransmission(float energy[AUDIO_NUM_BANDS]) const {
  for (int32 i = 0; i < AUDIO_NUM_BANDS; ++i) {
    energy[i] *= m_transmission[i] * m_transmissionLoss;
  }
}

}
