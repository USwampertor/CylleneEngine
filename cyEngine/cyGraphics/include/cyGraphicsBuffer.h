#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK {

struct GBufferElement
{
  uint32 byteWidth;
  uint32 usage;
  uint32 bindFlags;
  uint32 cpuAccessFlags;
  uint32 miscFlags;
  uint32 structureByteStride;
};

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

