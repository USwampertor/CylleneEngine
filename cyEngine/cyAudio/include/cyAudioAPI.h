#pragma once
#include <cyCorePrerequisites.h>
#include <cyModule.h>

namespace CYLLENE_SDK {

class AudioAPI : public Module<AudioAPI>
{
public:

  AudioAPI() = default;

  virtual ~AudioAPI() {};
  
  virtual bool 
  initialize(void* pHandle) = 0;
  
  virtual void 
  shutdown() = 0;

  void
  update();



};

}