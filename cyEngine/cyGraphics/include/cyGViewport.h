#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

#include <cyRect.h>

namespace CYLLENE_SDK
{

class GViewport : public Graphic
{
public:
  GViewport() = default;
  GViewport(int32 x, int32 y, int32 width, int32 height)
    : {
      m_rect = { x,y,width, height };
    }
  
  ~GViewport() = default;

  void
  setViewport(int32 x, int32 y, int32 width, int32 height);

public:

  Rect m_rect;
};

}