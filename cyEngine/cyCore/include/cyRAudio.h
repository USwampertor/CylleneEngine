#pragma once

// TODO: Move this into something that can be called as a DLL

#include "cyCorePrerequisites.h"
#include "cyRResource.h"

#include <cyTime.h>

namespace CYLLENE_SDK {

struct AudioData
{
  int32 
  m_sampleRate = 0;
  
  int32 
  m_channels = 0;
  
  uint64 
  m_totalFrames = 0;
  
  Vector<float> 
  m_samples;

};

class CY_CORE_EXPORT RAudio : public RResource {

public:

  RAudio() : RResource(RAudio::staticType()) {}

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eAUDIO;
  }

  double
  getDuration(const DELTA_TYPE::E& in);

  virtual void*
  getData() override;

  virtual void
  setData(void* data) override;

  const int32&
  getSampleRate() const { return m_sampleRate; }

  const int32&
  getChannels() const { return m_channels; }

  const Vector<float>&
  getSamples() const { return m_samples; }

  const uint64&
  getTotalFrames() const { return m_totalFrames; }

private:

  int32
  m_sampleRate = 0;

  int32
  m_channels = 0;

  Vector<float> 
  m_samples;

  uint64 
  m_totalFrames;



};
}

