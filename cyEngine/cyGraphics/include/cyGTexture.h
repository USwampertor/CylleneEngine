#pragma once

#include "cyGraphicsPrerequisites.h"
#include "cyGraphic.h"

namespace CYLLENE_SDK
{

class GTexture : public Graphic
{
public:
  GTexture() = default;
  virtual ~GTexture() = default;
  
  virtual void bind() = 0;
  virtual void* getBlob() = 0;
  virtual void* getResource() = 0;

};


}