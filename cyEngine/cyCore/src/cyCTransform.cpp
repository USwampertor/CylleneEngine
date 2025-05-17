#include "cyCTransform.h"

namespace CYLLENE_SDK {



void
CTransform::setLookAt(const Vector3f& eyePos, const Vector3f& targetPos, const Vector3f& upDir) {
  m_tMatrix.view(eyePos, targetPos, upDir);
}

}