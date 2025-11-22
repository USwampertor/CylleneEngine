#include "cyRAudio.h"


namespace CYLLENE_SDK {

double
RAudio::getDuration(const DELTA_TYPE::E& in) {
  if (m_sampleRate == 0 || m_channels == 0) {
    return 0.0;
  }
  double totalSeconds = static_cast<double>(m_samples.size()) / (m_sampleRate * m_channels);
  switch (in) {
  case DELTA_TYPE::E::eSECOND:
    return totalSeconds;
  case DELTA_TYPE::E::eMILLISECOND:
    return totalSeconds * 1000.0;
  case DELTA_TYPE::E::eMICROSECOND:
    return totalSeconds * 1000000.0;
  default:
    return totalSeconds;
  }
}

void*
RAudio::getData() {
  AudioData* data = new AudioData();
  data->m_sampleRate = m_sampleRate;
  data->m_channels = m_channels;
  data->m_samples = m_samples;
  return reinterpret_cast<void*>(data);
}

void
RAudio::setData(void* data) {
  if (!data) return;
  AudioData* audioData = reinterpret_cast<AudioData*>(data);
  m_sampleRate = audioData->m_sampleRate;
  m_channels = audioData->m_channels;
  m_samples = audioData->m_samples;
  delete audioData;
}

}
