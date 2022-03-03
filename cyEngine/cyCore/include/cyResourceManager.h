#pragma once

#include "cyCorePrerequisites.h"

#include <cyModule.h>


namespace CYLLENE_SDK {

class Resource;

class ResourceManager : Module<ResourceManager>
{
 public:

  ResourceManager() = default;

  ~ResourceManager() = default;

  bool
  init();

};
}
