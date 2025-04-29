#include "cyLine.h"
#include "cyMath.h"
#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Line::toString() {
  return Utils::format("A: %s, B: %s", m_a, m_b);
}

bool
Line::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::LINE) {
    const Line& otherCapsule = static_cast<const Line&>(other);

    Vector3f da = m_b - m_a;
    Vector3f db = otherCapsule.m_b - otherCapsule.m_a;
    Vector3f dc = otherCapsule.m_a - m_a;

    if ((dc | (da ^ db)) != 0.0f) {
      return false;
    }

    float s = ((dc ^ db) | (da ^ db)) / (da ^ db).sqrMagnitude();

    if (s >= 0.0 && s <= 1.0) {
      return true;
    }

  }
  return false;
}




}