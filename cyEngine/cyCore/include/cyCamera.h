#pragma once
#include "cyCorePrerequisites.h"
#include "cyComponent.h"

#include <cyVector3f.h>
#include <cyMatrix4.h>


namespace CYLLENE_SDK {


  namespace CAMERA_TYPE
  {
    BETTER_ENUM(E, uint32, GENERAL, PERSPECTIVE, ORTHOGRAPHIC);
  }


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

  void
  setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f upDir);

  void
  changeCameraType(const CAMERA_TYPE::E& type);

public:


  float m_fov;
  float m_width;
  float m_height;
  float m_zNear;
  float m_zFar;

  bool m_viewDirty;
  bool projectionDirty;


  // View Matrix is updated really by the transform component and then set as dirty
  Matrix4 m_view;

  // Projection is created after values are set
  Matrix4 m_projection;


};

}

