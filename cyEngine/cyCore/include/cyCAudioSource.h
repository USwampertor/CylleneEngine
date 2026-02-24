#pragma once
#include "cyCorePrerequisites.h"

#include "cyAudioBackend.h"
#include "cyCComponent.h"
#include "cyRAudio.h"


#include <cyEvent.h>

namespace CYLLENE_SDK {

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

  void
  setClip(WPtr<RAudio> clip) {
    m_clip = clip;
    m_onClipLoaded.invoke();
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

  bool 
  m_loop = false;

  bool 
  m_spatialize = false;

  int32 
  m_priority;

  bool 
  m_isPlaying = false;

  bool
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

  Vector<PathContribution> m_pathContributions;

private:

  void
  setClipBuffer();

  uint32 
  m_sourceId = 0;

  uint32 
  m_bufferId = 0;

  uint64
  m_frameIndex = 0;

  Event<void> 
  m_onClipLoaded;

  APIAudio 
  m_apiAudio;

};

}
