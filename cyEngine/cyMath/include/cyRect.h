#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector2i.h"

namespace CYLLENE_SDK {

/*
 *	@class Rect
 *	@brief A rectangle primitive.
 */
class CY_MATH_EXPORT Rect : public Primitive
{
public:
  Rect() : Primitive(Rect::staticType()) {}

  Rect(const Rect& other)
    : Primitive(Rect::staticType()),
      m_x(other.m_x),
      m_y(other.m_y),
      m_width(other.m_width),
      m_height(other.m_height) {}

  Rect(uint32 nx, uint32 ny, uint32 nwidth, uint32 nheight)
    : Primitive(Rect::staticType()),
      m_x(nx),
      m_y(ny),
      m_width(nwidth),
      m_height(nheight) {}

  static PRIMITIVE_TYPE::E 
  staticType() {
    return PRIMITIVE_TYPE::E::eRECT;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets all rectangle values at once.
   * @param nx The x-coordinate of the rectangle's top-left corner.
   * @param ny The y-coordinate of the rectangle's top-left corner.
   * @param nwidth The width of the rectangle.
   * @param nheight The height of the rectangle.
   */
  void
  setDimensions(const uint32& nx, 
                const uint32& ny, 
                const uint32& nwidth, 
                const uint32& nheight);

  /**
   * @brief Sets the x-coordinate of the rectangle's top-left corner.
   * @param nx The x-coordinate of the rectangle's top-left corner.
   */
  void
  setX(const uint32& nx) {
    m_x = nx;
  }

  /**
   * @brief Sets the y-coordinate of the rectangle's top-left corner.
   * @param ny The y-coordinate of the rectangle's top-left corner.
   */
  void
  setY(const uint32& ny) {
    m_y = ny;
  }

  /**
   * @brief Sets the width of the rectangle.
   * @param nwidth The width of the rectangle.
   */
  void
  setWidth(const uint32& nwidth) {
    m_width = nwidth;
  }

  /**
   * @brief Sets the height of the rectangle.
   * @param nheight The height of the rectangle.
   */
  void
  setHeight(const uint32& nheight) {
    m_height = nheight;
  }

  /**
   * @brief Gets the x-coordinate of the rectangle's top-left corner.
   * @return The x-coordinate of the rectangle's top-left corner.
   */
  uint32
  getX() const {
    return m_x;
  }

  /**
   * @brief Gets the y-coordinate of the rectangle's top-left corner.
   * @return The y-coordinate of the rectangle's top-left corner.
   */
  uint32
  getY() const {
    return m_y;
  }

  /**
   * @brief Gets the width of the rectangle.
   * @return The width of the rectangle.
   */
  uint32
  getWidth() const {
    return m_width;
  }

  /**
   * @brief Gets the height of the rectangle.
   * @return The height of the rectangle.
   */
  uint32
  getHeight() const {
    return m_height;
  }

  /**
   * @brief Sets the position of the rectangle's top-left corner.
   * @param nx The x-coordinate of the rectangle's top-left corner.
   * @param ny The y-coordinate of the rectangle's top-left corner.
   */
  void
  setPosition(const uint32& nx, const uint32& ny);

  /**
   * @brief Sets the size of the rectangle.
   * @param nwidth The width of the rectangle.
   * @param nheight The height of the rectangle.
   */
  void
  setSize(const uint32& nwidth, const uint32& nheight);

  /**
   * @brief Gets the position of the rectangle's top-left corner as a Vector2i.
   * @return A Vector2i representing the x and y coordinates 
   *         of the rectangle's top-left corner.
   */
  Vector2i
  getPosition() const;

  /**
   * @brief Gets the size of the rectangle as a Vector2i.
   * @return A Vector2i representing the width and height of the rectangle.
   */
  Vector2i
  getSize() const;

  /**
   * @brief Gets the left
   * @return The left edge (x)
   */
  uint32
  getLeft() const;

  /**
   * @brief Gets the top edge (y)
   * @return The top edge (y)
   */
  uint32
  getTop() const;

  /**
   * @brief Gets the right edge (x + width)
   * @return The right edge (x + width)
   */
  uint32
  getRight() const;

  /**
   * @brief Gets the bottom edge (y + height)
   * @param The bottom edge (y + height)
   */
  uint32
  getBottom() const;

  /**
   * @brief Checks if the rectangle has valid dimensions (positive width and height).
   * @return True if the rectangle has positive width and height, false otherwise.
   *         A rectangle with zero or negative width or height is considered invalid, 
   *         as it does not represent a meaningful area in 2D space.
   */
  bool
  isValid() const;

  /**
   * @brief Gets the area of the rectangle.
   * @return The area of the rectangle calculated as width multiplied by height.
   */
  uint32
  getArea() const;

  /**
    * @brief Gets the perimeter of the rectangle.
    * @return The perimeter of the rectangle calculated as 2 times the sum of width and height.
    */
  uint32
  getPerimeter() const;

  /**
   * @brief Gets the center point of the rectangle.
   * @return A Vector2i representing the x and y coordinates of the rectangle's center point,
   */
  Vector2i
  getCenter() const;

  /**
   * @brief Checks if a point is contained within the rectangle (including edges).
   * @param px The x-coordinate of the point to check.
   * @param py The y-coordinate of the point to check.
   * @return True if the point (px, py) is inside the rectangle or on its edge, false otherwise.
   */
  bool
  contains(const uint32& px, const uint32& py) const;

  /**
   * @brief Checks if a point is contained within the rectangle (including edges).
   * @param point A Vector2i representing the x and y coordinates of the point to check.
   * @return True if the point is inside the rectangle or on its edge, false otherwise.
    *         If the point has negative coordinates, it is considered outside the rectangle, 
    *         as rectangles are defined in the positive coordinate space.
   */
  bool
  contains(const Vector2i& point) const;

  /**
   * @brief Checks if another rectangle is entirely contained within this rectangle 
   *        (including edges).
   * @param other The other rectangle to check for containment.
   * @return True if the other rectangle is entirely inside this rectangle or on its edge, false otherwise.
   *         If the other rectangle has negative coordinates or dimensions, it is considered outside this rectangle, 
   *         as rectangles are defined in the positive coordinate space and must have positive width and height to be valid.
   */
  bool
  contains(const Rect& other) const;

  /**
   * @brief Checks if this rectangle overlaps with another rectangle (i.e., they share any area).
   * @param other The other rectangle to check for overlap.
   * @return True if this rectangle overlaps with the other rectangle, false otherwise.
   *         If either rectangle has negative coordinates or dimensions, they are considered non-overlapping, 
   *         as rectangles are defined in the positive coordinate space and must have positive width and height to be valid.
   */
  bool
  overlaps(const Rect& other) const;

  /**
   * @brief Clamps a point to be within the bounds of the rectangle.
   * @param point A Vector2i representing the x and y coordinates of the point to clamp.
   * @return A Vector2i representing the clamped point, which will be the original 
   *         point if it is within the rectangle,
   */
  Vector2i
  clampPoint(const Vector2i& point) const;

public:
  
  /**
   * @brief The x-coordinate of the rectangle's top-left corner.
   */
  uint32 m_x;

  /**
   * @brief The y-coordinate of the rectangle's top-left corner.
   */
  uint32 m_y;

  /**
   * @brief The width of the rectangle.
   */
  uint32 m_width;

  /**
   * @brief The height of the rectangle.
   */
  uint32 m_height;

};

}
