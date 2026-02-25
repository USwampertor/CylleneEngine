#pragma once
#include "cyMathPrerequisites.h"
#include "cyMath.h"
#include "cyPrimitive.h"
#include "cyVector2i.h"

namespace CYLLENE_SDK {

/*
 *	@class Rect
 *	@brief A rectangle primitive.
 *  
 */
class CY_MATH_EXPORT Rect : public Primitive
{
public:
  Rect() : Primitive(Rect::staticType()) {}

  Rect(const Rect& other)
    : Primitive(Rect::staticType()),
      x(other.x),
      y(other.y),
      width(other.width),
      height(other.height) {}

  Rect(uint32 nx, uint32 ny, uint32 nwidth, uint32 nheight)
    : Primitive(Rect::staticType()),
      x(nx),
      y(ny),
      width(nwidth),
      height(nheight) {}

  static PRIMITIVE_TYPE::E 
  staticType() {
    return PRIMITIVE_TYPE::E::OBB;
  }

  virtual bool
  intersects(const Primitive& other) override;

  virtual String
  toString() override;

  /**
   * @brief Sets all rectangle values at once.
   */
  void
  set(const uint32& nx, const uint32& ny, const uint32& nwidth, const uint32& nheight) {
    x = nx;
    y = ny;
    width = nwidth;
    height = nheight;
  }

  void
  setPosition(const uint32& nx, const uint32& ny) {
    x = nx;
    y = ny;
  }

  void
  setSize(const uint32& nwidth, const uint32& nheight) {
    width = nwidth;
    height = nheight;
  }

  Vector2i
  getPosition() const {
    return Vector2i(static_cast<int32>(x), static_cast<int32>(y));
  }

  Vector2i
  getSize() const {
    return Vector2i(static_cast<int32>(width), static_cast<int32>(height));
  }

  uint32
  getLeft() const {
    return x;
  }

  uint32
  getTop() const {
    return y;
  }

  uint32
  getRight() const {
    return x + width;
  }

  uint32
  getBottom() const {
    return y + height;
  }

  bool
  isValid() const {
    return width > 0 && height > 0;
  }

  uint32
  getArea() const {
    return width * height;
  }

  uint32
  getPerimeter() const {
    return (width + height) * 2;
  }

  Vector2i
  getCenter() const {
    return Vector2i(static_cast<int32>(x + width / 2),
                    static_cast<int32>(y + height / 2));
  }

  bool
  contains(const uint32& px, const uint32& py) const {
    return px >= x && px < (x + width) &&
           py >= y && py < (y + height);
  }

  bool
  contains(const Vector2i& point) const {
    if (point.x < 0 || point.y < 0) {
      return false;
    }
    return contains(static_cast<uint32>(point.x), static_cast<uint32>(point.y));
  }

  bool
  contains(const Rect& other) const {
    return other.getLeft() >= getLeft() &&
           other.getTop() >= getTop() &&
           other.getRight() <= getRight() &&
           other.getBottom() <= getBottom();
  }

  bool
  overlaps(const Rect& other) const {
    return getLeft() < other.getRight() &&
           getRight() > other.getLeft() &&
           getTop() < other.getBottom() &&
           getBottom() > other.getTop();
  }

  Vector2i
  clampPoint(const Vector2i& point) const {
    if (!isValid()) {
      return getPosition();
    }

    const int32 minX = static_cast<int32>(getLeft());
    const int32 maxX = static_cast<int32>(getRight() - 1);
    const int32 minY = static_cast<int32>(getTop());
    const int32 maxY = static_cast<int32>(getBottom() - 1);

    return Vector2i(Math::clamp(point.x, minX, maxX),
                    Math::clamp(point.y, minY, maxY));
  }

public:
  
  uint32 x;
  uint32 y;
  uint32 width;
  uint32 height;

};

}
