#pragma once
#include "cyCorePrerequisites.h"

#include <cyModule.h>
#include <cyThread.h>


#include <AL/al.h>
#include <AL/alc.h>

namespace CYLLENE_SDK {

namespace AUDIO_ERROR {
  BETTER_ENUM(E, int32,
              eNONE             = AL_NO_ERROR,
              eINVALIDNAME      = AL_INVALID_NAME,
              eINVALIDENUM      = AL_INVALID_ENUM,
              eINVALIDVALUE     = AL_INVALID_VALUE,
              eINVALIDOPERATION = AL_INVALID_OPERATION,
              eALOUTOFMEMORY    = AL_OUT_OF_MEMORY);
}

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

}
