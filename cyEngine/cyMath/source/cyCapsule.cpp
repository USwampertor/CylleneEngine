#include "cyCapsule.h"
#include "cyMath.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
Capsule::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f", 
                       m_pointA.toString(), 
                       m_pointB.toString(), 
                       m_radius);
}

bool
Capsule::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::CAPSULE) {
    const Capsule& otherCapsule = static_cast<const Capsule&>(other);
    
    Vector3f d1 = m_pointB - m_pointA;
    Vector3f d2 = otherCapsule.m_pointB - otherCapsule.m_pointA;
    Vector3f r = m_pointA - otherCapsule.m_pointA;
    float a = d1 | d1;
    float b = d1 | d2;
    float c = d2 | d2;
    float d = d1 | r;
    float e = d2 | r;

    float denom = a * c - b * b; // Denominator for parametric equations

    float s = (b * e - c * d) / denom; // Parametric point on segment 1
    float t = (a * e - b * d) / denom; // Parametric point on segment 2

    // Clamp s and t to [0, 1] to stay on the segments
    s = Math::max(0.0f, Math::min(1.0f, s));
    t = Math::max(0.0f, Math::min(1.0f, t));

    // Compute closest points
    Vector3f p1 = m_pointA + d1 * s;
    Vector3f p2 = otherCapsule.m_pointA + d2 * t;

    float distSqr = Vector3f::sqrDistance(p1, p2);
    float sumRadii = m_radius + otherCapsule.m_radius;
    return (distSqr <= sumRadii * sumRadii);

  }

  return false;
}



}