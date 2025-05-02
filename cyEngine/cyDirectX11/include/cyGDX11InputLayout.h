#pragma once
#include "cyDirectX11Prerequisites.h"
#include <cyGInputLayout.h>

namespace CYLLENE_SDK
{

class GDX11InputLayout : public GInputLayout
{
public:
  GDX11InputLayout() = default;
  virtual ~GDX11InputLayout() override;

public:

  ID3D11InputLayout* m_pInputLayout = nullptr;

};

}
