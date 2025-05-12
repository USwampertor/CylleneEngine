#pragma once
#include "cyMathPrerequisites.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

/*
 *	@class Rect
 *	@brief A rectangle primitive.
 *  
 */
class CY_MATH_EXPORT Rect : public Primitive
{
public:
  Rect() : Primitive(Rect::staticType()) {}

  Rect(const Rect& other)
    : Primitive(Rect::staticType()),
      x(other.x),
      y(other.y),
      width(other.width),
      height(other.height) {}

  Rect(uint32 nx, uint32 ny, uint32 nwidth, uint32 nheight)
    : Primitive(Rect::staticType()),
      x(nx),
      y(ny),
      width(nwidth),
      height(nheight) {}

  static PRIMITIVE_TYPE::E 
  staticType() {
    return PRIMITIVE_TYPE::E::OBB;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

public:
  
  uint32 x;
  uint32 y;
  uint32 width;
  uint32 height;

};

}
