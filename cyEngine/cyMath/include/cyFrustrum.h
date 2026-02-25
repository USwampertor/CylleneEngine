#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT Frustrum : public Primitive
{
public:

  Frustrum()
    : Primitive(Frustrum::staticType()),
      m_nearPlane(0.1f),
      m_farPlane(1000.0f),
      m_fovYDegrees(60.0f),
      m_aspectRatio(16.0f / 9.0f) {}

  Frustrum(const float& nearPlane,
           const float& farPlane,
           const float& fovYDegrees,
           const float& aspectRatio)
    : Primitive(Frustrum::staticType()),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_fovYDegrees(fovYDegrees),
      m_aspectRatio(aspectRatio) {}

  Frustrum(const Frustrum& other)
    : Primitive(Frustrum::staticType()),
      m_nearPlane(other.m_nearPlane),
      m_farPlane(other.m_farPlane),
      m_fovYDegrees(other.m_fovYDegrees),
      m_aspectRatio(other.m_aspectRatio) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::FRUSTUM;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Set perspective frustum parameters.
   */
  void
  setPerspective(const float& nearPlane,
                 const float& farPlane,
                 const float& fovYDegrees,
                 const float& aspectRatio) {
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_fovYDegrees = fovYDegrees;
    m_aspectRatio = aspectRatio;
  }

  void
  setNearPlane(const float& nearPlane) {
    m_nearPlane = nearPlane;
  }

  void
  setFarPlane(const float& farPlane) {
    m_farPlane = farPlane;
  }

  void
  setFovYDegrees(const float& fovYDegrees) {
    m_fovYDegrees = fovYDegrees;
  }

  void
  setAspectRatio(const float& aspectRatio) {
    m_aspectRatio = aspectRatio;
  }

  float
  getNearPlane() const {
    return m_nearPlane;
  }

  float
  getFarPlane() const {
    return m_farPlane;
  }

  float
  getFovYDegrees() const {
    return m_fovYDegrees;
  }

  float
  getAspectRatio() const {
    return m_aspectRatio;
  }

  /**
   * @brief True when near/far/fov/aspect are physically valid.
   */
  bool
  isValid() const {
    return m_nearPlane > Math::EPSILONF &&
           m_farPlane > m_nearPlane &&
           m_fovYDegrees > Math::EPSILONF &&
           m_fovYDegrees < 179.0f &&
           m_aspectRatio > Math::EPSILONF;
  }

  float
  getDepth() const {
    return m_farPlane - m_nearPlane;
  }

  float
  getHalfFovYRadians() const {
    return Math::degToRad(m_fovYDegrees * 0.5f);
  }

  float
  getNearHalfHeight() const {
    return m_nearPlane * Math::tan(getHalfFovYRadians());
  }

  float
  getNearHalfWidth() const {
    return getNearHalfHeight() * m_aspectRatio;
  }

  float
  getFarHalfHeight() const {
    return m_farPlane * Math::tan(getHalfFovYRadians());
  }

  float
  getFarHalfWidth() const {
    return getFarHalfHeight() * m_aspectRatio;
  }

  float
  getNearWidth() const {
    return getNearHalfWidth() * 2.0f;
  }

  float
  getNearHeight() const {
    return getNearHalfHeight() * 2.0f;
  }

  float
  getFarWidth() const {
    return getFarHalfWidth() * 2.0f;
  }

  float
  getFarHeight() const {
    return getFarHalfHeight() * 2.0f;
  }

  float
  getNearArea() const {
    return getNearWidth() * getNearHeight();
  }

  float
  getFarArea() const {
    return getFarWidth() * getFarHeight();
  }

  /**
   * @brief Volume of a truncated pyramid frustum.
   */
  float
  getVolume() const {
    const float h = getDepth();
    const float a0 = getNearArea();
    const float a1 = getFarArea();
    return (h / 3.0f) * (a0 + a1 + Math::sqrt(a0 * a1));
  }

  Vector3f
  getCenter() const {
    const float zCenter = (m_nearPlane + m_farPlane) * 0.5f;
    return Vector3f(0.0f, 0.0f, zCenter);
  }

public:
  float m_nearPlane;
  float m_farPlane;
  float m_fovYDegrees;
  float m_aspectRatio;
};

}
