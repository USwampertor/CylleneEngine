/**
 * @file cyFrustum.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Frustum.
 */
#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {

/**
 * @class Frustum
 * @brief Represents a perspective frustum defined by near and far planes, 
 *        vertical field of view, and aspect ratio.
 */
class CY_MATH_EXPORT Frustum : public Primitive
{
public:

  Frustum()
    : Primitive(Frustum::staticType()),
      m_nearPlane(0.1f),
      m_farPlane(1000.0f),
      m_fovYDegrees(60.0f),
      m_aspectRatio(16.0f / 9.0f) {}

  Frustum(const float& nearPlane,
           const float& farPlane,
           const float& fovYDegrees,
           const float& aspectRatio)
    : Primitive(Frustum::staticType()),
      m_nearPlane(nearPlane),
      m_farPlane(farPlane),
      m_fovYDegrees(fovYDegrees),
      m_aspectRatio(aspectRatio) {}

  Frustum(const Frustum& other)
    : Primitive(Frustum::staticType()),
      m_nearPlane(other.m_nearPlane),
      m_farPlane(other.m_farPlane),
      m_fovYDegrees(other.m_fovYDegrees),
      m_aspectRatio(other.m_aspectRatio) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::eFRUSTUM;
  }

  /**
   * @brief Converts frustum to string representation.
   * @return Formatted frustum string.
   */
  virtual String
  toString() override;

  /**
   * @brief Checks if this frustum intersects another primitive.
   * @param other Primitive to test.
   * @return True if intersecting.
   */
  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Set perspective frustum parameters.
   * @param nearPlane Distance to the near clipping plane. Must be positive and less than farPlane.
   * @param farPlane Distance to the far clipping plane. Must be greater than nearPlane.
   * @param fovYDegrees Vertical field of view in degrees.
   * @param aspectRatio Aspect ratio (width/height) of the frustum.
   */
  void
  setDimensions(const float& nearPlane,
                const float& farPlane,
                const float& fovYDegrees,
                const float& aspectRatio);

  /**
   * @brief Set near plane distance.
   * @param nearPlane Distance to the near clipping plane.
   */
  void
  setNearPlane(const float& nearPlane) {
    m_nearPlane = nearPlane;
  }

  /**
   * @brief Set far plane distance.
   * @param farPlane Distance to the far clipping plane.
   */
  void
  setFarPlane(const float& farPlane) {
    m_farPlane = farPlane;
  }

  /**
   * @brief Set vertical field of view in degrees.
   * @param fovYDegrees Vertical field of view in degrees.
   */
  void
  setFovYDegrees(const float& fovYDegrees) {
    m_fovYDegrees = fovYDegrees;
  }

  /**
   * @brief Set aspect ratio (width/height) of the frustum.
   * @param aspectRatio Aspect ratio (width/height) of the frustum.
   */
  void
  setAspectRatio(const float& aspectRatio) {
    m_aspectRatio = aspectRatio;
  }

  /**
   * @brief Gets near plane distance.
   * @return Near plane distance.
   */
  float
  getNearPlane() const {
    return m_nearPlane;
  }

  /**
   * @brief Gets far plane distance.
   * @return Far plane distance.
   */
  float
  getFarPlane() const {
    return m_farPlane;
  }

  /**
   * @brief Gets vertical field of view in degrees.
   * @return Vertical field of view in degrees.
   */
  float
  getFovYDegrees() const {
    return m_fovYDegrees;
  }

  /**
   * @brief Gets aspect ratio.
   * @return Aspect ratio (width/height).
   */
  float
  getAspectRatio() const {
    return m_aspectRatio;
  }

  /**
   * @brief True when near/far/fov/aspect are physically valid.
   * @return True if parameters are valid, false otherwise.
   */
  bool
  isValid() const;

  /**
   * @brief Gets frustum depth.
   * @return Distance between far and near planes.
   */
  float
  getDepth() const;

  /**
   * @brief Gets half vertical field of view in radians.
   * @return Half of vertical field of view, in radians.
   */
  float
  getHalfFovYRadians() const;

  /**
   * @brief Gets near plane half height.
   * @return Half height of near plane.
   */
  float
  getNearHalfHeight() const;

  /**
   * @brief Gets near plane half width.
   * @return Half width of near plane.
   */
  float
  getNearHalfWidth() const;

  /**
   * @brief Gets far plane half height.
   * @return Half height of far plane.
   */
  float
  getFarHalfHeight() const;

  /**
   * @brief Gets far plane half width.
   * @return Half width of far plane.
   */
  float
  getFarHalfWidth() const;

  /**
   * @brief Gets near plane full width.
   * @return Full width of near plane.
   */
  float
  getNearWidth() const;

  /**
   * @brief Gets near plane full height.
   * @return Full height of near plane.
   */
  float
  getNearHeight() const;

  /**
   * @brief Gets far plane full width.
   * @return Full width of far plane.
   */
  float
  getFarWidth() const;

  /**
   * @brief Gets far plane full height.
   * @return Full height of far plane.
   */
  float
  getFarHeight() const;

  /**
   * @brief Gets near plane area.
   * @return Area of near plane.
   */
  float
  getNearArea() const;

  /**
   * @brief Gets far plane area.
   * @return Area of far plane.
   */
  float
  getFarArea() const;

  /**
   * @brief Gets frustum volume.
   * @return Volume of frustum.
   */
  float
  getVolume() const;

  /**
   * @brief Gets frustum center in view space.
   * @return Center point between near and far planes.
   */
  Vector3f
  getCenter() const;

public:

  /**
   * @brief Near plane distance, which defines the closest distance from the viewer 
   *        at which objects will be rendered.
   */
  float m_nearPlane;

  /**
   * @brief Far plane distance, which defines the farthest distance from the viewer 
   *        at which objects will be rendered.
   */
  float m_farPlane;

  /**
   * @brief Vertical field of view in degrees, which defines the vertical angle of the frustum.
   *        This is used to calculate the dimensions of the frustum at different distances from the viewer.
   */
  float m_fovYDegrees;

  /**
   * @brief Aspect ratio (width/height) of the frustum.
   */
  float m_aspectRatio;
};

}

