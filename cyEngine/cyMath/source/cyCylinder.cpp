#include "cyCylinder.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Cylinder::toString() {
  return Utils::format("Radius: %2.f, Height: %2.f", m_radius, m_height);
}

bool
Cylinder::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Cylinder::setDimensions(const float& radius, const float& height) {
  m_radius = radius;
  m_height = height;
}

float
Cylinder::getDiameter() const {
  return m_radius * 2.0f;
}

float
Cylinder::getHalfHeight() const {
  return m_height * 0.5f;
}

bool
Cylinder::isValid() const {
  return m_radius > Math::EPSILONF && m_height > Math::EPSILONF;
}

float
Cylinder::getBaseArea() const {
  return Math::PI * Math::sqr(m_radius);
}

float
Cylinder::getLateralSurfaceArea() const {
  return 2.0f * Math::PI * m_radius * m_height;
}

float
Cylinder::getSurfaceArea() const {
  return 2.0f * getBaseArea() + getLateralSurfaceArea();
}

float
Cylinder::getVolume() const {
  return getBaseArea() * m_height;
}

Vector3f
Cylinder::getCenter() const {
  return Vector3f(0.0f, 0.0f, m_height * 0.5f);
}

}
