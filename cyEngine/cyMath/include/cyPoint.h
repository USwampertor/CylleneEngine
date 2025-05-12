#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector3f.h"

#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

class CY_MATH_EXPORT Point : public Primitive, public Vector3f
{
public:


  Point() : Primitive(Point::staticType()) {}
  
  Point(float nx, float ny, float nz) : 
    Primitive(Point::staticType()), 
    Vector3f(nx, ny, nz) {}

  static PRIMITIVE_TYPE::E 
  staticType() { return PRIMITIVE_TYPE::E::POINT; }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

};

}

