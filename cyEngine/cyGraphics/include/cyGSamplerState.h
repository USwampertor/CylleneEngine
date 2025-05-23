#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"
#include "cyRImage.h"
#include "cyRTexture.h"

namespace CYLLENE_SDK
{

  namespace SAMPLERCOMPARISON
  {
    BETTER_ENUM(E, uint32, 
                NEVER = 1, 
                LESS = 2, 
                EQUAL = 3, 
                LESSEQUAL = 4, 
                GREATER = 5, 
                NOTEQUAL = 6, 
                GEATEREQUAL = 7, 
                ALWAYS = 8); 
  }

struct GSamplerStateElement
{
  uint32 filter = SAMPLERFILTER::E::eLINEAR;
  uint32 addressU = TEXTUREMODE::E::eCLAMP;
  uint32 addressV = TEXTUREMODE::E::eCLAMP;
  uint32 addressW = TEXTUREMODE::E::eCLAMP;
  float mipLODBias = 0;
  uint32 maxAnisotropy = 1;
  uint32 comparisonFunc = SAMPLERCOMPARISON::E::NEVER;
  float borderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  float minLOD = std::numeric_limits<float>::min();
  float maxLOD = std::numeric_limits<float>::max();
};

class CY_GRAPHICS_EXPORT GSamplerState : public GGraphic
{
public:
  GSamplerState() = default;
  virtual ~GSamplerState() {}
};

}
