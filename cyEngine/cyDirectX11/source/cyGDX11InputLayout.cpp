#include "cyGDX11InputLayout.h"

namespace CYLLENE_SDK
{
  GDX11InputLayout::~GDX11InputLayout() {
    DX11_SAFE_RELEASE(m_pInputLayout);
  }
}
