#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

#include <cyVertex.h>

namespace CYLLENE_SDK
{

class CY_GRAPHICS_EXPORT GMesh : public Graphic
{
public:
  GMesh() = default;

  virtual ~GMesh() = default;
  virtual void
  setVertexBuffer(Vector<Vertex> vertexBuffer) = 0;
  virtual void
  setIndexBuffer(Vector<uint32> indexBuffer) = 0;
  virtual void
  setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology) = 0;

  Vector<uint32> m_indexBuffer;
};


}