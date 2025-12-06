#include "cyCAudioListener.h"
#include "cyAudioBackend.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <cyLogger.h>

#include <algorithm>

namespace CYLLENE_SDK {
namespace {

void
setListenerOrientation(const Vector3f& forward, const Vector3f& up) {
  const float orientation[6] = {
    forward.x, forward.y, forward.z,
    up.x, up.y, up.z
  };
  alListenerfv(AL_ORIENTATION, orientation);
}

}

CAudioListener::CAudioListener() : CComponent(CAudioListener::staticType()) {
  AudioBackend::ensureOpenAL();
  setPosition(m_position);
  setVelocity(m_velocity);
  setOrientation(m_forward, m_up);
  setGain(m_gain);
}

CAudioListener::~CAudioListener() = default;

void
CAudioListener::setPosition(const Vector3f& position) {
  if (!AudioBackend::ensureOpenAL()) return;
  m_position = position;
  alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void
CAudioListener::setVelocity(const Vector3f& velocity) {
  if (!AudioBackend::ensureOpenAL()) return;
  m_velocity = velocity;
  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void
CAudioListener::setOrientation(const Vector3f& forward, const Vector3f& up) {
  if (!AudioBackend::ensureOpenAL()) return;
  m_forward = forward;
  m_up = up;
  setListenerOrientation(m_forward, m_up);
}

void
CAudioListener::setGain(float gain) {
  if (!AudioBackend::ensureOpenAL()) return;
  m_gain = std::max(0.0f, gain);
  alListenerf(AL_GAIN, m_gain);
}

}
