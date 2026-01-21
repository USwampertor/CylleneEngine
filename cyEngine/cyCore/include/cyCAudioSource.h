#pragma once
#include "cyCorePrerequisites.h"
#include "cyRAudio.h"
#include "cyCComponent.h"
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

  void
  play();

  void
  playDelayed(float delay);

  void
  stop();

  void
  pause();

  void
  playOnce(SPtr<RAudio> audio);

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

public:

  float m_volume = 1.0f;

  bool m_loop = false;

  bool m_spatialize = false;

  int32 m_priority = 128;

  bool m_isPlaying = false;

  bool m_mute = false;

  bool m_playOnAwake = false;

  bool m_bypassEffects = false;

  float m_pitch = 1.0f;

  WPtr<RAudio> m_clip;

private:

  void
  setClipBuffer();

  uint32 m_sourceId = 0;

  uint32 m_bufferId = 0;

  Event<void> m_onClipLoaded;

};

}
