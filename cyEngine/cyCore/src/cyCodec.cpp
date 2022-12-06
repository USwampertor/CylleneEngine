#include "cyCodec.h"
#include "cyDefaultPrimitives.h"
#include "cyImage.h"

namespace CYLLENE_SDK {

SharedPointer<Resource>
ImageCodec::load(const Path& pathToResource) {
  String noDot = pathToResource.extension().substr(1, pathToResource.extension().size() - 1);
  IMGEXT::E converter = IMGEXT::E::_from_string_nocase(noDot.c_str());
  FREE_IMAGE_FORMAT f = static_cast<FREE_IMAGE_FORMAT>( converter._to_index());
    
  FIBITMAP* data = FreeImage_Load(f, pathToResource.fullPath().c_str());
  
  FreeImage_GetWidth(data);
  FreeImage_GetHeight(data);
  FREE_IMAGE_TYPE type = FreeImage_GetImageType(data);
  FreeImage_GetBPP(data);
  Image<uint32> img;
  // SharedPointer<ImageResource> img = std::static_pointer_cast<ImageResource>(Resource(pathToResource, FreeImage_GetBits(data)));
  void* voidData = data->data;

  FreeImage_Unload(data);

  return SharedPointer<Resource>(new ImageResource(pathToResource, voidData));
}

SharedPointer<Resource>
ImageCodec::create(const Path& pathToResource) {
  // Default initialization for images
  FIBITMAP* data = FreeImage_Allocate(IMGDEFAULTW, IMGDEFAULTH, IMGDEFAULTC);
  RGBQUAD color;

  if (!data) {
    return SharedPointer<Resource>(new ImageResource(pathToResource, nullptr));
  }



  for (int32 i = 0; i < IMGDEFAULTW; ++i) {
    for (int32 j = 0; j < IMGDEFAULTH; ++j) {
      color.rgbRed      = DEFAULTPRIMS::defaultTexture[i][j * 0];
      color.rgbGreen    = DEFAULTPRIMS::defaultTexture[i][j * 1];
      color.rgbBlue     = DEFAULTPRIMS::defaultTexture[i][j * 2];
      color.rgbReserved = DEFAULTPRIMS::defaultTexture[i][j * 3];
    
      FreeImage_SetPixelColor(data, i, j, &color);
    }
  }

  void* voidData = data->data;

  FreeImage_Unload(data);

  return SharedPointer<Resource>(new ImageResource(pathToResource, data));
}

SharedPointer<Resource>
ImageCodec::create(const Path& pathToResource, void* data) {
  return SharedPointer<Resource>(new ImageResource(pathToResource, data));
}

SharedPointer<Resource>
ModelCodec::load(const Path& pathToResource) {

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(pathToResource.fullPath(), 0);

  if (!scene) {
    return nullptr;
  }

  void* voidData;

  return SharedPointer<Resource>(new MeshResource(pathToResource, voidData));
}

SharedPointer<Resource>
ModelCodec::create(const Path& pathToResource) {
  return nullptr;
}

SharedPointer<Resource>
ModelCodec::create(const Path& pathToResource, void* data) {
  return SharedPointer<Resource>(new ImageResource(pathToResource, data));
}


SharedPointer<Resource>
ShaderCodec::load(const Path& pathToResource) {

  File shaderFile = FileSystem::open(pathToResource.fullPath());

  if (!shaderFile.exists()) { 
    loadErrorMessage(pathToResource);
    return nullptr; 
  }

  // Create a copy so there is no dangling pointers
  String tmp = shaderFile.readFile();
  
  // This is a temporal hack which SHOULD work
  void* data = reinterpret_cast<void*>(tmp.c_str()[0]);

  // auto tmpPointer = new ShaderResource(pathToResource, data);
  // tmpPointer->isBlob = pathToResource.extension().compare(".blob") == 0;
  // return SharedPointer<Resource>(tmpPointer);
  // 
  // delete(data);

  return SharedPointer<Resource>(new ShaderResource(pathToResource, data));

}


SharedPointer<Resource>
ShaderCodec::create(const Path& pathToResource) {
  return nullptr;
}

SharedPointer<Resource>
ShaderCodec::create(const Path& pathToResource, void* data) {
  return SharedPointer<Resource>(new ShaderResource(pathToResource, data));
}

SharedPointer<Resource>
AudioCodec::load(const Path& pathToResource) {
  return nullptr;
}


SharedPointer<Resource>
AudioCodec::create(const Path& pathToResource) {
  return nullptr;
}

SharedPointer<Resource>
AudioCodec::create(const Path& pathToResource, void* data) {
  return SharedPointer<Resource>(new ImageResource(pathToResource, data));
}

}
