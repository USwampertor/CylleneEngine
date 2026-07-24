
#pragma once
#include "cyCorePrerequisites.h"

#include <cyModule.h>
#include <cyThread.h>

#include "cyAudioBackend.h"

namespace CYLLENE_SDK {

class CAudioSource;

class CAudioListener;

struct AudioDevice
{
  String 
  name;

  int32 
  id = -1;

  bool
  probed;

  int32
  outputChannels;

  int32
  intputChannels;

  int32
  duplexChannels;

  bool
  isDefaultOutput;

  Vector<uint32> 
  supportedSampleRates;

  Bitset<32>   
  nativeFormats;

};

struct AudioParameters
{
  int32
  sampleRate;
  
  int32
  channels;

  AUDIO_FORMAT::E
  format;
};


class CY_CORE_EXPORT AudioManager : public Module<AudioManager>
{
 
 public:

  AudioManager() = default;

  ~AudioManager() = default;

  virtual void
  onStartUp() override;

  virtual void
  onShutDown() override;

  Vector<AudioDevice>
  getAvailableDevices();

  AudioDevice
  getCurrentDevice();

  AudioDevice
  getDefaultDevice();

  void
  setCurrentDevice(const String& deviceName);

  void
  setCurrentDevice(int32 deviceID);

  bool
  ensureIsInit();

  void
  setBufferFrames(uint32 frames = 512) { m_bufferFrames = frames; }

  uint32
  getBufferFrames() const { return m_bufferFrames; }

 private:

  int32
  soundCallback(void* outputBuffer,
                void* inputBuffer,
                unsigned int nFrames,
                double streamTime,
                int32 status,
                void* userData);
  
 public:

  AudioDevice
  m_currentDevice;

  uint32 
  m_bufferFrames = 512;

 private:


  Vector<SPtr<CAudioSource>> m_audioSources;

  Vector<SPtr<CAudioListener>> m_audioListeners;

}; 

}
