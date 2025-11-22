#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

namespace CYLLENE_SDK
{
  namespace RASTERIZER_FILLMODE
  {
    BETTER_ENUM(E, uint32, WIREFRAME = 2, SOLID = 3);
  }

  namespace RASTERIZER_CULLMODE
  {
    BETTER_ENUM(E, uint32, NONE = 1, FRONT = 2, BACK = 3);
  }


struct GRasterizerElement
{
  uint32 fillMode = RASTERIZER_FILLMODE::E::SOLID;
  uint32 cullMode = RASTERIZER_CULLMODE::E::BACK;
  bool counterWise = false;
  int32 depthBias = 0;
  float depthBiasClamp = 0.0f;
  float slopScaledDepthBias = 0.0f;
  bool clipEnabled = true;
  bool scissorEnable = false;
  bool multisampleEnable = false;
  bool antialiasedLineEnable = false;
  uint32 forcedSampleCount = 0;
};

class CY_GRAPHICS_EXPORT GRasterizerState : public GGraphic
{
public:
  GRasterizerState() = default;
  virtual ~GRasterizerState() {}
};

}

