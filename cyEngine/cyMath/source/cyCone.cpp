#include "cyCone.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Cone::toString() {
  return Utils::format("Radius: %2.f, Height: %2.f", m_radius, m_height);
}

bool
Cone::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Cone::setDimensions(const float& radius, const float& height) {
    m_radius = radius;
    m_height = height;
  }

bool
Cone::isValid() const {
  return m_radius > Math::EPSILONF && m_height > Math::EPSILONF;
}

float
Cone::getSlantHeight() const {
  return Math::sqrt(Math::sqr(m_radius) + Math::sqr(m_height));
}

float
Cone::getBaseArea() const {
  return Math::PI * Math::sqr(m_radius);
}

float
Cone::getLateralSurfaceArea() const {
  return Math::PI * m_radius * getSlantHeight();
}

float
Cone::getSurfaceArea() const {
  return getBaseArea() + getLateralSurfaceArea();
}

float
Cone::getVolume() const {
  return (getBaseArea() * m_height) / 3.0f;
}


}
