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
    return std::find(m_extensions.begin(), m_extensions.end(), path.extension()) != m_extensions.end();
  }

  virtual Pair<Path, Resource>
  load() = 0;

  virtual RESOURCE_TYPE::E 
  getResource() = 0;

  Vector<String> m_extensions;

};

  
class CY_CORE_EXPORT ImageCodec : public Codec
{
public:

  ImageCodec() {
    m_extensions = {
      "png", 
      "jpg", 
      "jpeg", 
      "TIFF", 
      "TARGA", 
      "gif"
    };
  }

  virtual ~ImageCodec() = default;

  virtual RESOURCE_TYPE::E 
  getResource() override { return RESOURCE_TYPE::E::eTEXTURE; }

};

class CY_CORE_EXPORT MeshCodec : public Codec
{
public:

  MeshCodec() {
    m_extensions = { 
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


};

  
class CY_CORE_EXPORT AudioCodec : public Codec
{
public:

  AudioCodec() {
    m_extensions = {
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

};



}
