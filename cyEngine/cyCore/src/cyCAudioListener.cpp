#include "cyCAudioListener.h"

#include <algorithm>
#include <cyLogger.h>

#include "cyAudioBackend.h"
#include "cyAudioManager.h"
#include "cyBBeing.h"
#include "cyCTransform.h"

namespace CYLLENE_SDK {
namespace {

void
setListenerOrientation(const Vector3f& forward, const Vector3f& up) {
  const float orientation[6] = {
    forward.x, forward.y, forward.z,
    up.x, up.y, up.z
  };
  alListenerfv(LISTENER_PROPERTIES::E::eORIENTATION, orientation);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

}

CAudioListener::CAudioListener() : CComponent(CAudioListener::staticType()) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }

  m_onInit += [this]() {
    auto owner = this->getOwner();
    if (owner.lock()) {
      auto transform = owner.lock()->getTransform();
      setPosition(transform.lock()->getWorldPosition());
      setVelocity(Vector3f(0,0,0));
      setOrientation(transform.lock()->getWorldTransform().getForwardVector(), 
                     transform.lock()->getWorldTransform().getUpVector());
      setGain(m_gain);
    }
    };
}

CAudioListener::~CAudioListener() = default;

void
CAudioListener::setPosition(const Vector3f& position) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_position = position;
  alListener3f(LISTENER_PROPERTIES::E::ePOSITION, position.x, position.y, position.z);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioListener::setVelocity(const Vector3f& velocity) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_velocity = velocity;
  alListener3f(LISTENER_PROPERTIES::E::eVELOCITY, velocity.x, velocity.y, velocity.z);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioListener::setOrientation(const Vector3f& forward, const Vector3f& up) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_forward = forward;
  // m_up = up;
  setListenerOrientation(forward, up);
}

void
CAudioListener::setGain(float gain) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  m_gain = std::max(0.0f, gain);
  alListenerf(LISTENER_PROPERTIES::E::eGAIN, m_gain);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioListener::applyTransformChanges(const Matrix4& newTransform) {

  if (!AudioManager::instance().ensureIsInit()) {
    Logger::instance().logError("Error handling listener transform changes: OpenAL was not ensured");
    return;
  }

  Vector3f newPosition = newTransform.getPosition();
  Vector3f newForward = newTransform.getForwardVector();
  Vector3f newUp = newTransform.getUpVector();

  setPosition(newPosition);
  setOrientation(newForward, newUp);
}

}
