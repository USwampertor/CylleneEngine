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

  virtual Resource*
  load(Path pathToResource) = 0;

  Vector<String> m_fileExtensions;

  Vector<String> m_engineExtensions;

};

  
namespace IMGEXT {
  BETTER_ENUM(E, int32,
    UNKNOWN = -1,
    BMP = 0,
    ICO = 1,
    JPEG = 2,
    JNG = 3,
    KOALA = 4,
    LBM = 5,
    IFF = FIF_LBM,
    MNG = 6,
    PBM = 7,
    PBMRAW = 8,
    PCD = 9,
    PCX = 10,
    PGM = 11,
    PGMRAW = 12,
    PNG = 13,
    PPM = 14,
    PPMRAW = 15,
    RAS = 16,
    TARGA = 17,
    TIFF = 18,
    WBMP = 19,
    PSD = 20,
    CUT = 21,
    XBM = 22,
    XPM = 23,
    DDS = 24,
    GIF = 25,
    HDR = 26,
    FAXG3 = 27,
    SGI = 28,
    EXR = 29,
    J2K = 30,
    JP2 = 31,
    PFM = 32,
    PICT = 33,
    RAW = 34,
    WEBP = 35,
    JXR = 36);
}
class CY_CORE_EXPORT ImageCodec : public Codec
{
public:


  ImageCodec() {


    /*
  FIF_UNKNOWN = -1,
  FIF_BMP		= 0,
  FIF_ICO		= 1,
  FIF_JPEG	= 2,
  FIF_JNG		= 3,
  FIF_KOALA	= 4,
  FIF_LBM		= 5,
  FIF_IFF = FIF_LBM,
  FIF_MNG		= 6,
  FIF_PBM		= 7,
  FIF_PBMRAW	= 8,
  FIF_PCD		= 9,
  FIF_PCX		= 10,
  FIF_PGM		= 11,
  FIF_PGMRAW	= 12,
  FIF_PNG		= 13,
  FIF_PPM		= 14,
  FIF_PPMRAW	= 15,
  FIF_RAS		= 16,
  FIF_TARGA	= 17,
  FIF_TIFF	= 18,
  FIF_WBMP	= 19,
  FIF_PSD		= 20,
  FIF_CUT		= 21,
  FIF_XBM		= 22,
  FIF_XPM		= 23,
  FIF_DDS		= 24,
  FIF_GIF     = 25,
  FIF_HDR		= 26,
  FIF_FAXG3	= 27,
  FIF_SGI		= 28,
  FIF_EXR		= 29,
  FIF_J2K		= 30,
  FIF_JP2		= 31,
  FIF_PFM		= 32,
  FIF_PICT	= 33,
  FIF_RAW		= 34,
  FIF_WEBP	= 35,
  FIF_JXR		= 36
    */


    m_fileExtensions = {
      "png", 
      "jpg", 
      "jpeg", 
      "TIFF", 
      "TARGA",
      "gif"
    };
    // Should be called once
    FreeImage_Initialise();
  }

  virtual ~ImageCodec() = default;

  virtual RESOURCE_TYPE::E 
  getResource() override { return RESOURCE_TYPE::E::eTEXTURE; }

  virtual Resource*
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

  virtual Resource*
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

  virtual Resource*
  load(Path pathToResource) override;
};



}
