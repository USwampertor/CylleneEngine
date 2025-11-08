
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "cyCodec.h"
#include "cyResourceManager.h"
#include "cyWindow.h"
#include "cyRMaterial.h"
#include "cyRShader.h"
#include "cyEngineAssets.h"

namespace CYLLENE_SDK {


void
ResourceManager::onStartUp() {
  // Create Engine Icons and other default resources
  m_codecs.insert(Utils::makePair(ModelCodec::staticType()._to_string(),    
                                  makeSharedPtr<ModelCodec>()));
  m_codecs.insert(Utils::makePair(ImageCodec::staticType()._to_string(),  
                                  makeSharedPtr<ImageCodec>()));
  m_codecs.insert(Utils::makePair(ShaderCodec::staticType()._to_string(),   
                                  makeSharedPtr<ShaderCodec>()));
  m_codecs.insert(Utils::makePair(MaterialCodec::staticType()._to_string(),   
                                  makeSharedPtr<MaterialCodec>()));

  // Default resources (in-memory)
  {
    // Default shader
    SPtr<RShader> defShader = create<RShader>("DefaultShader");
    void* shaderData = const_cast<void*>(reinterpret_cast<const void*>(&DEFAULTSHADERS::defaultShader));
    defShader->setData(shaderData);
  }

  {
    // Default material (references DefaultShader)
    SPtr<RMaterial> defMaterial = create<RMaterial>("DefaultMaterial");
    void* materialData = const_cast<void*>(reinterpret_cast<const void*>(&DEFAULTMATERIALS::defaultMaterial));
    defMaterial->setData(materialData);
  }
}

}
