#include "cyAudioManager.h"
#include <cyLogger.h>

#include "cyAudioBackend.h"


namespace CYLLENE_SDK {

void
AudioManager::onStartUp() {
  String deviceName;
#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
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
    deviceName = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
  }
  if (deviceName.empty() || alcGetError(device) != AL_NO_ERROR) {
    deviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);
  }
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO
  m_currentDevice = getDefaultDevice();
  deviceName = m_currentDevice.name;
#endif // AUDIO_BACKEND

  Logger::instance().log(Utils::format("Opened \"%s\"\n", deviceName));
  Logger::instance().log("AudioManager started up.");
}

Vector<AudioDevice>
AudioManager::getAvailableDevices() {

  Vector<AudioDevice> devices = {};

#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  int32 code = 0;
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
    AudioDevice audioDevice;
    audioDevice.name = device;
    devices.push_back(audioDevice);
  }

#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

  APIAudio audio;

  int32 deviceCount = audio.getDeviceCount();

  for (int32 i = 0; i < deviceCount; ++i) {
    auto deviceInfo = audio.getDeviceInfo(i);

    AudioDevice audioDevice;
    audioDevice.name = deviceInfo.name;
    audioDevice.id = deviceInfo.ID;
    audioDevice.outputChannels = deviceInfo.outputChannels;
    audioDevice.intputChannels = deviceInfo.inputChannels;
    audioDevice.duplexChannels = deviceInfo.duplexChannels;
    audioDevice.isDefaultOutput = deviceInfo.isDefaultOutput;
    audioDevice.supportedSampleRates = deviceInfo.sampleRates;
    audioDevice.nativeFormats = Bitset<32>(deviceInfo.nativeFormats);

    devices.push_back(audioDevice);
  }


#endif // AUDIO_BACKEND

  return devices;
}

AudioDevice
AudioManager::getCurrentDevice() {
  AudioDevice device;
#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  ALCcontext* ctx = alcGetCurrentContext();
  if (ctx == nullptr) {
    return "";
  }
  ALCdevice* device = alcGetContextsDevice(ctx);
  if (device == nullptr) {
    return "";
  }
  String name = alcGetString(device, ALC_DEVICE_SPECIFIER);
  if (name.empty() || alcGetError(device) != AL_NO_ERROR) {
    return device;
  }
  device.name = name;
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

  device = m_currentDevice;

#endif // AUDIO_BACKEND
  return device;
}

void
AudioManager::setCurrentDevice(const String& deviceName) {

#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
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
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

  auto devices = getAvailableDevices();

  auto it = std::find_if(devices.begin(), devices.end(), [&deviceName](const AudioDevice& device) {
    return device.name == deviceName;
    });

  if (it == devices.end()) {
    Logger::instance().logError("Could not find audio device: " + deviceName);
  }
  else {
    m_currentDevice = *it;
  }
#endif // AUDIO_BACKEND
  Logger::instance().log("Switched to audio device: " + deviceName);
}

AudioDevice
AudioManager::getDefaultDevice() {

  AudioDevice defaultDevice;
#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL

  int32 code = 0;
  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_TRUE) {
    if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") != AL_TRUE) {
      Logger::instance().logWarning("Device enumeration not supported.");
      return "";
    }
    code = ALC_DEFAULT_DEVICE_SPECIFIER;
  }
  code = ALC_DEFAULT_ALL_DEVICES_SPECIFIER;

  String defaultDeviceName = alcGetString(nullptr, code);
  if (defaultDeviceName.empty() || alcGetError(nullptr) != AL_NO_ERROR) {
    Logger::instance().logWarning("Could not get default device.");
    return defaultDevice;
  }

  defaultDevice.name = defaultDeviceName;
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

  auto devices = getAvailableDevices();

  auto it = std::find_if(devices.begin(), devices.end(), [](const AudioDevice& device) {
    return device.isDefaultOutput;
    });

  if (it == devices.end()) {
    Logger::instance().logError("Could not find default device: ");
  }
  else {
    defaultDevice = *it;
  }
#endif

  return defaultDevice;
}

bool
AudioManager::ensureIsInit() {
#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  ALCcontext* ctx = alcGetCurrentContext();
  return ctx != nullptr;
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO
  return !m_currentDevice.name.empty();
#endif
}

void
AudioManager::onShutDown() {
# if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
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
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

#endif // AUDIO_BACKEND

  Logger::instance().log("AudioManager shut down.");
}


}
