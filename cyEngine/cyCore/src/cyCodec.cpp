#include "cyCodec.h"
#include "cyDefaultPrimitives.h"
#include "cyImage.h"
#include "cyResource.h"
#include "cyResourceManager.h"

// Model decoding
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// Sound decoding


// Shader decoding


namespace CYLLENE_SDK {

  SharedPointer<Resource>
  TextureCodec::decode(const File& f) {
    
    Path p(f.path());

    int32 format = -1;

    for (IMGEXT::E v : IMGEXT::E::_values()) {
      String strExt = v._to_string();
      if (Utils::toLowerCase(p.extension()) == Utils::toLowerCase(strExt)) {
        
        format = v._to_integral();
        break;
      }
    }
    CY_ASSERT(format != -1 && "You should first check if this codec is able to decode a format");
    void* data = FreeImage_Load(static_cast<FREE_IMAGE_FORMAT>(format), f.path().c_str());

    // TODO: Extract file path starting from our project folder path so name includes this
    SharedPointer<TextureResource> newResource = 
      ResourceManager::instance().create<TextureResource>(p.baseName());
    newResource->setData(data);
    newResource->m_textureFormat = format;
    return REINTERPRETPOINTER(Resource, newResource);
    // return nullptr;
  }
  
  SharedPointer<Resource>
  ModelCodec::decode(const File& f) {

    Path p(f.path());

    SharedPointer<MeshResource> newResource = 
      ResourceManager::instance().create<MeshResource>(p.baseName());

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(f.path(), aiProcess_Triangulate |
                                                        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr) {
      return REINTERPRETPOINTER(MeshResource, newResource);
    }

    return REINTERPRETPOINTER(Resource, newResource);
    // return nullptr;
  }
  
  SharedPointer<Resource>
  ShaderCodec::decode(const File& f) {

    // Create a copy so there is no dangling pointers
    Path p(f.path());
    String tmp = f.readFile();
    
    // This is a temporal hack which SHOULD work
    void* data = reinterpret_cast<void*>(tmp.c_str()[0]);
    
    // auto tmpPointer = new ShaderResource(pathToResource, data);
    // return SharedPointer<Resource>(tmpPointer);
    // 
    // delete(data);

    SharedPointer<ShaderResource> newResource =
      ResourceManager::instance().create<ShaderResource>(p.baseName());

    newResource->m_isBlob = p.extension().compare(".blob") == 0;
    return REINTERPRETPOINTER(Resource, newResource);
  }
  
  SharedPointer<Resource>
  AudioCodec::decode(const File& f) {
    
    SharedPointer<AudioResource> newResource;
    return REINTERPRETPOINTER(Resource, newResource);
  }

// SharedPointer<Resource>
// TextureCodec::load(const Path& pathToResource) {
//   String noDot = pathToResource.extension().substr(1, pathToResource.extension().size() - 1);
//   IMGEXT::E converter = IMGEXT::E::_from_string_nocase(noDot.c_str());
//   FREE_IMAGE_FORMAT f = static_cast<FREE_IMAGE_FORMAT>( converter._to_index());
//     
//   FIBITMAP* data = FreeImage_Load(f, pathToResource.fullPath().c_str());
//   
//   FreeImage_GetWidth(data);
//   FreeImage_GetHeight(data);
//   FREE_IMAGE_TYPE type = FreeImage_GetImageType(data);
//   FreeImage_GetBPP(data);
//   Image<uint32> img;
//   // SharedPointer<ImageResource> img = std::static_pointer_cast<ImageResource>(Resource(pathToResource, FreeImage_GetBits(data)));
//   void* voidData = data->data;
// 
//   FreeImage_Unload(data);
// 
//   return SharedPointer<Resource>(new TextureResource(pathToResource, voidData));
// }
// 
// SharedPointer<Resource>
// TextureCodec::create(const Path& pathToResource) {
//   // Default initialization for images
//   FIBITMAP* data = FreeImage_Allocate(IMGDEFAULTW, IMGDEFAULTH, IMGDEFAULTC);
//   RGBQUAD color;
// 
//   if (!data) {
//     return SharedPointer<Resource>(new TextureResource(pathToResource, nullptr));
//   }
// 
// 
// 
//   for (int32 i = 0; i < IMGDEFAULTW; ++i) {
//     for (int32 j = 0; j < IMGDEFAULTH; ++j) {
//       // color.rgbRed      = DEFAULTPRIMS::defaultTexture[i][j * 0];
//       // color.rgbGreen    = DEFAULTPRIMS::defaultTexture[i][j * 1];
//       // color.rgbBlue     = DEFAULTPRIMS::defaultTexture[i][j * 2];
//       // color.rgbReserved = DEFAULTPRIMS::defaultTexture[i][j * 3];
//     
//       FreeImage_SetPixelColor(data, i, j, &color);
//     }
//   }
// 
//   void* voidData = data->data;
// 
//   FreeImage_Unload(data);
// 
//   return SharedPointer<Resource>(new TextureResource(pathToResource, data));
// }
// 
// SharedPointer<Resource>
// TextureCodec::create(const Path& pathToResource, void* data) {
//   return SharedPointer<Resource>(new TextureResource(pathToResource, data));
// }
// 
// SharedPointer<Resource>
// ModelCodec::load(const Path& pathToResource) {
// 
//   Assimp::Importer importer;
//   const aiScene* scene = importer.ReadFile(pathToResource.fullPath(), 0);
// 
//   if (!scene) {
//     return nullptr;
//   }
// 
//   void* voidData;
// 
//   return SharedPointer<Resource>(new MeshResource(pathToResource, voidData));
// }
// 
// SharedPointer<Resource>
// ModelCodec::create(const Path& pathToResource) {
//   return nullptr;
// }
// 
// SharedPointer<Resource>
// ModelCodec::create(const Path& pathToResource, void* data) {
//   return SharedPointer<Resource>(new TextureResource(pathToResource, data));
// }
// 
// 
// SharedPointer<Resource>
// ShaderCodec::load(const Path& pathToResource) {
// 
//   File shaderFile = FileSystem::open(pathToResource.fullPath());
// 
//   if (!shaderFile.exists()) { 
//     loadErrorMessage(pathToResource);
//     return nullptr; 
//   }
// 
//   // Create a copy so there is no dangling pointers
//   String tmp = shaderFile.readFile();
//   
//   // This is a temporal hack which SHOULD work
//   void* data = reinterpret_cast<void*>(tmp.c_str()[0]);
// 
//   // auto tmpPointer = new ShaderResource(pathToResource, data);
//   // tmpPointer->isBlob = pathToResource.extension().compare(".blob") == 0;
//   // return SharedPointer<Resource>(tmpPointer);
//   // 
//   // delete(data);
// 
//   return SharedPointer<Resource>(new ShaderResource(pathToResource, data));
// 
// }
// 
// 
// SharedPointer<Resource>
// ShaderCodec::create(const Path& pathToResource) {
//   return nullptr;
// }
// 
// SharedPointer<Resource>
// ShaderCodec::create(const Path& pathToResource, void* data) {
//   return SharedPointer<Resource>(new ShaderResource(pathToResource, data));
// }
// 
// SharedPointer<Resource>
// AudioCodec::load(const Path& pathToResource) {
//   return nullptr;
// }
// 
// 
// SharedPointer<Resource>
// AudioCodec::create(const Path& pathToResource) {
//   return nullptr;
// }
// 
// SharedPointer<Resource>
// AudioCodec::create(const Path& pathToResource, void* data) {
//   return SharedPointer<Resource>(new TextureResource(pathToResource, data));
// }

}
