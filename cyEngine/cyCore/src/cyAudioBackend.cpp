#include "cyAudioBackend.h"

namespace CYLLENE_SDK {

int32
audioFormatToAPIFormat(AUDIO_FORMAT::E format) {

#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  switch (format) {
  case AUDIO_FORMAT::E::eS8:
    return AL_FORMAT_MONO8;
  case AUDIO_FORMAT::E::eS16:
    return AL_FORMAT_MONO16;
  case AUDIO_FORMAT::E::eS24:
    // OpenAL does not support 24-bit directly; usually packed into 32-bit
    return AL_FORMAT_MONO_FLOAT32;
  case AUDIO_FORMAT::E::eS32:
    return AL_FORMAT_MONO_FLOAT32;
  case AUDIO_FORMAT::E::eF32:
    return AL_FORMAT_MONO_FLOAT32;
  case AUDIO_FORMAT::E::eF64:
    // OpenAL does not support 64-bit float directly
    return AL_FORMAT_MONO_FLOAT32;
  default:
    return AL_FORMAT_MONO16;
  }
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO
  switch (format) {
  case AUDIO_FORMAT::E::eS8:
    return RTAUDIO_SINT8;
  case AUDIO_FORMAT::E::eS16:
    return RTAUDIO_SINT16;
  case AUDIO_FORMAT::E::eS24:
    return RTAUDIO_SINT24;
  case AUDIO_FORMAT::E::eS32:
    return RTAUDIO_SINT32;
  case AUDIO_FORMAT::E::eF32:
    return RTAUDIO_FLOAT32;
  case AUDIO_FORMAT::E::eF64:
    return RTAUDIO_FLOAT64;
  default:
    return RTAUDIO_SINT16;
  }
#endif
}

}