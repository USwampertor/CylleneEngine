#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGShader.h>

namespace CYLLENE_SDK {

class GDX11Shader : public GShader
{
public:
  GDX11Shader() = default;
  virtual ~GDX11Shader() {}
  virtual void*
  getBlob() override {

  }
private:
  ID3DBlob* m_pBlob;
};

}

