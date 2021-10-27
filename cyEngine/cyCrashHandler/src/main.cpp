
#include "cyCrashHandlerPrerequisites.h"
#include "cyGraphicsUI.h"
#include "cyUtilities.h"
#define DIRECTX11 1;

#if CY_PLATFORM == CY_PLATFORM_WIN32
# ifdef DIRECTX11
#   include "cyDirectx11UI.h"
# endif // DIRECTX11
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

using namespace CYLLENE_SDK;

int main(int argc, char* argv[]) {

  try
  {
#if CY_PLATFORM == CY_PLATFORM_WIN32
# ifdef DIRECTX11
  GraphicsUI::startUp<Directx11UI>();
  ImGuiIO a;
    if (!GraphicsUI::instance().init(a, nullptr)) {
      String error("Could not generate Instance");
      Utils::ThrowException(error);
    }
# endif // DIRECTX11
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif
  }
  catch (const std::exception& e)
  {

  }

  return 0;
  
}
