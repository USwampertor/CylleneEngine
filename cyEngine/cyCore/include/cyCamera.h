#pragma once
#include "cyCorePrerequisites.h"
#include "cyComponent.h"

#include <cyVector3f.h>
#include <cyMatrix4x4.h>


namespace CYLLENE_SDK {

class Camera : public Component
{
public:
  
  Camera() : Component(Camera::staticType()) {}

  ~Camera() = default;
  
  static COMPONENT_TYPE::E staticType() { return COMPONENT_TYPE::E::eCAMERA; }

  void
  setPerspective(const float& newWidth, 
                 const float& newHeight, 
                 const float& newZNear, 
                 const float& newZFar, 
                 const float& newFOV);
public:

  // Vector3f m_position;
  Vector3f m_target;
  Vector3f m_up;

  float m_fov;
  float m_width;
  float m_height;
  float m_zNear;
  float m_zFar;

  Matrix4x4 m_view;
  Matrix4x4 m_projection;


};

}

