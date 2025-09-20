#pragma once
#include "cyMathPrerequisites.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"
#include "cyVector2f.h"

namespace CYLLENE_SDK
{

struct HitInfo
{
  bool hit = false;
  float distance = std::numeric_limits<float>::infinity();

  uint32 triangleIndex = std::numeric_limits<uint32>::max();

  Vector2f uv = Vector2f::ZERO;
  Vector3f position = Vector3f::ZERO;
  Vector3f normal = Vector3f::ZERO;
};

class Ray : public Primitive
{
  Ray()
    : Primitive(Ray::staticType()),
      m_origin(Vector3f::ZERO),
      m_direction(Vector3f::FRONT) {}

  Ray(const Ray& other)
    : Primitive(Ray::staticType()),
      m_origin(other.m_origin),
      m_direction(other.m_direction) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::RAY;
  }

  virtual String
  toString() override;



public:

  Vector3f m_origin;
  Vector3f m_direction;
};

}

