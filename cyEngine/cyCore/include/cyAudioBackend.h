
#pragma once

#include "cyCorePrerequisites.h"

#include "cyVector3f.h"
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
#define AUDIO_PRIORITY_MAX 255

#define AUDIO_MAX_CHANNELS 8

#define AUDIO_NUM_BANDS 6

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

  namespace AUDIO_TRACK_STATE {
  BETTER_ENUM(E, int32,
              eSTOPPED  = 0,    // rt::audio::STREAM_STOPPED,
              eSTOPPING = 1,    // rt::audio::STREAM_STOPPING,
              eRUNNING  = 2,    // rt::audio::STREAM_RUNNING,
              eCLOSED   = -50); // rt::audio::STREAM_CLOSED);

  }

  namespace AUDIO_STREAM_MODE {
  BETTER_ENUM(E, int32,
              eOUTPUT         = 0,    // rt::audio::OUTPUT,
              eINPUT          = 1,    // rt::audio::INPUT,
              eDUPLEX         = 2,    // rt::audio::DUPLEX,
              eUNINITIALIZED  = -50); // rt::audio::UNINITIALIZED);

  }

  namespace AUDIO_OS_API {
  BETTER_ENUM(E, int32,
              eUNKNOWN         = 0,   // rt::audio::UNSPECIFIED,    
              eMACOSX          = 1,   // rt::audio::MACOSX_CORE,    
              eLINUX_ALSA      = 2,   // rt::audio::LINUX_ALSA,    
              eUNIX_JACK       = 3,   // rt::audio::UNIX_JACK);    
              eLINUX_PULSE     = 4,   // rt::audio::LINUX_PULSE,   
              eLINUX_OSS       = 5,   // rt::audio::LINUX_OSS,     
              eWINDOWS_ASIO    = 6,   // rt::audio::WINDOWS_ASIO,  
              eWINDOWS_WASAPI  = 7,   // rt::audio::WINDOWS_WASAPI, 
              eWINDOWS_DS      = 8,   // rt::audio::WINDOWS_DS,    
              eDUMMY           = 9);  // rt::audio::RTAUDIO_DUMMY
  }

  using APIAudio            = rt::audio::RtAudio;
  using APIAudioDeviceInfo  = rt::audio::RtAudio::DeviceInfo;
  using APIAudioParams      = rt::audio::RtAudio::StreamParameters;
  using APIAudioFormat      = rt::audio::RtAudioFormat;
  using APIAudioStatus      = rt::audio::RtAudioStreamStatus;

#endif // AUDIO_BACKEND


  int32
  audioFormatToAPIFormat(AUDIO_FORMAT::E format);

struct PathContribution
{
  float delay;                          // seconds
  float attenuation[AUDIO_NUM_BANDS];   // spectral attenuation
  Vector3f direction;                   // for spatialization
};

} // namespace CYLLENE_SDK
