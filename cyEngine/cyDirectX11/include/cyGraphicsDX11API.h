#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGraphicsAPI.h>

namespace CYLLENE_SDK {

class CY_DX11_EXPORT GraphicsDX11API : public GraphicsAPI
{
public:
  GraphicsDX11API() = default;
  ~GraphicsDX11API() {}
  virtual void
  initialize(void* pHandle) override;
};

}

