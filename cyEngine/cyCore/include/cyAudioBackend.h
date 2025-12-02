#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <cyLogger.h>

namespace CYLLENE_SDK {
namespace AudioBackend {

struct OpenALContext {
  OpenALContext() {
    ALCcontext* existing = alcGetCurrentContext();
    if (existing) {
      m_context = existing;
      m_device = alcGetContextsDevice(existing);
      m_ownsContext = false;
      return;
    }

    m_device = alcOpenDevice(nullptr);
    if (!m_device) {
      return;
    }

    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context) {
      alcCloseDevice(m_device);
      m_device = nullptr;
      return;
    }

    if (!alcMakeContextCurrent(m_context)) {
      alcDestroyContext(m_context);
      alcCloseDevice(m_device);
      m_context = nullptr;
      m_device = nullptr;
      return;
    }

    m_ownsContext = true;
  }

  ~OpenALContext() {
    if (m_ownsContext && m_context) {
      alcMakeContextCurrent(nullptr);
      alcDestroyContext(m_context);
    }
    if (m_ownsContext && m_device) {
      alcCloseDevice(m_device);
    }
  }

  bool
  isValid() const {
    return (m_device && m_context) || alcGetCurrentContext() != nullptr;
  }

  ALCdevice* m_device = nullptr;
  ALCcontext* m_context = nullptr;
  bool m_ownsContext = false;
};

inline OpenALContext&
getContext() {
  static OpenALContext context;
  return context;
}

inline bool
ensureOpenAL() {
  static bool hasLogged = false;
  OpenALContext& ctx = getContext();
  if (!ctx.isValid()) {
    if (!hasLogged) {
      Logger::instance().logError("OpenAL-soft: Failed to initialize audio device or context");
      hasLogged = true;
    }
    return false;
  }
  return true;
}

} // namespace AudioBackend
} // namespace CYLLENE_SDK
