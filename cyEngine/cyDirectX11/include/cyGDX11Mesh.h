#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGMesh.h>


namespace CYLLENE_SDK
{

class CY_DX11_EXPORT GDX11Mesh : public GMesh
{
public:
  GDX11Mesh() = default;
  
  virtual ~GDX11Mesh() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  virtual void
  setVertexBuffer(Vector<Vertex> vertexBuffer) override;
  virtual void
  setIndexBuffer(Vector<uint32> indexBuffer) override;
  virtual void
  setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology) override;
};

}