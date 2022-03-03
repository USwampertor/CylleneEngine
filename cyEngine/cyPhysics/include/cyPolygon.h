/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyPolygon.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/02
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyPhysicsPrerequisites.h"

#include <box2d/b2_polygon_shape.h>

namespace CYLLENE_SDK {
class CY_PHYSICS_EXPORT Polygon {
 public:

  Polygon() = default;

  virtual ~Polygon() = default;



  b2PolygonShape m_p;

};
}
