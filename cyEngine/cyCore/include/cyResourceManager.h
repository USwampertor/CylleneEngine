#pragma once

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
  create(String name, RESOURCE_TYPE::E type);

  void
  flush();

  bool
  canDecode(String path);

  Map<uint32, SharedPointer<Resource>> m_resources;

};
}
