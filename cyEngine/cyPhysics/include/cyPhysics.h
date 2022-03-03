/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyPhysics.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/02
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include <cyModule.h>
#include <box2d/box2d.h>
#include <reactphysics3d/reactphysics3d.h>

#include "cyPhysicsPrerequisites.h"

#include "cyPhysicsMathRemap.h"

namespace CYLLENE_SDK {

using AABB      = reactphysics3d::AABB;
using Box3D     = reactphysics3d::BoxShape;
using Capsule   = reactphysics3d::CapsuleShape;
using RigidBody = reactphysics3d::RigidBody;

class Physics : Module <Physics>
{
 public:

  Physics() = default;

  ~Physics() = default;

  bool
  init();


  Vector2f m_gravity;
};
}
