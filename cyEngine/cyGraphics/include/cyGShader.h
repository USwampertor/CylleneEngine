#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{

  namespace GSHADERTYPE
  {
  BETTER_ENUM(E, uint32,
              eUNKNOWN  = -1,
              eNONE     = 0,
              eVERTEX   = 1,
              ePIXEL    = 2,
              eGEOMETRY = 3,
              eCOMPUTE  = 4
  );
  }

struct GShaderBlob
{
public:
  
  /*
   * The idea is that the DX11 Shaderblob has a D3D11Blob 
   */
  virtual void*
  getBlob() = 0;

  uint32 size;
  uint32 type;
  String entry;
  String model;
  String data;
  String error;
  bool isCompiled = false;
};

class CY_GRAPHICS_EXPORT GShader : public GGraphic
{
public:
  GShader() = default;

  virtual ~GShader() {}
  
  virtual void*
  getBlob() = 0;

  virtual void*
  getShader() = 0;

};


class GVertexShader : public GShader
{
public:
  GVertexShader() = default;
  virtual ~GVertexShader() = default;
};

class GPixelShader : public GShader
{
public:
  GPixelShader() = default;
  virtual ~GPixelShader() = default;
};

class GGeometryShader : public GShader
{
public:
  GGeometryShader() = default;
  virtual ~GGeometryShader() = default;
};

class GComputeShader : public GShader
{
public:
  GComputeShader() = default;
  virtual ~GComputeShader() = default;
};

}