#pragma once

#include "cyCorePrerequisites.h"
#include <cyFileSystem.h>


namespace CYLLENE_SDK {

  namespace RESOURCE_TYPE {
    BETTER_ENUM(E, uint32, 
      eNONE,
      eMESH, 
      eAUDIO, 
      eTEXTURE,
      eSHADER);
  }
  
class CY_CORE_EXPORT Resource {
public:
  Resource() = default;

  virtual ~Resource() {}

  virtual RESOURCE_TYPE::E
  getType() = 0;

  const void* 
  getData() { return m_data; }

  const String& 
  getName() { return m_filePath.baseName(); }
  
  const String&
  getExtension() { return m_filePath.extension(); }
  
  const Path&
  getPath() { return m_filePath; }

private:
  void* m_data;

  Path m_filePath;
};
}
