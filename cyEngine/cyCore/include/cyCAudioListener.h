#pragma once
#include "cyCorePrerequisites.h"
#include "cyCComponent.h"

#include <cyVector3f.h>

namespace CYLLENE_SDK {

/*
 *  @class  CAudioListener
 *  @brief  Represents the global OpenAL listener state.
 */
class CY_CORE_EXPORT CAudioListener : public CComponent {
public:

  CAudioListener(); // CAudioListener() : CComponent(CAudioListener::staticType()) {}

  ~CAudioListener() = default;

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eAUDIOLISTENER; }

  virtual const String
  toString() override {
    return Utils::format("Audio Listener with gain: %2.2f", m_gain);
  }

  void
  setGain(float gain);

  float
  getGain() const { return m_gain; }

  void
  applyTransformChanges(const Matrix4& newTransform) override;

private:

  void
  setPosition(const Vector3f& position);

  void
  setVelocity(const Vector3f& velocity);

  void
  setOrientation(const Vector3f& forward, const Vector3f& up);

// TODO: Maybe remove these as they can be redundant with the transform component
//   Vector3f m_position = Vector3f::ZERO;
//   Vector3f m_velocity = Vector3f::ZERO;
//   Vector3f m_forward = Vector3f::FRONT;
//   Vector3f m_up = Vector3f::UP;


  float m_gain = 1.0f;
};


}
