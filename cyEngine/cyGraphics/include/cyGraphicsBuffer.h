#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK {

class GraphicsBuffer : public Graphic
{
public:

  GraphicsBuffer() = default;

  virtual ~GraphicsBuffer();
  
  virtual void
  setData(void* data) = 0;

  friend class GraphicsAPI;

public:

  // void* m_pBuffer = nullptr;
  
};

}

