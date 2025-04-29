#pragma once
#include "cyCorePrerequisites.h"
#include "cyVertex.h"

namespace CYLLENE_SDK {

class GraphicsBuffer
{
  GraphicsBuffer() = default;

  virtual ~GraphicsBuffer();

  // friend class GraphicsAPI;

public:
  void* m_buffer;
};

using VertexBuffer = GraphicsBuffer;
using IndexBuffer = GraphicsBuffer;
using ConstantBuffer = GraphicsBuffer;
}

