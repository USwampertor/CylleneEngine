#pragma once
#include "cyCorePrerequisites.h"

#include "cyAudioBackend.h"
#include "cyCComponent.h"
#include "cyRAudio.h"


#include <cyEvent.h>

#include <atomic>

namespace CYLLENE_SDK {

template<typename T>
using Atomic = std::atomic<T>;


/**
 * @struct AudioPath
 * @brief Represents the propagation data path
 */
// struct AudioPath {
//   float delay; // seconds
//   float attenuation[AUDIO_NUM_BANDS]; 
//   Vector3f direction; // incoming direction for spatialization
// 
//   float dopplerFactor = 1.0f; // for doppler effect
// };

/*
 *	@class	CAudioSource
 *	@brief	An audio source that holds an audio clip to be played.
 *
 */
class CY_CORE_EXPORT CAudioSource : public CComponent {
public: 

  CAudioSource();
  
  ~CAudioSource();

  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eAUDIOSOURCE; }

  virtual const String
  toString() override {
    return Utils::format("%s is %s", (m_clip.lock() ? 
                                      m_clip.lock()->getName().c_str() : 
                                      "audio source without clip"), 
                                     (m_isPlaying ?
                                      "playing" :
                                      "not playing"));
  }

  virtual void
  applyTransformChanges(const Matrix4& newTransform) override;

  void
  play();

  void
  playDelayed(float delay);

  void
  playOnce(SPtr<RAudio> audio);

  void
  stop();

  void
  pause();

  void
  setVolume(float volume) {
    m_volume = Math::clamp(volume, 0.0f, 1.0f);
  }

  const float&
  getMinDistance() const { return m_minDistance; }

  const float&
  getMaxDistance() const { return m_maxDistance; }

  void
  setMinDistance(float minDistance) {
    m_minDistance = Math::max(0.0f, minDistance);
  } 

  void
  setMaxDistance(float maxDistance) {
    m_maxDistance = Math::max(0.0f, maxDistance);
  }

  void
  setClip(WPtr<RAudio> clip) {
    m_clip = clip;
    m_onClipLoadedInternal.invoke();
  }

  WPtr<RAudio>
  getClip() {
    return m_clip;
  }

  bool
  isPlaying() const {
    return m_isPlaying;
  }

private:

  void
  setPosition(const Vector3f& position);

  void
  setVelocity(const Vector3f& velocity);

  void
  setDirection(const Vector3f& forward);

#if AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO
  int32
  playCallback(void* outputBuffer,
               void* inputBuffer,
               unsigned int nFrames,
               double streamTime,
               int32 status,
               void* userData);
#endif // AUDIO_BACKEND

public:

  float 
  m_volume = 1.0f;

  Atomic<bool>
  m_loop = false;

  bool 
  m_spatialize = false;

  int32 
  m_priority;

  Atomic<bool>
  m_isPlaying = false;

  Atomic<bool>
  m_isPaused = false;

  bool 
  m_mute = false;

  bool 
  m_playOnAwake = false;

  bool 
  m_bypassEffects = false;

  float 
  m_pitch = 1.0f;

  WPtr<RAudio> 
  m_clip;

  float
  m_gain = 1.0f;

  float
  m_minDistance = 1.0f;

  float 
  m_maxDistance = 100.0f;

  Vector<PathContribution> 
  m_pathContributions[2];
  
  Atomic<uint32>
  m_activePathBufferIndex = 0;

  Event<void>
  m_onAudioPlay;

  Event<void>
  m_onAudioStop;

  Event<void>
  m_onClipLoad;

private:

  void
  setClipBuffer();

  uint32 
  m_sourceId = 0;

  uint32 
  m_bufferId = 0;

  Atomic<uint64>
  m_frameIndex = 0;


  Event<void> 
  m_onClipLoadedInternal;

  APIAudio 
  m_apiAudio;

};

}
