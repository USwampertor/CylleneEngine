#include "cyCodec.h"
#include "cyDefaultPrimitives.h"
#include "cyImage.h"

namespace CYLLENE_SDK {

SharedPointer<Resource>
ImageCodec::load(Path pathToResource) {
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
ImageCodec::create(Path pathToResource) { 
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
ImageCodec::create(Path pathToResource, void* data) {
  return SharedPointer<Resource>(new ImageResource(pathToResource, data));
}

SharedPointer<Resource>
ModelCodec::load(Path pathToResource) {

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(pathToResource.fullPath(), 0);

  if (!scene) {
    return nullptr;
  }

  void* voidData;

  return SharedPointer<Resource>(new MeshResource(pathToResource, voidData));
}

SharedPointer<Resource>
ModelCodec::create(Path pathToResource) {
  return nullptr;
}

SharedPointer<Resource>
AudioCodec::load(Path pathToResource) {
  return nullptr;
}

}
