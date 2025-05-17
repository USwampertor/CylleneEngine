#include "cyTransform.h"

namespace CYLLENE_SDK {



void
TransformComponent::setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  m_tMatrix.view(eyePos, targetPos, upDir);
}

}