#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGMesh.h>


namespace CYLLENE_SDK
{

class GDX11Mesh : GMesh
{
  GDX11Mesh() = default;
  virtual ~GDX11Mesh() override;


  virtual void
  setVertexBuffer(Vector<Vertex> vertexBuffer) = 0;
  virtual void
  setIndexBuffer(Vector<uint32> indexBuffer) = 0;
  virtual void
  setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology) = 0;
};

}