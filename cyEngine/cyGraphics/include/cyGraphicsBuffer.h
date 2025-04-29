#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK {

class GraphicsBuffer
{
public:

  GraphicsBuffer() = default;

  virtual ~GraphicsBuffer();
  
  friend class GraphicsAPI;

public:

  void* m_pBuffer = nullptr;
  
};

}

