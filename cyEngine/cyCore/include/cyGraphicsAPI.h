#pragma once

#include "cyCorePrerequisites.h"
#include <cyModule.h>

namespace CYLLENE_SDK {

class CY_CORE_EXPORT GraphicsAPI : public Module<GraphicsAPI>
{

};

extern CY_CORE_EXPORT UniquePointer<GraphicsAPI> g_GraphicsAPI;

}

