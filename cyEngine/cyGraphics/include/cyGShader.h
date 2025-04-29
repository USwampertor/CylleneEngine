#pragma once

#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{

class GShader
{
public:
  GShader() = default;
  virtual ~GShader() = default;
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