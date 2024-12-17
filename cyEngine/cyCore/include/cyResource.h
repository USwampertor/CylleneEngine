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

// Forward declaration
class ResourceManager;


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

  Resource(const Path& newFile)
    : m_filePath(newFile),
      m_isDirty(false) {}

  Resource(const RESOURCE_TYPE::E& type)
    : m_type(type),
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

  // const void* 
  // getData() { return m_data; }

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

  /**
   * template<typename Args...> 
   * void 
   * setData(Args...) = 0;
   * 
   * // TextureResource
   * void
   * setData(Args...) override {
   *  // Assert there is colors and metadata in that order
   *  // m_colors = colors;
   *  // m_metadata = metadata;
   * }
   * 
   * 
   * i.e. implementation
   * // foo.cpp
   * Ptr<TextureResource> r = MakePtr<TextureResource>;
   * Ptr<TextureResource> r = ResourceManager::instance().create<TextureResource>();
   * r.setData(MyDataBuffer, myDataMetadata);
   * 
   */

  // virtual void
  // fromMemory(void*) = 0;

  virtual void
  setData(void* data) = 0;

  friend class ResourceManager;

 protected:

  Path m_filePath;

  String m_name;

  bool m_isDirty;

  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::E::eUNKNOWN;
};

}
