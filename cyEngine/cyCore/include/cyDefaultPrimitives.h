/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyDefaultPrimitives.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/09/30
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"
#include "cyVertex.h"

namespace CYLLENE_SDK
{
  namespace DEFAULTPRIMS
  {
    struct Triangle
    {
      Triangle() = default;
      Triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
        _m.v0 = a;
        _m.v1 = b;
        _m.v2 = c;
      }

      Triangle(const Triangle& copy)
        : _m(copy._m) {}


      union {
        struct {
          Vertex v0, v1, v2;
        }_m;
        Vertex m[3];
      };


    };
  }



}
