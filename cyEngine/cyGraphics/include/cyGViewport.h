#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK
{

class GViewport : public Graphic
{
public:
  GViewport() = default;
  GViewport(int32 x, int32 y, int32 width, int32 height)
    : m_x(x), 
      m_y(y), 
      m_width(width), 
      m_height(height) {}
  
  ~GViewport() = default;

  void
  setViewport(int32 x, int32 y, int32 width, int32 height);

public:

  int32 m_x = 0;
  int32 m_y = 0;
  int32 m_width = 0;
  int32 m_height = 0;
};

}