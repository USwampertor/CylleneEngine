#pragma once

#include "cyCorePrerequisites.h"

#include <cyFileSystem.h>

#include "cyRResource.h"

namespace CYLLENE_SDK {

class CY_CORE_EXPORT RAsset : public RResource {
public:

  Path              m_filePointer;
  uint64            m_fileVersion;
  uint64            m_lastModify;
  RESOURCE_TYPE::E  m_type;
  size_t            m_dataSize;
  void*             m_data;
};

}
