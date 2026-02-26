#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK
{

/**
 * @class Cone
 * @brief Cone defined by a base radius and height. The cone is assumed to be oriented
 *        along the positive Y-axis, with its apex at the origin and the base centered 
 *        at (0, -height, 0).
 */
class CY_MATH_EXPORT Cone : public Primitive
{
public:

  /**
   * @brief Default constructor initializes to a unit cone with radius 1 and height 1.
   */
  Cone()
    : Primitive(Cone::staticType()),
      m_radius(1.0f),
      m_height(1.0f) {}

  /**
   * @brief Constructor with specified radius and height.
   * @param radius Base radius of the cone. Must be positive for a valid cone.
   * @param height Height of the cone. Must be positive for a valid cone.
   */
  Cone(const float& radius, const float& height)
    : Primitive(Cone::staticType()),
      m_radius(radius),
      m_height(height) {}

  /**
   * @brief Copy constructor.
   * @param other The cone to copy from.
   */
  Cone(const Cone& other)
    : Primitive(Cone::staticType()),
      m_radius(other.m_radius),
      m_height(other.m_height) {}

  static PRIMITIVE_TYPE::E staticType() {
    return PRIMITIVE_TYPE::E::eCONE;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Sets both dimensions at once.
   * @param radius Base radius of the cone.
   * @param height Height of the cone.
   */
  void
  setDimensions(const float& radius, const float& height);

  /**
   * @brief Sets cone base radius.
   * @param radius Base radius of the cone.
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Sets cone height.
   * @param height Height of the cone.
   */
  void
  setHeight(const float& height) {
    m_height = height;
  }

  /**
   * @brief Gets cone base radius.
   * @return Base radius of the cone.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Gets cone height.
   * @return Height of the cone.
   */
  float
  getHeight() const {
    return m_height;
  }

  /**
   * @brief Returns true when dimensions are positive and usable.
   * @return True if dimensions are valid, false otherwise.
   */
  bool
  isValid() const;

  /**
   * @brief Gets slant height (generatrix): sqrt(r^2 + h^2).
   * @return Slant height of the cone.
   */
  float
  getSlantHeight() const;

  /**
   * @brief Base area: pi * r^2.
   * @return Base area of the cone.
   */
  float
  getBaseArea() const;

  /**
   * @brief Lateral area: pi * r * slantHeight.
   * @return Lateral surface area of the cone.
   */
  float
  getLateralSurfaceArea() const;

  /**
   * @brief Total surface area: base + lateral.
   * @return Total surface area of the cone.
   */
  float
  getSurfaceArea() const;

  /**
   * @brief Volume: (pi * r^2 * h) / 3.
   * @return Volume of the cone.
   */
  float
  getVolume() const;

public:

  /**
   * Radius of the cone's base. Must be positive for a valid cone.
   */
  float m_radius;

  /**
   * Height of the cone. Must be positive for a valid cone.
   */
  float m_height;

};

}
