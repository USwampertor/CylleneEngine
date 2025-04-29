#include "cyTriangle.h"

namespace CYLLENE_SDK {

Vertex
Triangle::getBarycenter(const Vertex& p) {
  Vector3f v0 = _m.v1.m_position - _m.v0.m_position;
  Vector3f v1 = _m.v1.m_position - _m.v0.m_position;
  Vector3f v2 = p.m_position - _m.v0.m_position;

  float d00 = Vector3f::dot(v0, v0);
  float d01 = Vector3f::dot(v0, v1);
  float d11 = Vector3f::dot(v1, v1);
  float d20 = Vector3f::dot(v2, v0);
  float d21 = Vector3f::dot(v2, v1);

  float denom = d00 * d11 - d01 * d01;

  float v = (d11 * d20 - d01 * d21) / denom;
  float w = (d00 * d21 - d01 * d20) / denom;
  float u = 1.0f - v - w;

  return Vertex(Vector3f(u, v, w));
}
}
