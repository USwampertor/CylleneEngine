#pragma once

#include "cyCorePrerequisites.h"


#include "cyResource.h"
#include "cyCodec.h"
#include "cyTexture.h"

#include <cyEvent.h>
#include <cyJSON.h>
#include <cyModule.h>
#include <cySmartPointers.h>
#include <cyUtilities.h>
#include <cyLogger.h>

namespace CYLLENE_SDK {

class Device;
class Codec;

class CY_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:

  ResourceManager() = default;

  ~ResourceManager() = default;

  virtual void
  onStartUp() override;

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
  String 
  generateResourceID(const String& assetName) {
    RESOURCE_TYPE::E type = T::staticType();
    String realName = Utils::format("%s_%s", type._to_string(), assetName.c_str());
    return realName;
  }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
  SharedPointer<T> 
  create(const String& assetPath) {
    String realName = generateResourceID<T>(assetPath);
    if (m_resources.find(Hash<String>()(realName)) != m_resources.end()) {
      return REINTERPRETPOINTER(T, m_resources.at(Hash<String>()(realName)));
    }
    SharedPointer<T> newResource = makeSharedPtr<T>();
    newResource->m_name = assetPath;
    m_resources.insert(Utils::makePair(Hash<String>()(realName), newResource));
    return newResource;
  }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
  SharedPointer<T>
  loadFromPath(const String& assetPath) {

    File f = FileSystem::open(assetPath);

    if (!f.exists()) {
      // Throw error as this file does not even exist
      // Better create a new resource in that case
      return nullptr;
    }

    RESOURCE_TYPE::E type = T::staticType();

    SharedPointer<Codec> codec;
    codec = getCodec<Codec>(type);
    
    if (!codec->canDecode(assetPath)) {
      // Throw error as this type of file is not compatible with the resource
      // being instanced
      Logger::instance().logError(Utils::format("Cannot decode %s as resource %s", 
                                                assetPath.c_str(), 
                                                type._to_string()));
      return nullptr;
    }

    //TODO: Check for projectDir root and make paths relative
    // i.e C:/Foo/Bar/image.png -> /ProjectDir/SelectedFolder/image.png

    SharedPointer<TextureResource> newResource = create<TextureResource>(assetPath);
    newResource->setData(codec->decode(f));
    return REINTERPRETPOINTER(T, newResource);
  }

  template<typename T,
           typename = std::enable_if_t<std::is_base_of<Resource, T>::value>,
           typename... Args>
  SharedPointer<T>
  loadFromMemory(Args...) {

  }

  

  void 
  deserialize(const JSONValue& resources) {
    
  }

  JSONDocument 
  serialize() {
    JSONDocument d;
    return d;
  }

  // template<typename T, 
  //          typename = std::enable_if_t<std::is_base_of<Codec, T>::value>>
  // SharedPointer<T>&
  // getCodec() {
  //   RESOURCE_TYPE::E type = T::staticType();
  // 
  //   if (m_codecs.find(type._to_string()) != m_codecs.end()) {
  //     return REINTERPRETPOINTER(T, m_codecs[type._to_string()]);
  //   }
  // 
  // }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Codec, T>::value>>
  SharedPointer<T>
  getCodec(const RESOURCE_TYPE::E& type) {

    if (m_codecs.find(type._to_string()) != m_codecs.end()) {
      return REINTERPRETPOINTER(T, m_codecs[type._to_string()]);
    }

  }


//   void
//   init(/*Device* pDevice*/);
// 
//   template<typename T = Resource>
//   SharedPointer<T>
//   load(const String& path, bool& wasSuccesful);
// 
//   template<typename T = Resource>
//   SharedPointer<T>
//   create(const String& name, const RESOURCE_TYPE::E& type);
// 
//   template<typename T = Resource>
//   SharedPointer<T>
//   create(const String& name, const RESOURCE_TYPE::E& type, void* data);
// 
//   void
//   createPrimitives();
// 
//   void
//   flush();
// 
//   bool
//   canDecode(const String& path);
// 
//   RESOURCE_TYPE::E
//   getFormatType(const String& path);
// 
//   bool
//   resourceExists(const Path& path);
// 
//   SharedPointer<Codec>
//   getCodec(const RESOURCE_TYPE::E& resType);


  Map<SizeT, SharedPointer<Resource>> m_resources;

  Map<String, SharedPointer<Codec>> m_codecs;

  Event<void> m_resourceLoaded;

  Event<void> m_resourceCreated;

  Event<void> m_resourcesSerialized;

  Event<void> m_resourcesDeserialized;
};
}
