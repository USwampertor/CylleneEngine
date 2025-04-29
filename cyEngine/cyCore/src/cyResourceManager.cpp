
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "cyCodec.h"
#include "cyResourceManager.h"
#include "cyWindow.h"

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
}

}
