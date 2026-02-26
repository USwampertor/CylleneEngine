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

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Set perspective frustum parameters.
   * @param nearPlane Distance to the near clipping plane. Must be positive and less than farPlane.
   * @param farPlane Distance to the far clipping plane. Must be greater than nearPlane
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
   * @param nearPlane Distance to the near clipping plane. Must be positive and less than
   */
  void
  setNearPlane(const float& nearPlane) {
    m_nearPlane = nearPlane;
  }

  /**
   * @brief Set far plane distance.
   * @param farPlane Distance to the far clipping plane. Must be greater than nearPlane
   */
  void
  setFarPlane(const float& farPlane) {
    m_farPlane = farPlane;
  }

  /**
   * @brief Set vertical field of view in degrees.
   * @param fovYDegrees Vertical field of view in degrees
   */
  void
  setFovYDegrees(const float& fovYDegrees) {
    m_fovYDegrees = fovYDegrees;
  }

  /**
   * @brief Set aspect ratio (width/height) of the frustum.
   * @param aspectRatio Aspect ratio (width/height) of the frustum
   */
  void
  setAspectRatio(const float& aspectRatio) {
    m_aspectRatio = aspectRatio;
  }

  /**
   * @brief Getters for frustum near plane.
   * @return The current values of the near plane
   */
  float
  getNearPlane() const {
    return m_nearPlane;
  }

  /**
   * @brief Getters for frustum far plane.
   * @return The current values of the far plane
   */
  float
  getFarPlane() const {
    return m_farPlane;
  }

  /**
   * @brief Getters for frustum vertical field of view in degrees.
   * @return The current values of the vertical field of view in degrees
   */
  float
  getFovYDegrees() const {
    return m_fovYDegrees;
  }

  /**
   * @brief Getters for frustum aspect ratio (width/height).
   * @return The current values of the aspect ratio (width/height)
   */
  float
  getAspectRatio() const {
    return m_aspectRatio;
  }

  /**
   * @brief True when near/far/fov/aspect are physically valid.
   * @return True if the frustum parameters are valid (near plane > 0, far plane > near plane,
   */
  bool
  isValid() const;

  /**
   * @brief gets the depth of the frustum, which is the distance between the near and far planes.
   * @return The depth of the frustum, calculated as the difference between the far plane
   */
  float
  getDepth() const;

  /**
   * @brief gets the vertical field of view in radians,
            which is used for various calculations related to the frustum's geometry.
   * @return The vertical field of view in radians, calculated by converting the stored
   */
  float
  getHalfFovYRadians() const;

  /**
   * @brief gets the half width and half height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The half width and half height of the near and far planes, calculated using
   */
  float
  getNearHalfHeight() const;

  /**
   * @brief gets the half width and half height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The half width and half height of the near and far planes, calculated using
   */
  float
  getNearHalfWidth() const;

  /**
   * @brief gets the half width and half height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The half width and half height of the near and far planes, calculated using
   */
  float
  getFarHalfHeight() const;

  /**
   * @brief gets the half width and half height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The half width and half height of the near and far planes, calculated using
   */
  float
  getFarHalfWidth() const;

  /**
   * @brief gets the full width and height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The full width and height of the near and far planes, calculated by doubling
   */
  float
  getNearWidth() const;

  /**
   * @brief gets the full width and height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The full width and height of the near and far planes, calculated by doubling
   */
  float
  getNearHeight() const;

  /**
   * @brief gets the full width and height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The full width and height of the near and far planes, calculated by doubling
   */
  float
  getFarWidth() const;

  /**
   * @brief gets the full width and height of the near and far planes, which are used to calculate
            the dimensions of the frustum at different distances from the viewer.
   * @return The full width and height of the near and far planes, calculated by doubling
   */
  float
  getFarHeight() const;

  /**
   * @brief Gets the area of the near plane of the frustum.
   * @return The area of the near plane, calculated as the product 
   *         of the width and height of the near plane.
   */
  float
  getNearArea() const;

  /**
   * @brief Gets the area of the far plane of the frustum, which is calculated as 
   *        the product of the width and height of the far plane.
   * @return The area of the far plane, calculated as the product of the width and height
   *         of the far plane.
   */
  float
  getFarArea() const;

  /**
   * @brief Volume of a truncated pyramid frustum.
   * @return The volume of the frustum, calculated using the formula for the volume 
   *         of a truncated pyramid,
   */
  float
  getVolume() const;

  /**
   * @brief Gets the center point of the frustum in view space, which is the midpoint 
   *        between the near and far planes along the view direction.
   * @return The center point of the frustum in view space, calculated as the midpoint between
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
   * @brief Aspect ratio (width/height) of the frustum, which is used to calculate 
            the horizontal dimensions
   */
  float m_aspectRatio;
};

}
