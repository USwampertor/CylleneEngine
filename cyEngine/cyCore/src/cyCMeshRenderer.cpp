#include "cyCMeshRenderer.h"

#include "cyBBeing.h"
#include "cyCTransform.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

void
CMeshRenderer::setModel(const SPtr<RModel>& newModel) {
  // TODO: THIS SHOULD BE A DESTROY, NOT A CLEAR
  m_owner->getTransform()->m_children.clear();
  for (int i = 0; i < newModel->m_meshes.size(); ++i) {
    String childName = Utils::format("%s_mesh_%d", m_owner->getName().c_str(), i);
    SPtr<BBeing> child = std::make_shared<BBeing>(childName);
    child->createComponent<CTransform>(Vector3f::ZERO, 
                                       Vector3f::ONE, 
                                       Quaternion::IDENTITY, 
                                       m_owner->getTransform()); // TODO: REVISION ON HOW DO YOU GET TRANSFORM
    child->createComponent<CMeshRenderer>(newModel->m_meshes[i]);
    m_owner->getTransform()->addChild(child->getTransform());
  }
}

}
