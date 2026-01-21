#pragma once
#include "cyCorePrerequisites.h"
#include <cyModule.h>
#include <cyThread.h>

namespace CYLLENE_SDK {

class CY_CORE_EXPORT AudioManager : public Module<AudioManager>
{
  public:

  AudioManager() = default;

  ~AudioManager() = default;

  Vector<String>
  getAvailableDevices();

  String
  getCurrentDevice();

  String
  getDefaultDevice();

  void
  setCurrentDevice(const String& deviceName);

  bool
  ensureIsInit();

  virtual void
  onStartUp() override;

  virtual void
  onShutDown() override;

};

  Thread 

}
