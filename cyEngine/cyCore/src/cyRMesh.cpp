#include "cyRMesh.h"
#include "cyResourceManager.h"
#include "cyEngineAssets.h"

namespace CYLLENE_SDK {

RMesh::RMesh() : RResource(RMesh::staticType()) {
  // Assign default material from resource manager; create if missing.
  SPtr<RMaterial> defMat = ResourceManager::instance().get<RMaterial>("DefaultMaterial");
  if (!defMat) {
    defMat = ResourceManager::instance().create<RMaterial>("DefaultMaterial");
    void* data = const_cast<void*>(reinterpret_cast<const void*>(&DEFAULTMATERIALS::defaultMaterial));
    defMat->setData(data);
  }
  m_material = defMat;
}

void*
RMesh::getData() {
  return nullptr;
}

void
RMesh::setData(void* data) {

}


}
