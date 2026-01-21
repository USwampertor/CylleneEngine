#include "cyAudioManager.h"
#include <cyLogger.h>

#include <AL/al.h>
#include <AL/alc.h>

namespace CYLLENE_SDK {

void
AudioManager::onStartUp() {
  String name;
  ALCdevice* device;
  ALCcontext* ctx;

  /* Open and initialize a default device */
  device = nullptr;
  device = alcOpenDevice(nullptr);
  
  if (!device)
  {
    Logger::instance().logError("Could not open a device!\n");
    return;
  }

  ctx = alcCreateContext(device, nullptr);
  if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE)
  {
    if (ctx != nullptr) {
      alcDestroyContext(ctx);
    }
    alcCloseDevice(device);
    Logger::instance().logError("Could not set a context!\n");
    return;
  }

  // name = nullptr;
  if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT")) {
    name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
  }
  if (name.empty() || alcGetError(device) != AL_NO_ERROR) {
    name = alcGetString(device, ALC_DEVICE_SPECIFIER);
  }
  Logger::instance().log(Utils::format("Opened \"%s\"\n", name));
  Logger::instance().log("AudioManager started up.");
}

Vector<String>
AudioManager::getAvailableDevices() {
  Vector<String> devices = {};
  int code = 0;
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_TRUE) {
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") != AL_TRUE) {
      Logger::instance().logWarning("Device enumeration not supported.");
      return devices;
    }
    code = ALC_DEVICE_SPECIFIER;
  }
  code = ALC_ALL_DEVICES_SPECIFIER;

  auto stringList = alcGetString(nullptr, code);
  if (!stringList || alcGetError(nullptr) != AL_NO_ERROR) {
    Logger::instance().logWarning("Could not get list of devices.");
    return devices;
  }

  String stringListStr = String(stringList);
  StringStream ss(stringListStr);
  String device;

  while (std::getline(ss, device, '\0')) {
    devices.push_back(device);
  }
  return devices;
}

String
AudioManager::getCurrentDevice() {
  ALCcontext* ctx = alcGetCurrentContext();
  if (ctx == nullptr) {
    return "";
  }
  ALCdevice* device = alcGetContextsDevice(ctx);
  if (device == nullptr) {
    return "";
  }
  String name = alcGetString(device, ALC_DEVICE_SPECIFIER);
  return name;
}

void
AudioManager::setCurrentDevice(const String& deviceName) {
  // Shut down current context and device
  onShutDown();
  // Open new device
  ALCdevice* device = alcOpenDevice(deviceName.c_str());
  if (!device) {
    Logger::instance().logError("Could not open audio device: " + deviceName);
    return;
  }
  // Create new context
  ALCcontext* ctx = alcCreateContext(device, nullptr);
  if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE) {
    if (ctx != nullptr) {
      alcDestroyContext(ctx);
    }
    alcCloseDevice(device);
    Logger::instance().logError("Could not set audio context for device: " + deviceName);
    return;
  }
  Logger::instance().log("Switched to audio device: " + deviceName);
}

String
AudioManager::getDefaultDevice() {
  int code = 0;
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_TRUE) {
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") != AL_TRUE) {
      Logger::instance().logWarning("Device enumeration not supported.");
      return "";
    }
    code = ALC_DEFAULT_DEVICE_SPECIFIER;
  }
  code = ALC_DEFAULT_ALL_DEVICES_SPECIFIER;

  String defaultDevice = alcGetString(nullptr, code);
  if (defaultDevice.empty() || alcGetError(nullptr) != AL_NO_ERROR) {
    Logger::instance().logWarning("Could not get default device.");
    return "";
  }
  return defaultDevice;
}

bool
AudioManager::ensureIsInit() {
  ALCcontext* ctx = alcGetCurrentContext();
  return ctx != nullptr;
}

void
AudioManager::onShutDown() {

  ALCdevice* device;
  ALCcontext* ctx;

  ctx = alcGetCurrentContext();
  if (ctx == nullptr) {
    return;
  }

  device = alcGetContextsDevice(ctx);

  alcMakeContextCurrent(nullptr);
  alcDestroyContext(ctx);
  alcCloseDevice(device);

  Logger::instance().log("AudioManager shut down.");
}


}
