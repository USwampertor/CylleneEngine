#pragma once


#include <box2d/b2_math.h>
#include <reactphysics3d/mathematics/Matrix2x2.h>
#include <reactphysics3d/mathematics/Matrix3x3.h>
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Vector2.h>
#include <reactphysics3d/mathematics/Vector3.h>

#include <cyMatrix2x2.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyQuaternion.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

namespace CYLLENE_SDK {

// All this functions should be internal and not be available for users
// As this lets us integrate other libraries

Vector2i
toVector2i(const b2Vec2& v) {
  return Vector2i(v.x, v.y);
}

Vector2f
toVector2f(const b2Vec2& v) {
  return Vector2f(v.x, v.y);
}

b2Vec2
tob2Vec2(const Vector2i& v) {
  return b2Vec2(v.x, v.y);
}

b2Vec2
tob2Vec2(const Vector2f& v) {
  return b2Vec2(v.x, v.y);
}

}

reactphysics3d::Matrix2x2::Matrix2x2(const CYLLENE_SDK::Matrix2x2& m) {
  mRows[0][0] = m.m[0][0]; mRows[0][1] = m.m[0][1];
  mRows[1][0] = m.m[1][0]; mRows[1][1] = m.m[1][1];
}

reactphysics3d::Matrix3x3::Matrix3x3(const CYLLENE_SDK::Matrix3x3& m) {
  mRows[0][0] = m.m[0][0]; mRows[0][1] = m.m[0][1]; mRows[0][2] = m.m[0][2];
  mRows[1][0] = m.m[1][0]; mRows[1][1] = m.m[1][1]; mRows[1][2] = m.m[1][2];
  mRows[2][0] = m.m[2][0]; mRows[2][1] = m.m[2][1]; mRows[2][2] = m.m[2][2];
}

reactphysics3d::Quaternion::Quaternion(const CYLLENE_SDK::Quaternion& q) {
  x = q.x;
  y = q.y;
  z = q.z;
  w = q.w;
}

reactphysics3d::Quaternion::Quaternion(const CYLLENE_SDK::Vector4f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = v.w;
}

reactphysics3d::Quaternion::Quaternion(const CYLLENE_SDK::Vector3f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = 0.0f;
}

reactphysics3d::Vector2::Vector2(const CYLLENE_SDK::Vector2i& v) {
  x = v.x;
  y = v.y;
}

reactphysics3d::Vector2::Vector2(const CYLLENE_SDK::Vector2f& v) {
  x = v.x;
  y = v.y;
}

reactphysics3d::Vector2::Vector2(const CYLLENE_SDK::Vector3f& v) {
  x = v.x;
  y = v.y;
}

reactphysics3d::Vector3::Vector3(const CYLLENE_SDK::Vector3f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

b2Vec2::b2Vec2(const CYLLENE_SDK::Vector2f& v) {
  x = v.x;
  y = v.y;
}

b2Vec2::b2Vec2(const CYLLENE_SDK::Vector2i& v) {
  x = v.x;
  y = v.y;
}
