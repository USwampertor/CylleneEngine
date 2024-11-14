#pragma once

#include "cyCorePrerequisites.h"
#include <cyFileSystem.h>


namespace CYLLENE_SDK {

  namespace RESOURCE_TYPE {
    BETTER_ENUM(E, uint32, 
                eUNKNOWN,
                eMODEL, 
                eAUDIO, 
                eTEXTURE,
                eSHADER,
                eATLAS,
                eFONT,
                eSCENE,
                eANIMATION);
  }

class ResourceManager;


// TODO: Maybe move each resource to its corresponding file

/**
 * Resource
 * Description:
 * This is the object that will store information as in-engine object
 * Sample usage:
 *
 */
class CY_CORE_EXPORT Resource {
 public:
  Resource() = default;

  Resource(const Path& newFile, void* newData)
    : m_filePath(newFile),
      m_data(newData),
      m_isDirty(false) {}

  Resource(const RESOURCE_TYPE::E& type)
    : m_type(type),
      m_data(nullptr),
      m_isDirty(false) {}

  virtual ~Resource() {}

  static RESOURCE_TYPE::E 
  staticType() { 
    CY_ASSERT(true && "IMPLEMENT THIS");  
    return RESOURCE_TYPE::E::eUNKNOWN;
  }

  const RESOURCE_TYPE::E& getType() { return m_type; }

  // virtual void
  // initialize() = 0;

  const void* 
  getData() { return m_data; }

  const String& 
  getName() { return m_filePath.baseName(); }
  
  const String&
  getExtension() { return m_filePath.extension(); }
  
  const Path&
  getPath() { return m_filePath; }

  const bool
  getIsDirty() { return m_isDirty; }

  void
  setIsDirty(const bool& newValue) { m_isDirty = newValue; }

  void
  setData(void* data) {
    m_data = data;
  }

  friend class ResourceManager;

 protected:

  void* m_data;

  Path m_filePath;

  String m_name;

  bool m_isDirty;

  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::E::eUNKNOWN;
};


class CY_CORE_EXPORT MeshResource : public Resource {
  
public:

  MeshResource() : Resource(MeshResource::staticType()) {}
  
  MeshResource(const Path& newFile, void* newData) 
    : Resource(newFile, newData) {
    m_type = MeshResource::staticType();
  }

  static RESOURCE_TYPE::E 
  staticType() { 
    return RESOURCE_TYPE::E::eMODEL; 
  }

};

class CY_CORE_EXPORT TextureResource : public Resource {

public:

  TextureResource() : Resource(TextureResource::staticType()) {}

  TextureResource(const Path& newFile, void* newData)
    : Resource(newFile, newData) {
    m_type = TextureResource::staticType();
  }

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eTEXTURE; 
  }
public:
  uint32 m_textureFormat;
  uint32 m_width;
  uint32 m_height;
  uint32 m_colorFormat;
};

class CY_CORE_EXPORT ShaderResource : public Resource {

public:

  ShaderResource() : Resource(ShaderResource::staticType()) {}

  ShaderResource(const Path& newFile, void* newData)
    : Resource(newFile, newData), m_isBlob(false) {
    m_type = ShaderResource::staticType();
  }

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eSHADER;
  }
  bool m_isBlob;
};


class CY_CORE_EXPORT AudioResource : public Resource {

public:

  AudioResource() : Resource(AudioResource::staticType()) {}

  AudioResource(const Path& newFile, void* newData)
    : Resource(newFile, newData) {
    m_type = AudioResource::staticType();
  }

  static RESOURCE_TYPE::E staticType() {
    return RESOURCE_TYPE::E::eAUDIO;
  }
};

}
