#pragma once
#include "cyCorePrerequisites.h"
#include "cyGGraphic.h"


namespace CYLLENE_SDK {

class GGraphicPass : public GGraphic
{
public:

  GGraphicPass() = default;
  virtual ~GGraphicPass() {}

  int32 m_layerID = -1;
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

}
