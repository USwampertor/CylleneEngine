
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "cyCodec.h"
#include "cyResourceManager.h"
#include "cyWindow.h"

namespace CYLLENE_SDK {
  void
  ResourceManager::init(Device* pDevice) {
    auto imageCodec = std::make_unique<ImageCodec>();
    m_codecs.push_back(std::move(imageCodec));
    auto meshCodec = std::make_unique<ModelCodec>();
    m_codecs.push_back(std::move(meshCodec));
    

  }

  template<typename T>
  SharedPointer<T>
  ResourceManager::load(const String& path, bool& wasSuccesful) {
    // return nullptr;


    // End it quickly, if it is not a valid format, just create a nullptr
    if (!canDecode(path)) {
      return nullptr;
    }

    Path p(path);
    bool isReloading = false;
    // Check if the file is already in the resource manager
    if (resourceExists(p)) {
      // Send warning if the user wants to reload the resource or just leave it as is
      // This can be interesting when you change the file in another app
      int result = 0; // Show message box 
      if (0 == result) {
        return nullptr;
      }
      else {
        isReloading = true;
      }
    }

    auto type = getFormatType(p.extension());
    int32 hashValue = std::hash<String>{}(p.fullPath());
    // Check first if the file is actually valid for the engine
    File f;
    
    if (RESOURCE_TYPE::E::eUNKNOWN == type) {
      WindowManager::ShowErrorMessage("Resource Error",
        Utils::format("There was an error loading this resource: %s", p.fullPath()));
      return nullptr;
    }
    else {
      f = FileSystem::open(p.fullPath());
    }

    SharedPointer<T> newResource = std::static_pointer_cast<T>(getCodec(type)->load(p));

    if (!isReloading) {
      m_resources.insert(std::make_pair(hashValue, newResource));
    }
    else {
      m_resources[hashValue] = newResource;
    }

    return newResource;
  }

  template<typename T>
  SharedPointer<T>
  ResourceManager::create(const String& name, const RESOURCE_TYPE::E& type ) {
    SharedPointer<T> newResource = std::static_pointer_cast<T>(getCodec(type)->create(name));
    return newResource;
  }

  template<typename T>
  SharedPointer<T>
  ResourceManager::create(const String& name, const RESOURCE_TYPE::E& type, void* data ) {
    SharedPointer<T> newResource = std::static_pointer_cast<T>(getCodec(type)->create(name));
    return newResource;
  }

  void
  ResourceManager::createPrimitives() {
    // Create 3D Objects
    create<MeshResource>("primitives/Box", RESOURCE_TYPE::E::eMODEL);
    create<MeshResource>("primitives/Sphere", RESOURCE_TYPE::E::eMODEL);
    create<MeshResource>("primitives/Plane", RESOURCE_TYPE::E::eMODEL);
    create<MeshResource>("primitives/Cylinder", RESOURCE_TYPE::E::eMODEL);

    create<MeshResource>("primitives/defaultTexture", RESOURCE_TYPE::E::eTEXTURE);


  }

  void
  ResourceManager::flush() {

  }

  bool
  ResourceManager::canDecode(const String& path) {
    Path p(path);
    for (int i = 0; i < m_codecs.size(); ++i) {
      if (m_codecs[i]->canDecode(p)) { return true; }
    }
    return false;
  }

  RESOURCE_TYPE::E 
  ResourceManager::getFormatType(const String& path) {
    Path p(path);
    for (int i = 0; i < m_codecs.size(); ++i) {
      if (m_codecs[i]->canDecode(p)) { return m_codecs[i]->getResource(); }
    }
    return RESOURCE_TYPE::E::eUNKNOWN;
  }

  bool
  ResourceManager::resourceExists(const Path& hashValue) {
    int32 h = std::hash<String>{}(hashValue.fullPath());

    return m_resources.find(h) != m_resources.end();
  }

  // void
  // processNode(aiNode* node, const aiScene* scene) {
  //   uint32 meshNum = 0;
  //   for (meshNum = 0; meshNum < scene->mNumMeshes; ++meshNum) {
  //     aiMesh* mesh = scene->mMeshes[node->mMeshes[meshNum]];
  //   }
  // }

  SharedPointer<Codec>
  ResourceManager::getCodec(const RESOURCE_TYPE::E& resType) {
    for (int32 i = 0; i < m_codecs.size(); ++i)
    {
      if (resType == m_codecs[i]->getResource()) {
        return m_codecs[i];
      }
    }
    return nullptr;
  }


}
