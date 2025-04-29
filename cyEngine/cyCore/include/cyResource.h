#pragma once

#include "cyCorePrerequisites.h"
#include <cyFileSystem.h>

namespace CYLLENE_SDK {

  namespace RESOURCE_TYPE {
    BETTER_ENUM(E, uint32, 
                eUNKNOWN,
                eANIMATION,
                eATLAS,
                eAUDIO, 
                eFONT,
                eIMAGE,
                eMATERIAL,
                eMESH,
                eMODEL,
                eSCENE,
                eSCRIPT,
                eSHADER,
                eTEXTURE);
  }

// Forward declaration
class ResourceManager;
class Codec;
class ModelCodec;


/**
 * Resource
 * Description:
 * This is the object that will store information as in-engine object
 * Sample usage:
 *
 */
class CY_CORE_EXPORT Resource {
 public:

  /**
   * Default constructor
   */
  Resource() = default;

  /*
   *	@brief Resource defining the type of object this can be
   *	@param const RESOURCE_TYPE::E& type the type the resource is
   *  @return	
   */
  Resource(const RESOURCE_TYPE::E& type)
    : m_type(type),
      m_isDirty(false) {}

  /*
   *	@brief	
   *	@param		
   *  @return	
   */
  Resource(const RESOURCE_TYPE::E& type, const Path& path)
    : m_type(type),
      m_isDirty(false),
      m_filePath(path),
      m_name(path.fullPath()) {}

  /**
   * virtual destructor
   */
  virtual ~Resource() {}

  /*
   *	@brief  a static function to get the type a resource is as RESOURCE_TYPE::E enum	
   *  @return	a RESOURCE_TYPE::E implemented by each resource
   */
  static RESOURCE_TYPE::E 
  staticType() { 
    CY_ASSERT(true && "IMPLEMENT THIS");  
    return RESOURCE_TYPE::E::eUNKNOWN;
  }

  /*
   *	@brief	returns the data from the resource
   *  @return	the data returned as void* blob of data
   */
  virtual void*
  getData() = 0;

  /*
   *	@brief	sets the data from memory into a resource
   *	@param	void* the blob of data needed in the resource. 
   *          Each resource implements what to insert
   */
  virtual void
  setData(void* data) = 0;

  /*
   *	@brief	returns the type the resource is as a non-static function
   *  @return	RESOURCE_TYPE of the instanced object
   */
  const RESOURCE_TYPE::E& getType() { return m_type; }

  /*
   *	@brief	Gets the name of the resource
   *  @return	the name of the resource
   */
  const String& 
  getName() { return m_filePath.baseName(); }
  
  /*
   *	@brief	Returns the extension the resource is as a file 
   *  @return	String with the extension the file is beginning with "."
   */
  const String&
  getExtension() { return m_filePath.extension(); }
  
  /*
   *	@brief	returns the path the resource is located within the project
   *  @return	the path of the object with the project as root
   */
  const Path&
  getPath() { return m_filePath; }

  /*
   *	@brief	if the object has beed modified, this should return true
   *  @return	true if the value was modified, false if not
   */
  const bool
  getIsDirty() { return m_isDirty; }

  /*
   *	@brief	set if the data was modified
   *	@param	const bool& newValue true if there was a change in the resource
   */
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

  /**
   * Resource Manager can access the protected data
   */
  friend class ResourceManager;

  friend class Codec;
  friend class ModelCodec;

 protected:

  /**
   * The path of the object relative to the projects root dir
   */
  Path m_filePath;

  /**
   * The name of the resource
   */
  String m_name;

  /**
   * if there was any change and should be reimported or checked
   */
  bool m_isDirty;

  /**
   * The type of resource this object is
   */
  RESOURCE_TYPE::E m_type = RESOURCE_TYPE::E::eUNKNOWN;
};

}
