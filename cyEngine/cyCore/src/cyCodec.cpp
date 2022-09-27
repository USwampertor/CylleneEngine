#include "cyCodec.h"

namespace CYLLENE_SDK {

  template<typename T>
  SharedPointer<T>
  ImageCodec::load(Path pathToResource) {
    String noDot = pathToResource.extension().substr(1, pathToResource.extension().size() - 1);
    IMGEXT::E converter = IMGEXT::E::_from_string_nocase(noDot.c_str());
    FREE_IMAGE_FORMAT f = static_cast<FREE_IMAGE_FORMAT>( converter._to_index());
    
    FIBITMAP* data = FreeImage_Load(f, pathToResource.fullPath().c_str());
    ImageResource* img = new ImageResource(pathToResource, FreeImage_GetBits(data));

    FreeImage_Unload(data);

    return img;
  }

  template<typename T>
  SharedPointer<T>
  MeshCodec::load(Path pathToResource) {

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathToResource.fullPath(), 0);

    if (!scene) {
      return nullptr;
    }

    MeshResource* mesh = new MeshResource();
    return mesh;
  }


  Resource*
  AudioCodec::load(Path pathToResource) {
    return nullptr;
  }

}