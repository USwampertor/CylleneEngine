#pragma once

#include "cyCorePrerequisites.h"
#include <cyModule.h>

namespace CYLLENE_SDK {

  namespace GFXTYPE
  {
  enum E
  {
    NONE,
    DX10,
    DX11,
    DX12,
    OGL2,
    OGL3,
    VLKN,
    APL2
  };
  }

class CY_CORE_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{
public:

  GraphicsAPI() = default;

};

// TODO: Check if this is correct
// extern CY_CORE_EXPORT UniquePointer<GraphicsAPI> g_GraphicsAPI;

}

