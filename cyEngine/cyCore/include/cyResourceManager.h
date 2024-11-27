#pragma once

#include "cyCorePrerequisites.h"

#include "cyResource.h"

#include <cyEvent.h>
#include <cyJSON.h>
#include <cyModule.h>
#include <cySmartPointers.h>
#include <cyUtilities.h>

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
  SharedPointer<T> 
  create(const String& assetName) {
    RESOURCE_TYPE::E type = T::staticType();
    String realName = Utils::format("%s_%s", type._to_string(), assetName.c_str());
    if (m_resources.find(Hash<String>()(realName)) != m_resources.end()) {
      return REINTERPRETPOINTER(T, m_resources.at(Hash<String>()(realName)));
    }
    SharedPointer<T> newResource = makeSharedPtr<T>();
    newResource->m_name = assetName;
    m_resources.insert(Utils::makePair(Hash<String>()(realName), newResource));
    return newResource;
  }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Resource, T>::value>>
  SharedPointer<T>
  load(const String& assetPath) {
    RESOURCE_TYPE::E type = T::staticType();

    if (RESOURCE_TYPE::E::eMODEL == type) {

    }

  }

  void 
  deserialize(const JSONValue& resources) {
    
  }

  JSONDocument 
  serialize() {
    JSONDocument d;
    return d;
  }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Codec, T>::value>>
  UniquePointer<T>&
  getCodec() {
    RESOURCE_TYPE::E type = T::staticType();

    for (int i = 0; i < m_codecs.size(); ++i) {
      if (m_codecs[i]->getType() == type) {
        return REINTERPRETPOINTER(T, m_codecs[i]);
      }
    }
  }

  template<typename T, 
           typename = std::enable_if_t<std::is_base_of<Codec, T>::value>>
  UniquePointer<T>&
  getCodec(const RESOURCE_TYPE::E& type) {
    for (int i = 0; i < m_codecs.size(); ++i) {
      if (m_codecs[i]->getType() == type) {
        return REINTERPRETPOINTER(T, m_codecs[i]);
      }
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

  Vector<UniquePointer<Codec>> m_codecs;

  Event<void> m_resourceLoaded;

  Event<void> m_resourceCreated;

  Event<void> m_resourcesSerialized;

  Event<void> m_resourcesDeserialized;
};
}
