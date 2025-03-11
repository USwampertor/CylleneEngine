#pragma once

#include "cyCorePrerequisites.h"

#include <cyModule.h>

#include "cyGraphicsBuffer.h"

namespace CYLLENE_SDK {

  namespace GFXTYPE
  {
  enum E
  {
    NONE,
    DX10,
    DX11,
    DX12,
    OGL2,
    OGL3,
    VLKN,
    APL2
  };
  }

class CY_CORE_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{
public:

  GraphicsAPI() = default;

  GraphicsAPI(void* pWindowHandle);

  void*
  createTexture();

  void*
  createShader();

  void*
  createVertexShader();

  void*
  createPixelShader();

  void*
  createInputLayout();

  virtual UniquePointer<GraphicsBuffer>
  createGraphicsBuffer(const Vector<char>& data) = 0;

  virtual UniquePointer<VertexBuffer>
  createVertexBuffer(const Vector<char>& data) = 0;

  virtual UniquePointer<IndexBuffer>
  createIndexBuffer(const Vector<char>& data) = 0;

  virtual UniquePointer<ConstantBuffer>
  createConstantBuffer(const Vector<char>& data) = 0;

  virtual void
  writeToBuffer(const UniquePointer<GraphicsBuffer>& pBuffer, const Vector<char>& data) = 0;

private:

  virtual void
  queryInterface() = 0;

  void*
  m_pWHandle;

  void*
  m_pDevice;

  void*
  m_pDeviceContext;

  void*
  m_pSwapChain;

  void*
  m_pBackBufferRTV;

  void*
  m_pBackBufferDSV;



};

// TODO: Check if this is correct
// extern CY_CORE_EXPORT UniquePointer<GraphicsAPI> g_GraphicsAPI;

}

