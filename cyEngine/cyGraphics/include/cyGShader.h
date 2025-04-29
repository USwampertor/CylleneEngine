#pragma once

#include "cyGraphicsPrerequisites.h"

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


class GShader
{
public:
  GShader() = default;
  virtual ~GShader() = default;
  
  virtual bool
  compile() = 0;

  void* blob = nullptr;
  void* shader = nullptr;
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



}