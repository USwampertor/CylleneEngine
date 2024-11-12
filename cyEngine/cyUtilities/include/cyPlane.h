#pragma once

#include "cyUtilitiesPrerequisites.h"
#include "cyVector3f.h"


namespace CYLLENE_SDK
{

class Plane
{
  Plane() = default;

  Plane(const Vector3f& origin, const Vector3f& normal)
    : m_origin(origin),
      m_normal(normal) {}



private:

  Vector3f m_origin;

  Vector3f m_normal;
};


}
