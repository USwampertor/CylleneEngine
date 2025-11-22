#pragma once
#include "cyDirectX11Prerequisites.h"

#include <cyGraphicsBuffer.h>


namespace CYLLENE_SDK {

class CY_DX11_EXPORT DX11GraphicsBuffer : public GraphicsBuffer
{
public:
  DX11GraphicsBuffer() = default;

  virtual ~DX11GraphicsBuffer() override;

  virtual void*
  get() override;

  virtual void
  set(void* pHandle) override;

  ID3D11Buffer* m_pBuffer = nullptr;

};

}

