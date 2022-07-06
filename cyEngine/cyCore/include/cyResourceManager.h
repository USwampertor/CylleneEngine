#pragma once

#include "cyCodec.h"
#include "cyCorePrerequisites.h"
#include "cyResource.h"
#include <cyModule.h>


namespace CYLLENE_SDK {

class Device;

class ResourceManager : Module<ResourceManager>
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
  create(const String& name);

  void
  flush();

  bool
  canDecode(const String& path);

  RESOURCE_TYPE::E
  getFormatType(const String& path);

  bool
  resourceExists(const Path& path);

  Map<uint32, SharedPointer<Resource>> m_resources;

  Vector<UniquePointer<Codec>> m_codecs;

};
}
