#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK {

struct GShaderResourceViewElement
{
  void* resource;
  uint32 format;
  uint32 viewDimension;
  uint32 mipLevels;
  uint32 firstMipLevel;
  uint32 firstArraySlice;
  uint32 arraySize;
  uint32 minLOD;
  uint32 maxLOD;
  uint32 flags;
};


class GShaderResourceView : public Graphic
{
public:
  GShaderResourceView() = default;
  virtual ~GShaderResourceView();
};

}

