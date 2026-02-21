
#pragma once

#include "cyCorePrerequisites.h"

// TODO: Make this a DLL instead of macro definitions

#define AUDIO_BACKEND_OPENAL 1
#define AUDIO_BACKEND_RTAUDIO 2

#define AUDIO_BACKEND AUDIO_BACKEND_RTAUDIO


#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

#include <rtaudio/RtAudio.h>
#include <rtaudio/rtaudio_c.h>

#endif

#define AUDIO_PRIORITY_DEFAULT 128

namespace CYLLENE_SDK {

  namespace AUDIO_FORMAT {
  BETTER_ENUM(E, int32,
              eS8   = 0x1,
              eS16  = 0x2,
              eS24  = 0x4,
              eS32  = 0x8,
              eF32  = 0x10,
              eF64  = 0x20);
  }

  
  namespace AUDIO_ERROR {
#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  BETTER_ENUM(E, int32,
              eNONE             = AL_NO_ERROR,
              eINVALIDNAME      = AL_INVALID_NAME,
              eINVALIDENUM      = AL_INVALID_ENUM,
              eINVALIDVALUE     = AL_INVALID_VALUE,
              eINVALIDOPERATION = AL_INVALID_OPERATION,
              eOUTOFMEMORY      = AL_OUT_OF_MEMORY);
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO
  BETTER_ENUM(E, int32,
              eNONE               = rt::audio::RTAUDIO_NO_ERROR,          /*!< No error. */
              eWARNING            = rt::audio::RTAUDIO_WARNING,           /*!< A non-critical error. */
              eUNKNOWN            = rt::audio::RTAUDIO_UNKNOWN_ERROR,     /*!< An unspecified error type. */
              eNODEVICESFOUND     = rt::audio::RTAUDIO_NO_DEVICES_FOUND,  /*!< No devices found on system. */
              eINVALIDDEVICE      = rt::audio::RTAUDIO_INVALID_DEVICE,    /*!< An invalid device ID was specified. */
              eDEVICEDISCONNECTED = rt::audio::RTAUDIO_DEVICE_DISCONNECT, /*!< A device in use was disconnected. */
              eMEMORY             = rt::audio::RTAUDIO_MEMORY_ERROR,      /*!< An error occurred during memory allocation. */
              eINVALIDPARAMETER   = rt::audio::RTAUDIO_INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
              eINVALIDUSE         = rt::audio::RTAUDIO_INVALID_USE,       /*!< The function was called incorrectly. */
              eDRIVER             = rt::audio::RTAUDIO_DRIVER_ERROR,      /*!< A system driver error occurred. */
              eSYSTEM             = rt::audio::RTAUDIO_SYSTEM_ERROR,      /*!< A system error occurred. */
              eTHREAD             = rt::audio::RTAUDIO_THREAD_ERROR       /*!< A thread error occurred. */);
#endif // AUDIO_BACKEND
}

#if AUDIO_BACKEND == AUDIO_BACKEND_OPENAL
  namespace LISTENER_PROPERTIES {
  BETTER_ENUM(E, int32,
              eGAIN         = AL_GAIN,
              ePOSITION     = AL_POSITION,
              eVELOCITY     = AL_VELOCITY,
              eORIENTATION  = AL_ORIENTATION);
  }

  namespace BUFFER_PROPERTIES {
  BETTER_ENUM(E, int32,
              eFREQUENCY  = AL_FREQUENCY,
              eBITS       = AL_BITS,
              eCHANNELS   = AL_CHANNELS,
              eSIZE       = AL_SIZE);
  }

  namespace SOURCE_PROPERTIES {
  BETTER_ENUM(E, int32,
              ePITCH              = AL_PITCH,
              eGAIN               = AL_GAIN,
              eMAXDISTANCE        = AL_MAX_DISTANCE,
              eROLLOFFFACTOR      = AL_ROLLOFF_FACTOR,
              eREFERENCEDISTANCE  = AL_REFERENCE_DISTANCE,
              eMINGAIN            = AL_MIN_GAIN,
              eMAXGAIN            = AL_MAX_GAIN,
              eCONEOUTERGAIN      = AL_CONE_OUTER_GAIN,
              eCONEINNERANGLE     = AL_CONE_INNER_ANGLE,
              eCONEOUTERANGLE     = AL_CONE_OUTER_ANGLE,
              ePOSITION           = AL_POSITION,
              eVELOCITY           = AL_VELOCITY,
              eDIRECTION          = AL_DIRECTION,
              eSOURCERELATIVE     = AL_SOURCE_RELATIVE,
              eSOURCETYPE         = AL_SOURCE_TYPE,
              eLOOPING            = AL_LOOPING,
              eBUFFER             = AL_BUFFER,
              eSOURCESTATE        = AL_SOURCE_STATE,
              eBUFFERSQUEUED      = AL_BUFFERS_QUEUED,
              eBUFFERSPROCESSED   = AL_BUFFERS_PROCESSED,
              eSECOFFSET          = AL_SEC_OFFSET,
              eSAMPLEOFFSET       = AL_SAMPLE_OFFSET,
              eBYTEOFFSET         = AL_BYTE_OFFSET);
  }
#elif AUDIO_BACKEND == AUDIO_BACKEND_RTAUDIO

  namespace AUDIO_STATE {
  BETTER_ENUM(E, int32,
              eSTOPPED  = rt::audio::STREAM_STOPPED,
              eSTOPPING = rt::audio::STREAM_STOPPING,
              eRUNNING  = rt::audio::STREAM_RUNNING,
              eCLOSED   = rt::audio::STREAM_CLOSED);

  }

  namespace AUDIO_MODE {
  BETTER_ENUM(E, int32,
              eOUTPUT         = rt::audio::OUTPUT,
              eINPUT          = rt::audio::INPUT,
              eDUPLEX         = rt::audio::DUPLEX,
              eUNINITIALIZED  = rt::audio::UNINITIALIZED);


  

  }

  using APIAudio = rt::audio::RtAudio;
  using APIAudioDeviceInfo = rt::audio::RtAudio::DeviceInfo;
  using APIAudioParams = rt::audio::RtAudio::StreamParameters;
  using APIAudioFormat = rt::audio::RtAudioFormat;
  using APIAudioStatus = rt::audio::RtAudioStreamStatus;

#endif // AUDIO_BACKEND


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



} // namespace CYLLENE_SDK
