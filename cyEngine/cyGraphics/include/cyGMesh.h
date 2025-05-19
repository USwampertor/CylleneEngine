#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphicsBuffer.h"
#include "cyGraphic.h"

#include <cyVertex.h>


namespace CYLLENE_SDK
{

class CY_GRAPHICS_EXPORT GMesh : public Graphic
{
public:

  GMesh() = default;

  ~GMesh() = default;


  virtual void*
  get() override {
    return nullptr;
  }

  virtual void
  set(void* data) override {

  }

//   virtual void
//   setVertexBuffer(Vector<Vertex> vertexBuffer) = 0;
//   
//   virtual void
//   setIndexBuffer(Vector<uint32> indexBuffer) = 0;
//   virtual void
//   setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology) = 0;

  PRIMITIVE_TOPOLOGY::E m_topology;
  int m_baseVertex;
  int m_numVertices;
  int m_baseIndex;
  int m_numIndices;
  SPtr<GraphicsBuffer> m_pVertexBuffer;
  SPtr<GraphicsBuffer> m_pIndexBuffer;
};


}