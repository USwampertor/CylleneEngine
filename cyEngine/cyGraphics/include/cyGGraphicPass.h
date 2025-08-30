#pragma once
#include "cyCorePrerequisites.h"
#include "cyGraphicsBuffer.h"
#include "cyGDepthStencilView.h"
#include "cyGGraphic.h"
#include "cyGShader.h"
#include "cyGTexture.h"
#include "cyGRasterizerState.h"
#include "cyGRenderTargetView.h"
#include "cyGSamplerState.h"
#include "cyGBlendState.h"

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

  virtual void* get() override { return this; }
  virtual void set(void*) override { }

  virtual void initialize(WPtr<CCamera> newParentCamera,
                          WPtr<GraphicsPipeline> newParentPipeline) {
    m_parentPipeline = newParentPipeline;
    m_camera = newParentCamera;
    m_parentPipeline.lock()->addPass(makeSharedPtr<GraphicPass>(this));
    setID(m_parentPipeline.lock()->getPasses().size() - 1);
  }
  virtual void clear() = 0;
  virtual void execute() = 0;
  virtual void cleanup() = 0;
  virtual void shutdown() = 0;

  int32 getID() const { return m_passID; }
  void setID(int32 newID) { m_passID = newID; }

  virtual SPtr<GTexture> getOutputTexture() const { return nullptr; }
  virtual SPtr<GDepthStencilView> getOutputDepthStencil() const { return nullptr; }
  virtual SPtr<GraphicsBuffer> getOutputBuffer() const { return nullptr; }
  virtual SPtr<GRenderTargetView> getOutputRenderTarget() const { return nullptr; }

  WPtr<GGraphicPass> m_nextPass;
  // WPtr<GGraphicPass> m_previousPass;

  WPtr<GraphicsPipeline> m_parentPipeline;
  WPtr<CCamera> m_camera;

  Vector<char> m_bufferData;
  SPtr<GraphicsBuffer> m_perPassCB;
  SPtr<GRasterizerState> m_rasterState;

  DefaultPerPassConstantBuffer m_perPassConstants;
  DefaultShaderConstants m_shaderConstants;
  DefaultPerObjectConstantBuffer m_perObjectConstants;

  Map<uint32, WPtr<GGraphicPass>> m_dependencies;

private:
  int32 m_passID = -1;
};

struct DefaultShadowConstantBuffer
{
  Matrix4 shadowView = Matrix4::IDENTITY;
  Matrix4 shadowProjection = Matrix4::IDENTITY;
};

//////////////////////////////////////////////////////////////////////////
// Default Passes
//////////////////////////////////////////////////////////////////////////

/*
 *	@class	GDefaultShadowPass
 *	@brief	A shadow pass that renders the scene from the shadow casting light's point of view
 *
 */
class CY_GRAPHICS_EXPORT GDefaultShadowPass : public GGraphicPass
{
public: 
  GDefaultShadowPass() = default;
  virtual ~GDefaultShadowPass() override;

  virtual void initialize(WPtr<CCamera> newParentCamera, 
                          WPtr<GraphicsPipeline> newParentPipeline) override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  virtual SPtr<GTexture> getOutputTexture() const override { return m_shadowDST; }
  virtual SPtr<GDepthStencilView> getOutputDepthStencil() const { return m_shadowDSV; }
  virtual SPtr<GraphicsBuffer> getOutputBuffer() const { return m_shadowCB; }

  DefaultShadowConstantBuffer getShadowConstants() { return m_shadowConstants; }

  DefaultShadowConstantBuffer m_shadowConstants;
  SPtr<GraphicsBuffer> m_shadowCB;
  SPtr<GVertexShader> m_pVShadowShader;
  SPtr<GTexture> m_shadowDST;
  SPtr<GDepthStencilView> m_shadowDSV;
};

/*
 *	@class	GDefaultGeometryPass
 *	@brief	
 *
 */
class GDefaultGeometryPass : public GGraphicPass
{
public:
  GDefaultGeometryPass() = default;

  virtual ~GDefaultGeometryPass() override;

  virtual void initialize(WPtr<CCamera> newParentCamera, 
                          WPtr<GraphicsPipeline> newParentPipeline) override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  virtual SPtr<GRenderTargetView> getOutputRenderTarget() const override { return m_colorRenderTarget; }
  
  SPtr<GRenderTargetView> getPositionRenderTarget() const { return m_positionRenderTarget; }

  SPtr<GSamplerState> m_pointSampler;
  SPtr<GSamplerState> m_linearSampler;

  SPtr<GVertexShader> m_pVGeometryShader;
  SPtr<GPixelShader> m_pPGeometryShader;

  SPtr<GRenderTargetView> m_colorRenderTarget;
  SPtr<GRenderTargetView> m_positionRenderTarget;
  SPtr<GraphicsBuffer> m_shaderConstantsBuffer;


  float m_time = 0.0f;
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

class CY_GRAPHICS_EXPORT GDefaultParticlesPass : public GGraphicPass
{
public:
  GDefaultParticlesPass() = default;
  virtual ~GDefaultParticlesPass() {}

  virtual void initialize(WPtr<CCamera> newParentCamera, 
                          WPtr<GraphicsPipeline> newParentPipeline) override;
  virtual void clear() override;
  virtual void execute() override;
  virtual void cleanup() override;
  virtual void shutdown() override;

  SPtr<GVertexShader> m_pVParticleShader;
  SPtr<GPixelShader> m_pPParticleShader;

  SPtr<GBlendState> m_defaultBlend;
  SPtr<GBlendState> m_alphaBlend;

  SPtr<GSamplerState> m_pointSampler;
  SPtr<GSamplerState> m_linearSampler;


  SPtr<GraphicsBuffer> m_shaderConstantsBuffer;

  float m_time = 0.0f;

};

class CY_GRAPHICS_EXPORT GDefaultPPPass : public GGraphicPass
{
public:
  GDefaultPPPass() = default;
  virtual ~GDefaultPPPass() {}

  virtual void initialize(WPtr<CCamera> newParentCamera, 
                          WPtr<GraphicsPipeline> newParentPipeline) override;
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
