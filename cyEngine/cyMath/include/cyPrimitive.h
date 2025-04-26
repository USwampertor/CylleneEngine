#pragma once
#include "cyMathPrerequisites.h"

#include "cyVector4f.h"

namespace CYLLENE_SDK
{

  namespace PRIMITIVE_TYPE
  {
  BETTER_ENUM(E, 
              uint32,
              NONE = 0, 
              AABB,
              CAPSULE,
              CONE,
              CYLINDER,
              FRUSTUM,
              LINE,
              OBB,
              PLANE,
              POINT,
              RAY,
              RECTANGLE,  
              SPHERE);
  }


class CY_MATH_EXPORT Primitive
{
public:


  Primitive() = default;

  Primitive(const PRIMITIVE_TYPE::E& type) :m_type(type) {}

  virtual ~Primitive() = default;

  virtual bool
  intersects(const Primitive& other) = 0;

  const PRIMITIVE_TYPE::E getType() const { return m_type; }

  // Implement this per primitive
  virtual PRIMITIVE_TYPE::E staticType() {
    CY_ASSERT(true && "IMPLEMENT THIS");
    return PRIMITIVE_TYPE::E::NONE;
  }

  PRIMITIVE_TYPE::E m_type = PRIMITIVE_TYPE::E::NONE;

  
  virtual String
  toString() = 0;
};

}
