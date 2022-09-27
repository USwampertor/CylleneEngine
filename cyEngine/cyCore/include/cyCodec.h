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

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#if CY_PLATFORM == CY_PLATFORM_WIN32
#include <Windows.h>
#undef min
#undef max
#endif
#include <FreeImage/FreeImage.h>

#include <cyFileSystem.h>


namespace CYLLENE_SDK {

class CY_CORE_EXPORT Codec
{
public:

  Codec() = default;

  virtual ~Codec() = default;

  bool
  canDecode(const String& path) {
    Path p(path);
    return canDecode(p);
  }

  bool
  canDecode(const Path& path) {
    return std::find(m_fileExtensions.begin(), m_fileExtensions.end(), path.extension()) != m_fileExtensions.end();
  }

  virtual RESOURCE_TYPE::E 
  getResource() = 0;

  template<typename T>
  SharedPointer<T>
  load(Path pathToResource) = 0;

  Vector<String> m_fileExtensions;

  Vector<String> m_engineExtensions;

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


  ImageCodec() {
    for (auto extension : IMGEXT::E::_names()) {
      m_fileExtensions.push_back(extension);
    }

    // Should be called once
    FreeImage_Initialise();
  }

  virtual ~ImageCodec() = default;

  virtual RESOURCE_TYPE::E 
  getResource() override { return RESOURCE_TYPE::E::eTEXTURE; }

  template<typename T>
  SharedPointer<T>
  load(Path pathToResource) override;
};

class CY_CORE_EXPORT MeshCodec : public Codec
{
public:

  MeshCodec() {
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

  virtual ~MeshCodec() = default;

  virtual RESOURCE_TYPE::E
  getResource() override { return RESOURCE_TYPE::E::eMESH; }

  template<typename T>
  SharedPointer<T>
  load(Path pathToResource) override;
};

  
class CY_CORE_EXPORT AudioCodec : public Codec
{
public:

  AudioCodec() {
    m_fileExtensions = {
      "wav", 
      "mp3", 
      "wma", 
      "flac", 
      "aac"
    };
  }

  virtual ~AudioCodec() = default;

  virtual RESOURCE_TYPE::E 
  getResource() override { return RESOURCE_TYPE::E::eAUDIO; }

  template<typename T>
  SharedPointer<T>
  load(Path pathToResource) override;
};



}
