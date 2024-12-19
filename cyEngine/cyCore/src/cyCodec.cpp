#include "cyCodec.h"

#include "cyAudio.h"
#include "cyDefaultPrimitives.h"
#include "cyImage.h"
#include "cyMesh.h"
#include "cyResource.h"
#include "cyResourceManager.h"
#include "cyShader.h"
#include "cyTexture.h"

// Model decoding
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// Sound decoding


// Shader decoding


namespace CYLLENE_SDK {

  void*
  TextureCodec::decode(const File& f) {
    
    Path p(f.path());

    int32 format = -1;

    for (IMGEXT::E v : IMGEXT::E::_values()) {
      String strExt = v._to_string();
      if (Utils::toLowerCase(p.extension().substr(1)) == Utils::toLowerCase(strExt)) {
        
        format = v._to_integral();
        break;
      }
    }
    
    CY_ASSERT(format != -1 && "You should first check if this codec is able to decode a format");

    
    TextureMetaData metadata;

    void* data                = FreeImage_Load(static_cast<FREE_IMAGE_FORMAT>(format), 
                                               f.path().c_str());
    metadata.m_width          = FreeImage_GetWidth(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_height         = FreeImage_GetHeight(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_bpp            = FreeImage_GetBPP(reinterpret_cast<FIBITMAP*>(data));
    metadata.m_textureFormat  = format;
    metadata.m_colorFormat    = FreeImage_GetColorType(reinterpret_cast<FIBITMAP*>(data));

    Vector<Color> colorData(metadata.m_height * metadata.m_width);

    for (uint32 y = 0; y < metadata.m_height; ++y) {
      for (uint32 x = 0; x < metadata.m_width; ++x) {
        // Get the pixel color
        RGBQUAD color;
        if (FreeImage_GetPixelColor(reinterpret_cast<FIBITMAP*>(data), x, y, &color)) {
          // Convert the pixel to Color (normalize values to [0, 1])
          float r = color.rgbRed / 255.0f;
          float g = color.rgbGreen / 255.0f;
          float b = color.rgbBlue / 255.0f;
          float a = (metadata.m_bpp == 32) ? color.rgbReserved / 255.0f : 1.0f; // Alpha if 32-bit

          colorData[y * metadata.m_width + x] = Color(r, g, b, a);
        }
        else {
          Utils::throwRuntimeError("Failed to retrieve pixel color.");
        }
      }
    }




    // Check for metadata
    
    // TODO: Extract file path starting from our project folder path so name includes this
    // SharedPointer<TextureResource> newResource = 
    //   ResourceManager::instance().create<TextureResource>(p.fullPath());

    /* Vector<Color> colors;
     * TextureMetaData metadata;
     * // Fill metadata
     * // Fill colors
     * newResource->setData(colors, metadata);
     * 
     * 
     */

    std::tuple<TextureMetaData, Vector<Color>>* tupleData = new std::tuple<TextureMetaData, Vector<Color>>(metadata, colorData);
    // newResource->setData(&tupleData);
    // newResource->m_metadata = metadata;
    // return REINTERPRETPOINTER(Resource, newResource);
    return reinterpret_cast<void*>(tupleData);
  }
  
  void*
  ModelCodec::decode(const File& f) {

    Path p(f.path());

    SharedPointer<MeshResource> newResource = 
      ResourceManager::instance().create<MeshResource>(p.baseName());

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(f.path(), aiProcess_Triangulate |
                                                        aiProcess_ConvertToLeftHanded);

    if (pScene == nullptr) {
      return nullptr;
      // return REINTERPRETPOINTER(MeshResource, newResource);
    }

    // return REINTERPRETPOINTER(Resource, newResource);
    return nullptr;
  }

  void
  ModelCodec::loadCylleneModel(const File& f) {
    // Loads our file type
  }

  
  void*
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
    // return REINTERPRETPOINTER(Resource, newResource);
    return nullptr;
  }
  
  void*
  AudioCodec::decode(const File& f) {
    
    SharedPointer<AudioResource> newResource;
    // return REINTERPRETPOINTER(Resource, newResource);
    return nullptr;
  }


}
