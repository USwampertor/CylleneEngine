#include "cyCamera.h"

namespace CYLLENE_SDK {

void
Camera::setLookAt(const Vector3f& position,
  const Vector3f& target,
  const Vector3f& up)
{
  m_target = target;
  m_up = up;
  m_view = Matrix4::lookAt(position, target, up);
}

}
