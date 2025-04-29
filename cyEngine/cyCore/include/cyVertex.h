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

#include <cyColor.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>


namespace CYLLENE_SDK {

/*
 *	@class  Vertex
 *	@brief	A vertex is a point in space, that can contain information for 
 *          meshes such as uv, position, normal, tangent, binormal, color,
 *          bones, and weights.
 *
 */
struct CY_CORE_EXPORT Vertex {
public:
  /**
   * Default constructor
   */
  Vertex() {
    m_position  = Vector3f::ZERO;
    m_normal    = Vector3f::ZERO;
    m_tangent   = Vector3f::ZERO;
    m_binormal  = Vector3f::ZERO;
    m_uv        = Vector2f::ZERO;
    m_color     = Color::CLEAR;
    m_boneIDs[0] = m_boneIDs[1] = m_boneIDs[2] = m_boneIDs[3] = 0;
    m_boneWeights[0] = m_boneWeights[1] = m_boneWeights[2] = m_boneWeights[3] = 0.0f;
    m_customData[0] = m_customData[1] = m_customData[2] = m_customData[3] = -1;
  }

  Vertex(const Vector3f& position, 
         const Vector3f& normal = Vector3f::ZERO, 
         const Vector3f& tangent = Vector3f::ZERO, 
         const Vector3f& binormal = Vector3f::ZERO, 
         const Vector2f& uv = Vector2f::ZERO,
         const Color& color = Color::CLEAR, 
         const Array<int32_t, 4>& boneIDs = {0,0,0,0},
         const Array<float, 4>& boneweights = {0.0f, 0.0f, 0.0f, 0.0f})
    : m_position(position),
      m_color(color),
      m_normal(normal),
      m_tangent(normal),
      m_binormal(normal),
      m_uv(uv) {
        for (int i = 0; i < 4; ++i) {
          m_boneIDs[i]      = boneIDs[i];
          m_boneWeights[i]  = boneweights[i];
          m_customData[i]   = -1;

        }
      }

public:

  /**
   * The position in 3d space
   */
  Vector3f m_position;

  /**
   * The normal vector towards where its pointing
   */
  Vector3f m_normal;

  /**
   * The tangent of the vertex
   */
  Vector3f m_tangent;

  /**
   * The binormal of the vertex
   */
  Vector3f m_binormal;

  /**
   * The color associated with this vertex
   */
  Color    m_color;

  /**
   * The UV associated with this vertex
   */
  Vector2f m_uv;

  /**
   * An array of up to 4 bones that can be associated with this vertex
   */
  int32 m_boneIDs[4];

  /**
   * How much the bones associated with this vertex can modify it
   */
  float m_boneWeights[4];

  /**
   * Extra custom data to set in a vertex if you wish for specific shader info
   * See: https://graphics.pixar.com/library/ToonRendering/paper.pdf
   */
  int32 m_customData[4];


  static const Vertex ZERO;

};

}


