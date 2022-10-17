#pragma once

#include "cyCorePrerequisites.h"
#include "cyResource.h"
#include <cyModule.h>


namespace CYLLENE_SDK {

class Device;
class Codec;

class CY_CORE_EXPORT ResourceManager : public Module<ResourceManager>
{
 public:

  ResourceManager() = default;

  ~ResourceManager() = default;

  void
  init(Device* pDevice);

  template<typename T = Resource>
  SharedPointer<T>
  load(const String& path, bool& wasSuccesful);

  template<typename T = Resource>
  SharedPointer<T>
  create(const String& name, const RESOURCE_TYPE::E& type);

  template<typename T = Resource>
  SharedPointer<T>
  create(const String& name, const RESOURCE_TYPE::E& type, void* data);

  void
  createPrimitives();

  void
  flush();

  bool
  canDecode(const String& path);

  RESOURCE_TYPE::E
  getFormatType(const String& path);

  bool
  resourceExists(const Path& path);

  SharedPointer<Codec>
  getCodec(const RESOURCE_TYPE::E& resType);


  Map<uint32, SharedPointer<Resource>> m_resources;

  Vector<SharedPointer<Codec>> m_codecs;

};
}
