#pragma once
#include "cyCorePrerequisites.h"
#include "cyVertex.h"

namespace CYLLENE_SDK {

class Triangle
{
public:

  Triangle() = default;

  Triangle(const Triangle& other) {
    _m = other._m;
  }

  Triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
    _m.v0 = a;
    _m.v1 = b;
    _m.v2 = c;
  }

  Vertex
  getBarycenter(const Vertex& p);


public:

  union {
    struct {
      Vertex v0, v1, v2;
    }_m;
    Vertex m[3];   
  };
};

}

