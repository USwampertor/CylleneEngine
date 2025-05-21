#pragma once
#include "cyGraphicsPrerequisites.h"
#include "cyGGraphic.h"

#include <cyRect.h>

namespace CYLLENE_SDK
{

class CY_GRAPHICS_EXPORT GViewport : public GGraphic
{
public:
  GViewport() = default;
  GViewport(int32 x, int32 y, int32 width, int32 height) {
    m_rect = { static_cast<uint32>(x),
               static_cast<uint32>(y),
               static_cast<uint32>(width),
               static_cast<uint32>(height) };
  }
  
  ~GViewport() = default;

  void
  setViewport(int32 x, int32 y, int32 width, int32 height);

  virtual void
  set(void* data) override {
    m_rect = *static_cast<Rect*>(data);
  }

  virtual void*
  get() override {
    return &m_rect;
  }

public:

  Rect m_rect;
};

}