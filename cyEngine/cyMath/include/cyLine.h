#pragma once

#include "cyMathPrerequisites.h"
#include "cyVector3f.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK
{

/*
 *	@class  Line 
 *	@brief  Defines two positions in space bound to each other
 *
 */
class CY_MATH_EXPORT Line : public Primitive
{
  /*
   *	@brief  Default constructor
   */
  Line() 
    : Primitive(PRIMITIVE_TYPE::E::LINE),
      m_a(Vector3f::ZERO),
      m_b(Vector3f::ZERO) {}

  /*
   *	@brief  Constructor taking two vectors
   *	@param  const Point& a - Point a
   *	@param  const Point& b - Point b
   */
  Line(const Vector3f& a, const Vector3f& b)
    : Primitive(PRIMITIVE_TYPE::E::LINE),
      m_a(a),
      m_b(b) {}


  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

private:

  /**
   * 
   */
  Vector3f m_a;

  Vector3f m_b;
};

}


