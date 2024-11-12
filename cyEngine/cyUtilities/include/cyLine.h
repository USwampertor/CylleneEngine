#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyVector4f.h"

namespace CYLLENE_SDK
{

/*
 *	@class  Line 
 *	@brief  Defines two positions in space bound to each other
 *
 */
class Line
{
  /*
   *	@brief  Default constructor
   */
  Line() = default;

  /*
   *	@brief  Constructor taking two vectors
   *	@param  const Point& a - Point a
   *	@param  const Point& b - Point b
   */
  Line(const Point& a, const Point& b)
    : m_a(a),
      m_b(b) {}


private:

  /**
   * 
   */
  Point m_a;

  Point m_b;
};

}


