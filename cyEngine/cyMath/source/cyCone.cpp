#include "cyCone.h"

namespace CYLLENE_SDK {

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