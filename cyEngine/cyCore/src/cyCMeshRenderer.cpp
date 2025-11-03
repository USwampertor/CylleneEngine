#include "cyCMeshRenderer.h"

#include "cyBBeing.h"
#include "cyCTransform.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

void
CMeshRenderer::setModel(const SPtr<RModel>& newModel) {
  // TODO: THIS SHOULD BE A DESTROY, NOT A CLEAR
  m_owner.lock()->/*getTransform()->m_children*/getChildren().clear();
  for (int i = 0; i < newModel->m_meshes.size(); ++i) {
    String childName = Utils::format("%s_mesh_%d", m_owner.lock()->getName().c_str(), i);
    SPtr<BBeing> child = makeSharedPtr<BBeing>(childName);
    // child->createComponent<CTransform>(Vector3f::ZERO, 
    //                                    Vector3f::ONE, 
    //                                    Quaternion::IDENTITY); // TODO: REVISION ON HOW DO YOU GET TRANSFORM
    WPtr<CMeshRenderer> subMesh = child->createComponent<CMeshRenderer>(newModel->m_meshes[i]);
    m_owner.lock()/*->getTransform()*/->addChild(child/*->getTransform()*/);
    
  }
}

void
CMeshRenderer::createMaterialInstance() {
  if (m_mesh) {
    String matInstanceChildName = Utils::format("%s_matInst", m_owner.lock()->getName().c_str());
    m_materialInstance = ResourceManager::instance().get<RMaterialInstance>(matInstanceChildName);
    if (!m_materialInstance.lock()) {
      m_materialInstance = ResourceManager::instance().create<RMaterialInstance>(matInstanceChildName);
    }
    WPtr<RMaterial> ogMaterial = ResourceManager::instance().get<RMaterial>(m_mesh->m_materialName);
    if (ogMaterial.lock()) {
      m_materialInstance.lock()->setMaterial(ogMaterial.lock());
    }
    // else {
    //   ogMaterial = ResourceManager::instance().get<RMaterial>("DefaultMaterial");
    //   if (ogMaterial.lock()) {
    //     m_materialInstance.lock()->setMaterial(ogMaterial.lock());
    //   }
    // }
  }
}

}
