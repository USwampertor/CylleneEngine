/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyVertex.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/12/15
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>


namespace CYLLENE_SDK {
struct CY_CORE_EXPORT Vertex {

  Vertex() {
    m_uv        = Vector2f::ZERO;
    m_position  = Vector3f::ZERO;
    m_normal    = Vector3f::ZERO;
    m_tangent   = Vector3f::ZERO;
    m_binormal  = Vector3f::ZERO;
    m_color     = Vector3f::ZERO;
    m_boneIDs[0] = m_boneIDs[1] = m_boneIDs[2] = m_boneIDs[3] = 0;
    m_boneWeights[0] = m_boneWeights[1] = m_boneWeights[2] = m_boneWeights[3] = 0.0f;
  }

  Vertex(Vector3f position, 
         Vector3f color, 
         Vector3f normal, 
         Vector3f tangent, 
         Vector3f binormal, 
         Vector2f uv,
         int32 boneIDs[4],
         float boneweights[4])
    : m_position(position),
      m_color(color),
      m_normal(normal),
      m_tangent(normal),
      m_binormal(normal),
      m_uv(uv) {
        for (int i = 0; i < 4; ++i) {
          m_boneIDs[i]      = boneIDs[i];
          m_boneWeights[i]  = boneweights[i];
        }
      }

public:

  Vector3f m_position;
  Vector3f m_normal;
  Vector3f m_tangent;
  Vector3f m_binormal;
  Vector3f m_color;

  Vector2f m_uv;

  int32 m_boneIDs[4];

  float m_boneWeights[4];

};
}


