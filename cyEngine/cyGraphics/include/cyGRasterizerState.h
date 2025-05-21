#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{

struct GRasterizerElement
{

};

class CY_GRAPHICS_EXPORT GRasterizerState : public GGraphic
{
public:
  GRasterizerState() = default;
  virtual ~GRasterizerState() {}
};

}

