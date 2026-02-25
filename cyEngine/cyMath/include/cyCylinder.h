#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"

namespace CYLLENE_SDK {

class CY_MATH_EXPORT Cylinder : public Primitive
{
public:

  Cylinder()
    : Primitive(Cylinder::staticType()),
      m_radius(1.0f),
      m_height(1.0f) {}

  Cylinder(const float& radius, const float& height)
    : Primitive(Cylinder::staticType()),
      m_radius(radius),
      m_height(height) {}

  Cylinder(const Cylinder& other)
    : Primitive(Cylinder::staticType()),
      m_radius(other.m_radius),
      m_height(other.m_height) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::CYLINDER;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Sets both dimensions at once.
   */
  void
  setDimensions(const float& radius, const float& height) {
    m_radius = radius;
    m_height = height;
  }

  /**
   * @brief Sets cylinder radius.
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Sets cylinder height.
   */
  void
  setHeight(const float& height) {
    m_height = height;
  }

  /**
   * @brief Gets cylinder radius.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Gets cylinder height.
   */
  float
  getHeight() const {
    return m_height;
  }

  /**
   * @brief Gets cylinder diameter.
   */
  float
  getDiameter() const {
    return m_radius * 2.0f;
  }

  /**
   * @brief Gets half height.
   */
  float
  getHalfHeight() const {
    return m_height * 0.5f;
  }

  /**
   * @brief Returns true when dimensions are positive and usable.
   */
  bool
  isValid() const {
    return m_radius > Math::EPSILONF && m_height > Math::EPSILONF;
  }

  /**
   * @brief Base area: pi * r^2.
   */
  float
  getBaseArea() const {
    return Math::PI * Math::sqr(m_radius);
  }

  /**
   * @brief Lateral area: 2 * pi * r * h.
   */
  float
  getLateralSurfaceArea() const {
    return 2.0f * Math::PI * m_radius * m_height;
  }

  /**
   * @brief Total surface area: 2*base + lateral.
   */
  float
  getSurfaceArea() const {
    return 2.0f * getBaseArea() + getLateralSurfaceArea();
  }

  /**
   * @brief Volume: pi * r^2 * h.
   */
  float
  getVolume() const {
    return getBaseArea() * m_height;
  }

public:

  float m_radius;
  float m_height;

};

}
