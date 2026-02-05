#include "cyCAudioSource.h"
#include <AL/al.h>
#include <AL/alc.h>

#include "cyAudioBackend.h"
#include "cyAudioManager.h"

#include <cyLogger.h>
#include <cyThread.h>

#include <algorithm>
#include <chrono>
#include <thread>

namespace CYLLENE_SDK {
namespace {

ALenum
getALFormat(int channels) {
  switch (channels) {
  case 1: return AL_FORMAT_MONO16;
  case 2: return AL_FORMAT_STEREO16;
  default: return 0;
  }
}

Vector<int16_t>
convertToPCM16(const Vector<float>& samples) {
  Vector<int16_t> pcm(samples.size());
  for (size_t i = 0; i < samples.size(); ++i) {
    float clamped = std::max(-1.0f, std::min(1.0f, samples[i]));
    pcm[i] = static_cast<int16_t>(clamped * 32767.0f);
  }
  return pcm;
}

uint32
uploadToBuffer(ALuint bufferId, RAudio& clip) {
  UPtr<AudioData> data(reinterpret_cast<AudioData*>(clip.getData()));
  if (!data || data->m_sampleRate <= 0 || data->m_channels <= 0 || data->m_samples.empty()) {
    Logger::instance().logError("OpenAL-soft: Invalid audio data passed to CAudioSource");
    return 0;
  }

  ALenum format = getALFormat(data->m_channels);
  if (format == 0) {
    Logger::instance().logError("OpenAL-soft: Unsupported channel configuration");
    return 0;
  }

  Vector<int16_t> pcm = convertToPCM16(data->m_samples);
  alBufferData(bufferId, 
               format, 
               pcm.data(),
               static_cast<ALsizei>(pcm.size() * sizeof(int16_t)),
               data->m_sampleRate);

  if (alGetError() != AL_NO_ERROR) {
    Logger::instance().logError("OpenAL-soft: Failed to upload audio buffer");
    if (bufferId && alIsBuffer(bufferId)) {
      alDeleteBuffers(1, &bufferId);
    }
    return 0;
  }
  return bufferId;
}

}


CAudioSource::CAudioSource() : CComponent(CAudioSource::staticType()) {
  // AudioBackend::ensureOpenAL();
  m_onClipLoaded += [this]() {
    setClipBuffer();
  };
  if (AudioManager::isStarted()) {
    if (AudioManager::instance().ensureIsInit()) {
      alGenSources(1, &m_sourceId);
    }
  }
}

CAudioSource::~CAudioSource() {
  if (alcGetCurrentContext()) {
    if (m_sourceId != 0) {
      alDeleteSources(1, &m_sourceId);
    }
    if (m_bufferId != 0) {
      alDeleteBuffers(1, &m_bufferId);
    }
  }
  m_sourceId = 0;
  m_bufferId = 0;
}

void
CAudioSource::play() {
  if (!AudioManager::instance().ensureIsInit()) {
    Logger::instance().logError("Error handling playback: OpenAL was not ensured");
  }

  SPtr<RAudio> clip = m_clip.lock();
  if (!clip) {
    Logger::instance().logWarning("CAudioSource::play called without an assigned audio clip");
    return;
  }

  alSourcePlay(m_sourceId);
  m_isPlaying = true;
}

void
CAudioSource::setClipBuffer() {

  if (m_bufferId == 0) {
    alGenBuffers(1, &m_bufferId);
  }
  uint32 result = uploadToBuffer(m_bufferId, *m_clip.lock());
  if (!result) {
    Logger::instance().logError("Error handling playback: OpenAL did not upload buffer");
    return;
  }

  m_bufferId = result;

  alSourcei(m_sourceId, AL_BUFFER, static_cast<ALint>(m_bufferId));
  alSourcef(m_sourceId, AL_GAIN, m_mute ? 0.0f : m_volume);
  alSourcei(m_sourceId, AL_LOOPING, m_loop ? AL_TRUE : AL_FALSE);
  alSourcef(m_sourceId, AL_PITCH, m_pitch);

}

void
CAudioSource::playDelayed(float delay) {

  enqueueToThread([this, delay]() {
    Miliseconds d(static_cast<int32>(delay));
    Threading::sleepFor(d);
    play();
  });
  // std::this_thread::sleep_for(std::chrono::duration<float>(delay));
}

void
CAudioSource::stop() {
  if (!AudioManager::instance().ensureIsInit() || m_sourceId == 0) {
    Logger::instance().logError("Error handling pause: OpenAL was not ensured");
  }

  alSourceStop(m_sourceId);
  alSourceRewind(m_sourceId);
  m_isPlaying = false;
}

void
CAudioSource::pause() {
  if (!AudioManager::instance().ensureIsInit() || m_sourceId == 0) {
    Logger::instance().logError("Error handling pause: OpenAL was not ensured");
  }

  alSourcePause(m_sourceId);
  m_isPlaying = false;
}

void
CAudioSource::playOnce(SPtr<RAudio> audio) {
  if (!audio) return;

  WPtr<RAudio> previousClip = m_clip;
  bool previousLoop = m_loop;

  // m_clip = audio;
  m_loop = false;
  setClip(audio);
  play();
  if (previousClip.lock() != nullptr) {
    setClip(previousClip);
  }
  // m_clip = previousClip;
  m_loop = previousLoop;
}

void
CAudioSource::setPosition(const Vector3f& position) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_position = position;
  alSource3f(m_sourceId, SOURCE_PROPERTIES::E::ePOSITION, position.x, position.y, position.z);
  // alSource3f(m_sourceId, AL_POSITION, position.x, position.y, position.z);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioSource::setVelocity(const Vector3f& velocity) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_velocity = velocity;
  alSource3f(m_sourceId, SOURCE_PROPERTIES::E::eVELOCITY, velocity.x, velocity.y, velocity.z);
  // alSource3f(m_sourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioSource::setDirection(const Vector3f& forward) {
  if (!AudioManager::instance().ensureIsInit()) {
    return;
  }
  // m_forward = forward;
  // m_up = up;
  alSource3f(m_sourceId, SOURCE_PROPERTIES::E::eDIRECTION, forward.x, forward.y, forward.z);
  // alSource3f(m_sourceId, AL_DIRECTION, forward.x, forward.y, forward.z);
  AUDIO_ERROR::E error = AUDIO_ERROR::E::_from_integral(alGetError());
  if (error != +AUDIO_ERROR::E::eNONE) {
    Logger::instance().logError(Utils::format("alGenBuffers: %s", error._to_string()));
  }
}

void
CAudioSource::applyTransformChanges(const Matrix4& newTransform) {
  if (!AudioManager::instance().ensureIsInit() || m_sourceId == 0) {
    Logger::instance().logError("Error handling transform changes: OpenAL was not ensured");
  }
  Vector3f position = newTransform.getPosition();
  Vector3f forward = newTransform.getForwardVector();
  // TODO: Calculate velocity based on previous position and time delta
  Vector3f velocity = Vector3f::ZERO; 
  
  setPosition(position);
  setDirection(forward);
  setVelocity(velocity);
}

}
