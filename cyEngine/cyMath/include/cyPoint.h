#pragma once
#include "cyMathPrerequisites.h"
#include "cyVector3f.h"

#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

class Point : public Primitive, public Vector3f
{
public:

  Point() : Primitive(Point::staticType()) {}

  static PRIMITIVE_TYPE::E 
  staticType() { return PRIMITIVE_TYPE::E::POINT; }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

};

}

