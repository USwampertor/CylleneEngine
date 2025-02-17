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


Vector<Triangle>
Triangle::getTopAndBottom() {
  Vertex p0 = _m.v0;
  Vertex p1 = _m.v1;
  Vertex p2 = _m.v2;
  float tVal = static_cast<float>(p1.m_position.y - p0.m_position.y) / 
               static_cast<float>(p2.m_position.y - p0.m_position.y);
  int newX = p0.m_position.x +
             static_cast<int32>(0.5f +
             static_cast<float>(p1.m_position.y - p0.m_position.y) *
             static_cast<float>(p2.m_position.x - p0.m_position.x) /
             static_cast<float>(p2.m_position.y - p0.m_position.y));

  float newU = p0.m_uv.x + 
               ((p1.m_position.y - p0.m_position.y) * ((p2.m_uv.x - p0.m_uv.x) / 
                (p2.m_position.y - p0.m_position.y)));
  float newV = p0.m_uv.y + 
               ((p1.m_position.y - p0.m_position.y) * ((p2.m_uv.y - p0.m_uv.y) / 
                (p2.m_position.y - p0.m_position.y)));
  
  Vertex newVtx(Vector3f(static_cast<float>(newX), p1.m_position.y, 0), 
                Vector3f::ZERO, 
                Vector3f::ZERO, 
                Vector3f::ZERO, 
                Vector2f(newU, newV));

  Color nC = _m.v0.m_color + 
             ((_m.v2.m_color - _m.v0.m_color) * (p1.m_position.y - p0.m_position.y) / 
              (p2.m_position.y - p0.m_position.y));

  Triangle t1(p0, newVtx, p1);
  Triangle t2(p1, newVtx, p2);

  t1._m.v0.m_color = _m.v0.m_color;
  t1._m.v1.m_color = _m.v1.m_color;
  t1._m.v2.m_color = _m.v1.m_color;

  t2._m.v0.m_color = _m.v1.m_color;
  t2._m.v1.m_color = _m.v2.m_color;
  t2._m.v2.m_color = _m.v2.m_color;

  Vector<Triangle> vec;

  // Bottom
  vec.push_back(t1);
  // Top
  vec.push_back(t2);

  return vec;
}

}
