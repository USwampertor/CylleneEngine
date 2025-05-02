#include "cyGDX11Shader.h"

namespace CYLLENE_SDK
{

GDX11VertexShader::~GDX11VertexShader()
{
  DX11_SAFE_RELEASE(m_pVertexShader);
}


GDX11PixelShader::~GDX11PixelShader()
{
  DX11_SAFE_RELEASE(m_pPixelShader);
}

}
