#pragma once
#include "cyCorePrerequisites.h"
#include "cyGraphicsBuffer.h"
#include "cyGDepthStencilView.h"
#include "cyGGraphic.h"
#include "cyGShader.h"
#include "cyGTexture.h"


#include <cyMatrix4.h>

namespace CYLLENE_SDK {

class CY_GRAPHICS_EXPORT GGraphicPass : public GGraphic
{
public:

  GGraphicPass() = default;
  virtual ~GGraphicPass() {}

  virtual void initialize() = 0;
  virtual void clear() = 0;
  virtual void execute() = 0;
  virtual void cleanup() = 0;
  virtual void shutdown() = 0;

  WPtr<GGraphicPass> m_nextPass;
  WPtr<GGraphicPass> m_previousPass;

  int32 m_passID = -1;
};

struct ShadowConstantBuffer
{
  Matrix4 shadowView;
  Matrix4 shadowProjection;
};

class CY_GRAPHICS_EXPORT GShadowPass : public GGraphicPass
{
public: 
  GShadowPass() = default;
  virtual ~GShadowPass() override;

  virtual void initialize() override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  ShadowConstantBuffer m_shadowConstants;
  SPtr<GraphicsBuffer> m_shadowCB;
  SPtr<GVertexShader> m_pVShadowShader;
  SPtr<GTexture> m_shadowDST;
  SPtr<GDepthStencilView> m_shadowDSV;
};

class GGeometryPass : public GGraphicPass
{
public:
  GGeometryPass() = default;
  virtual ~GGeometryPass() {}
};

class CY_GRAPHICS_EXPORT GLightingPass : public GGraphicPass
{
public:
  GLightingPass() = default;
  virtual ~GLightingPass() {}
};

class CY_GRAPHICS_EXPORT GTransparencyPass : public GGraphicPass
{
public:
  GTransparencyPass() = default;
  virtual ~GTransparencyPass() {}
};

class CY_GRAPHICS_EXPORT GPostProcessPass : public GGraphicPass
{
public:
  GPostProcessPass() = default;
  virtual ~GPostProcessPass() {}
};

}
