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
                eSHADER);
  }
  
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

  Resource(const Path& newFile, void* newData) {
    m_filePath = newFile;
    m_data = newData;
  }

  virtual ~Resource() {}

  virtual RESOURCE_TYPE::E
  getType() = 0;

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
  getIsDirty() { return isDirty; }

  void
  setIsDirty(const bool& newValue) { isDirty = newValue; }


  protected:

  void* m_data;

  Path m_filePath;

  bool isDirty;
};


class CY_CORE_EXPORT MeshResource : public Resource {
  
public:

  MeshResource(const Path& newFile, void* newData) {
    m_filePath = newFile;
    m_data = newData;
  }

  virtual RESOURCE_TYPE::E 
  getType() override { return RESOURCE_TYPE::E::eMODEL; }

};

class CY_CORE_EXPORT ImageResource : public Resource {

public:

  ImageResource(const Path& newFile, void* newData) {
    m_filePath = newFile;
    m_data = newData;
  }

  virtual RESOURCE_TYPE::E 
  getType() override { return RESOURCE_TYPE::E::eTEXTURE; }

};

}
