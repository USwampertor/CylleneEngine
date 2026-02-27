#include "cyFrustum.h"
#include "cyIntersections.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Frustum::toString() {
  return Utils::format("Near: %2.f, Far: %2.f, FovY: %2.f, Aspect: %2.f",
                       m_nearPlane,
                       m_farPlane,
                       m_fovYDegrees,
                       m_aspectRatio);
}

bool
Frustum::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

void
Frustum::setDimensions(const float& nearPlane,
                       const float& farPlane,
                       const float& fovYDegrees,
  const float& aspectRatio) {
  m_nearPlane = nearPlane;
  m_farPlane = farPlane;
  m_fovYDegrees = fovYDegrees;
  m_aspectRatio = aspectRatio;
}

bool
Frustum::isValid() const {
  return m_nearPlane > Math::EPSILONF &&
    m_farPlane > m_nearPlane &&
    m_fovYDegrees > Math::EPSILONF &&
    m_fovYDegrees < 179.0f &&
    m_aspectRatio > Math::EPSILONF;
}

float
Frustum::getDepth() const {
  return m_farPlane - m_nearPlane;
}

float
Frustum::getHalfFovYRadians() const {
  return Math::degToRad(m_fovYDegrees * 0.5f);
}

float
Frustum::getNearHalfHeight() const {
  return m_nearPlane * Math::tan(getHalfFovYRadians());
}

float
Frustum::getNearHalfWidth() const {
  return getNearHalfHeight() * m_aspectRatio;
}

float
Frustum::getFarHalfHeight() const {
  return m_farPlane * Math::tan(getHalfFovYRadians());
}

float
Frustum::getFarHalfWidth() const {
  return getFarHalfHeight() * m_aspectRatio;
}

float
Frustum::getNearWidth() const {
  return getNearHalfWidth() * 2.0f;
}

float
Frustum::getNearHeight() const {
  return getNearHalfHeight() * 2.0f;
}

float
Frustum::getFarWidth() const {
  return getFarHalfWidth() * 2.0f;
}

float
Frustum::getFarHeight() const {
  return getFarHalfHeight() * 2.0f;
}

float
Frustum::getNearArea() const {
  return getNearWidth() * getNearHeight();
}

float
Frustum::getFarArea() const {
  return getFarWidth() * getFarHeight();
}

float
Frustum::getVolume() const {
  const float h = getDepth();
  const float a0 = getNearArea();
  const float a1 = getFarArea();
  return (h / 3.0f) * (a0 + a1 + Math::sqrt(a0 * a1));
}

Vector3f
Frustum::getCenter() const {
  const float zCenter = (m_nearPlane + m_farPlane) * 0.5f;
  return Vector3f(0.0f, 0.0f, zCenter);
}

}
