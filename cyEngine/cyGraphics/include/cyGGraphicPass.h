#pragma once
#include "cyCorePrerequisites.h"
#include "cyGraphicsBuffer.h"
#include "cyGDepthStencilView.h"
#include "cyGGraphic.h"
#include "cyGShader.h"
#include "cyGTexture.h"
#include "cyGRasterizerState.h"

#include <cyGraphicsPipeline.h>

#include <cyMatrix4.h>

namespace CYLLENE_SDK {

class CCamera;

struct DefaultShaderConstants
{
  float time;
  float align1;
  float align2;
  float align3;
};

struct DefaultPerObjectConstantBuffer
{
  Matrix4 world = Matrix4::IDENTITY;
};

struct DefaultPerPassConstantBuffer
{
  Vector4f cameraForward = Vector4f::ZERO;
  Vector4f cameraRight = Vector4f::ZERO;
  Vector4f cameraUp = Vector4f::ZERO;
  Matrix4 view = Matrix4::IDENTITY;
  Matrix4 projection = Matrix4::IDENTITY;
};



class CY_GRAPHICS_EXPORT GGraphicPass : public GGraphic, public GraphicPass
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

  WPtr<GraphicsPipeline> m_parentPipeline;
  WPtr<CCamera> m_camera;

  Vector<char> m_bufferData;
  SPtr<GraphicsBuffer> m_perPassCB;
  SPtr<GRasterizerState> m_rasterState;

  DefaultPerPassConstantBuffer m_perPassConstants;
  DefaultShaderConstants m_shaderConstants;
  DefaultPerObjectConstantBuffer m_perObjectConstants;

  int32 m_passID = -1;
};

struct DefaultShadowConstantBuffer
{
  Matrix4 shadowView = Matrix4::IDENTITY;
  Matrix4 shadowProjection = Matrix4::IDENTITY;
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

  virtual void* get() override { return m_shadowDST->get(); }
  virtual void set(void*) override {}

  DefaultShadowConstantBuffer m_shadowConstants;
  SPtr<GraphicsBuffer> m_shadowCB;
  SPtr<GVertexShader> m_pVShadowShader;
  SPtr<GTexture> m_shadowDST;
  SPtr<GDepthStencilView> m_shadowDSV;
};

class GGeometryPass : public GGraphicPass
{
public:
  GGeometryPass() = default;

  virtual ~GGeometryPass() override;

  virtual void initialize() override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  virtual void* get() override { return m_colorRenderTarget->get(); }
  virtual void set(void*) override {}

  SPtr<GSamplerState> m_pointSampler;
  SPtr<GSamplerState> m_linearSampler;

  SPtr<GVertexShader> m_pVGeometryShader;
  SPtr<GPixelShader> m_pPGeometryShader;

  SPtr<GRenderTargetView> m_colorRenderTarget;
  SPtr<GRenderTargetView> m_positionRenderTarget;
  SPtr<GraphicsBuffer> m_shaderConstantsBuffer;

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

class CY_GRAPHICS_EXPORT GParticlesPass : public GGraphicPass
{
public:
  GParticlesPass() = default;
  virtual ~GParticlesPass() {}

  virtual void initialize() override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  virtual void* get() override {  }
  virtual void set(void*) override {}

  SPtr<GVertexShader> m_pVParticleShader;
  SPtr<GPixelShader> m_pPParticleShader;

  SPtr<GBlendState> m_defaultBlend;
  SPtr<GBlendState> m_alphaBlend;

  SPtr<GSamplerState> m_pointSampler;
  SPtr<GSamplerState> m_linearSampler;

};

class CY_GRAPHICS_EXPORT GPostProcessPass : public GGraphicPass
{
public:
  GPostProcessPass() = default;
  virtual ~GPostProcessPass() {}

  virtual void initialize() override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  virtual void* get() override {}
  virtual void set(void*) override {}

  SPtr<GVertexShader> m_pVSAQShader;
  SPtr<GPixelShader> m_pPSAQShader;

  SPtr<GSamplerState> m_pointSampler;
  SPtr<GSamplerState> m_linearSampler;

};

}
