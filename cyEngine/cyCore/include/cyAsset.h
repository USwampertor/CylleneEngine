#pragma once

#include "cyCorePrerequisites.h"

#include <cyFileSystem.h>

#include "cyResource.h"

namespace CYLLENE_SDK {

  namespace FILE_TYPE {
    BETTER_ENUM(E, uint32,
                eNONE,
                eUNSUPPORTED,
                eMODEL,
                eSHADER);
  }


class CY_CORE_EXPORT Asset : public Resource {
public:

  Path          m_filePointer;
  uint64        m_fileVersion;
  uint64        m_lastModify;
  FILE_TYPE::E  m_type;
  size_t        m_dataSize;
  void*         m_data;

  static const String& extension;
};

}
