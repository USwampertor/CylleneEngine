#include "cyCamera.h"
#include "cyTransform.h"
#include "cyBeing.h"

namespace CYLLENE_SDK {

void
Camera::setPerspective(const float& newWidth, 
                       const float& newHeight, 
                       const float& newZNear, 
                       const float& newZFar, 
                       const float& newFOV) {
  m_fov = newFOV;
  m_width = newWidth;
  m_height = newHeight;
  m_zNear = newZNear;
  m_zFar = newZFar;
  m_projection.perspective(m_width, m_height, m_zNear, m_zFar, m_fov);
}

void
Camera::setLookAt(const Vector3f& eyePos,
                  const Vector3f& targetPos,
                  const Vector3f upDir) {
  if (m_owner != nullptr) {
    m_owner->getComponent<TransformComponent>()->setLookAt(eyePos, targetPos, upDir);
    // TODO: Use the dirty flag system
    m_view = m_owner->getComponent<TransformComponent>()->m_tMatrix;
  }
}

void
Camera::changeCameraType(const CAMERA_TYPE::E& type) {

}


}
