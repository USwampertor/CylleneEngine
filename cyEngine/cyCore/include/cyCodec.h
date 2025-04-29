/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyCodec.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/03
 * @brief 	
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"

#include <cyFileSystem.h>
#include <cyLogger.h>
#include <cySmartPointers.h>

// #if CY_PLATFORM == CY_PLATFORM_WIN32
// #include <Windows.h>
// #undef min
// #undef max
// #endif
 
// #define IMGDEFAULTW 512
// #define IMGDEFAULTH 512
// #define IMGDEFAULTC 8*4
// 
// class aiNode;
// class aiScene;
// class aiMesh;
// 

namespace CYLLENE_SDK {

class ModelResource;
class MeshResource;

// class Resource;

class CY_CORE_EXPORT Codec
{
public:

  /**
   * Codec defining the type it is based on the resource it can load
   */
  Codec(const RESOURCE_TYPE::E& type) : m_type(type) {}

  virtual ~Codec() = default;

  static RESOURCE_TYPE::E 
  staticType() { 
    CY_ASSERT(true && "IMPLEMENT THIS");
    return RESOURCE_TYPE::E::eUNKNOWN;
  }

  const RESOURCE_TYPE::E& 
  getType() { return m_type; }

  bool
  canDecode(const String& path) {
    Path p(path);
    bool cd = canDecode(p);
    return cd;
  }

  bool
  canDecode(const Path& path) {
    String ext = path.extension().substr(1);
    return std::find(m_fileExtensions.begin(), 
                     m_fileExtensions.end(), 
                     ext) != m_fileExtensions.end();
  }

  void
  loadErrorMessage(const Path& pathToFile) {
    Logger::instance().logError(Utils::format("The file at path %s cannot be loaded", 
                                              pathToFile.fullPath()));
  }

  virtual void*
  decode(const File& f) = 0;

  // virtual SPtr<Resource>
  // load(const Path& pathToResource) = 0;
  // 
  // virtual SPtr<Resource>
  // create(const Path& pathToResource) = 0;
  // 
  // virtual SPtr<Resource>
  // create(const Path& pathToResource, void* data) = 0;

protected:
  
  Vector<String> m_fileExtensions;

  // Vector<String> m_engineExtensions;

private:
  
  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::E::eUNKNOWN;
};

  namespace IMGEXT {
  BETTER_ENUM(E, int32,
              UNKNOWN = -1,
              BMP     = 0,
              ICO     = 1,
              JPEG    = 2,
              JNG     = 3,
              KOALA   = 4,
              LBM     = 5,
              IFF     = LBM,
              MNG     = 6,
              PBM     = 7,
              PBMRAW  = 8,
              PCD     = 9,
              PCX     = 10,
              PGM     = 11,
              PGMRAW  = 12,
              PNG     = 13,
              PPM     = 14,
              PPMRAW  = 15,
              RAS     = 16,
              TARGA   = 17,
              TIFF    = 18,
              WBMP    = 19,
              PSD     = 20,
              CUT     = 21,
              XBM     = 22,
              XPM     = 23,
              DDS     = 24,
              GIF     = 25,
              HDR     = 26,
              FAXG3   = 27,
              SGI     = 28,
              EXR     = 29,
              J2K     = 30,
              JP2     = 31,
              PFM     = 32,
              PICT    = 33,
              RAW     = 34,
              WEBP    = 35,
              JXR     = 36);
  }

class CY_CORE_EXPORT ImageCodec : public Codec
{
public:


  ImageCodec();

  virtual ~ImageCodec() override;

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eIMAGE;
  }

  virtual void*
  decode(const File& f) override;
};

class CY_CORE_EXPORT ModelCodec : public Codec
{
public:

  ModelCodec() : Codec(ModelCodec::staticType()) {
    m_fileExtensions = { 
      "fbx",
      "dae",
      "gltf",
      "glb",
      "blend",
      "3ds",
      "ase",
      "obj",
      "ifc",
      "xgl",
      "zgl",
      "ply",
      "dxf",
      "lwo",
      "lws",
      "lxo",
      "stl",
      "x",
      "ac",
      "ms3d",
      "cob",
      "scn",
      "mdl",
      "mdl2",
      "md3",
      "pk3",
      "mdc",
      "md5",
      "smd",
      "vta",
      "ogex",
      "3d",
      "xml",
      "blend",
      "mesh"
    };
  }

  virtual ~ModelCodec() = default;

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMODEL;
  }

  virtual void*
  decode(const File& f) override;

};

class CY_CORE_EXPORT ShaderCodec : public Codec
{
public:

  ShaderCodec() : Codec(ShaderCodec::staticType()) {
    m_fileExtensions = {
      "txt",
      "hlsl",
      "glsl",
      "rqsl",
      "blob",
      "frag",
    };
  }

  virtual ~ShaderCodec() {

  }

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eSHADER;
  }

  virtual void*
  decode(const File& f) override;

};
  
class CY_CORE_EXPORT AudioCodec : public Codec
{
public:

  AudioCodec() : Codec(AudioCodec::staticType()) {
    m_fileExtensions = {
      "wav", 
      "mp3", 
      "wma", 
      "flac", 
      "aac"
    };
  }

  virtual ~AudioCodec() = default;

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eAUDIO;
  }

  virtual void*
  decode(const File& f) override;
};

}



