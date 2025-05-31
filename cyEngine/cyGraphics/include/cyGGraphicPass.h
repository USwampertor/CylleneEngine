#pragma once
#include "cyCorePrerequisites.h"
#include "cyGGraphic.h"


namespace CYLLENE_SDK {

class GGraphicPass : public GGraphic
{
public:

  GGraphicPass() = default;
  virtual ~GGraphicPass() {}

  virtual void initialize() = 0;
  virtual void execute() = 0;
  virtual void cleanup() = 0;
  virtual void shutdown() = 0;

  WPtr<GGraphicPass> m_nextPass;
  WPtr<GGraphicPass> m_previousPass;

  int32 m_passID = -1;
};

class GShadowPass : public GGraphicPass
{
public: 
  GShadowPass() = default;
  virtual ~GShadowPass() {}

};

class GGeometryPass : public GGraphicPass
{
public:
  GGeometryPass() = default;
  virtual ~GGeometryPass() {}
};

class GLightingPass : public GGraphicPass
{
public:
  GLightingPass() = default;
  virtual ~GLightingPass() {}
};

class GTransparencyPass : public GGraphicPass
{
public:
  GTransparencyPass() = default;
  virtual ~GTransparencyPass() {}
};

class GPostProcessPass : public GGraphicPass
{
public:
  GPostProcessPass() = default;
  virtual ~GPostProcessPass() {}
};

}
