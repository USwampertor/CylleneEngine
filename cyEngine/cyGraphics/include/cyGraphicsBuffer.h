#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK {

struct GBufferElement
{
  Vector<char> data;
  uint32 byteSize;
  uint32 usage;
  uint32 bindFlags;
  uint32 cpuAccessFlags;
  uint32 miscFlags;
  uint32 structureByteStride;
};

class CY_GRAPHICS_EXPORT GraphicsBuffer : public GGraphic
{
public:

  GraphicsBuffer() = default;

  virtual ~GraphicsBuffer() {}

  friend class GraphicsAPI;

public:

  // void* m_pBuffer = nullptr;
  
};

}

