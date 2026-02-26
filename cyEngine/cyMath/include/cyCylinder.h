#pragma once
#include "cyMathPrerequisites.h"

#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector3f.h"

namespace CYLLENE_SDK {

/**
 * @class Cylinder
 * @brief Cylinder defined by a radius and height. The cylinder is assumed to be oriented
 *        along the positive Y-axis, with its center at the origin. The circular bases are 
 *        parallel to the XZ-plane, with one base centered at (0, -height/2, 0) and the 
 *        other at (0, height/2, 0).
 */
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
    return PRIMITIVE_TYPE::E::eCYLINDER;
  }

  virtual String
  toString() override;

  virtual bool
  intersects(const Primitive& other) override;

  /**
   * @brief Sets both dimensions at once.
   * @param radius The radius of the cylinder's circular base
   * @param height The height of the cylinder
   */
  void
  setDimensions(const float& radius, const float& height);

  /**
   * @brief Sets cylinder radius.
   * @param radius The radius of the cylinder's circular base
   */
  void
  setRadius(const float& radius) {
    m_radius = radius;
  }

  /**
   * @brief Sets cylinder height.
   * @param height The height of the cylinder
   */
  void
  setHeight(const float& height) {
    m_height = height;
  }

  /**
   * @brief Gets cylinder radius.
   * @return The radius of the cylinder's circular base, 
   *         which is the distance from the center of the base to its edge.
   */
  float
  getRadius() const {
    return m_radius;
  }

  /**
   * @brief Gets cylinder height.
   * @return The height of the cylinder, which is the distance between the two circular bases. 
   */
  float
  getHeight() const {
    return m_height;
  }

  /**
   * @brief Gets cylinder diameter.
   * @return The diameter of the cylinder, which is twice the radius. 
             This is a common measurement used in various calculations and can be 
             more intuitive in certain contexts than the radius alone.
   */
  float
  getDiameter() const;

  /**
   * @brief Gets half height.
   * @return Half of the cylinder's height, which is useful for calculations that require
   *         the distance from the center to the top or bottom face of the cylinder.
   */
  float
  getHalfHeight() const;

  /**
   * @brief Returns true when dimensions are positive and usable.
   * @return True if the cylinder has valid dimensions (positive radius and height), 
   *         false otherwise.
   */
  bool
  isValid() const;

  /**
   * @brief Base area: pi * r^2.
   * @return The area of one circular base of the cylinder. Since a cylinder has 
   *         two identical circular bases, the total area of both bases would be 
   *         2 * pi * r^2.
   */
  float
  getBaseArea() const;

  /**
   * @brief Lateral area: 2 * pi * r * h.
   * @return The lateral surface area of the cylinder, which is the area of the 
   *         curved surface connecting the two circular bases.
   */
  float
  getLateralSurfaceArea() const;

  /**
   * @brief Total surface area: 2*base + lateral.
   * @return The total surface area of the cylinder.
   */
  float
  getSurfaceArea() const;

  /**
   * @brief Volume: pi * r^2 * h.
   * @return The volume of the cylinder.
   */
  float
  getVolume() const;

  /**
   * @brief Gets the center point of the cylinder.
   * @return The center point of the cylinder, which is located at the midpoint
   */
  Vector3f
  getCenter() const;

public:

  /**
   * @brief Radius of the cylinder base. 
   *        Must be positive and greater than zero for a valid cylinder.
   */
  float m_radius;

  /* 
   * @brief Height of the cylinder.
   */
  float m_height;

};

}
