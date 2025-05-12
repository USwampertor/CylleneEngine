#include "cyRect.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK
{

bool
Rect::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::RECT) {
    const Rect& otherRect = static_cast<const Rect&>(other);

  }

  return false;
}

String
Rect::toString() {
  return Utils::format("x: %2.f, y: %2.f, width: %2.f, height %2.f", x, y, width, height);
}
}
