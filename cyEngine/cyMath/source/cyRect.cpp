#include "cyRect.h"
#include "cyIntersections.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK
{

bool
Rect::intersects(const Primitive& other) {
  return COLLISIONS::intersects(*this, other);
}

String
Rect::toString() {
  return Utils::format("x: %2.f, y: %2.f, width: %2.f, height %2.f", m_x, m_y, m_width, m_height);
}

void
Rect::setDimensions(const uint32& nx, const uint32& ny, const uint32& nwidth, const uint32& nheight) {
  m_x = nx;
  m_y = ny;
  m_width = nwidth;
  m_height = nheight;
}

void
Rect::setPosition(const uint32& nx, const uint32& ny) {
  m_x = nx;
  m_y = ny;
}

void
Rect::setSize(const uint32& nwidth, const uint32& nheight) {
  m_width = nwidth;
  m_height = nheight;
}

Vector2i
Rect::getPosition() const {
  return Vector2i(static_cast<int32>(m_x), static_cast<int32>(m_y));
}

Vector2i
Rect::getSize() const {
  return Vector2i(static_cast<int32>(m_width), static_cast<int32>(m_height));
}

uint32
Rect::getLeft() const {
  return m_x;
}

uint32
Rect::getTop() const {
  return m_y;
}

uint32
Rect::getRight() const {
  return m_x + m_width;
}

uint32
Rect::getBottom() const {
  return m_y + m_height;
}

bool
Rect::isValid() const {
  return m_width > 0 && m_height > 0;
}

uint32
Rect::getArea() const {
  return m_width * m_height;
}

uint32
Rect::getPerimeter() const {
  return (m_width + m_height) * 2;
}

Vector2i
Rect::getCenter() const {
  return Vector2i(static_cast<int32>(m_x + m_width / 2),
    static_cast<int32>(m_y + m_height / 2));
}

bool
Rect::contains(const uint32& px, const uint32& py) const {
  return px >= m_x && px < (m_x + m_width) &&
    py >= m_y && py < (m_y + m_height);
}

bool
Rect::contains(const Vector2i& point) const {
  if (point.x < 0 || point.y < 0) {
    return false;
  }
  return contains(static_cast<uint32>(point.x), static_cast<uint32>(point.y));
}

bool
Rect::contains(const Rect& other) const {
  return other.getLeft() >= getLeft() &&
    other.getTop() >= getTop() &&
    other.getRight() <= getRight() &&
    other.getBottom() <= getBottom();
}

bool
Rect::overlaps(const Rect& other) const {
  return getLeft() < other.getRight() &&
    getRight() > other.getLeft() &&
    getTop() < other.getBottom() &&
    getBottom() > other.getTop();
}

Vector2i
Rect::clampPoint(const Vector2i& point) const {
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


}
