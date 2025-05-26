#include "cyCCamera.h"
#include "cyCTransform.h"
#include "cyBBeing.h"

namespace CYLLENE_SDK {

void
CCamera::setPerspective(const float& newWidth, 
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
CCamera::setOrthogonal(const float& newWidth,
                       const float& newHeight,
                       const float& newZNear,
                       const float& newZFar) {
  m_width = newWidth;
  m_height = newHeight;
  m_zNear = newZNear;
  m_zFar = newZFar;
  m_projection.orthogonal(m_width, m_height, m_zNear, m_zFar);
}

void
CCamera::setLookAt(const Vector3f& eyePos,
                  const Vector3f& targetPos,
                  const Vector3f upDir) {
  if (m_owner != nullptr) {
    m_owner->getComponent<CTransform>()->setLookAt(eyePos, targetPos, upDir);
    // TODO: Use the dirty flag system
    m_view = m_owner->getComponent<CTransform>()->m_tMatrix;
  }
}

void
CCamera::changeCameraType(const CAMERA_TYPE::E& type) {

}


}
