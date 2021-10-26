
#include "cyCrashHandlerPrerequisites.h"
#include "cyGraphicsUI.h"

#define DIRECTX11 1;

#if CY_PLATFORM == CY_PLATFORM_WIN32
# ifdef DIRECTX11
#   include "cyDirectx11UI.h"
# endif // DIRECTX11
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

using namespace CYLLENE_SDK;

int main(int argc, char* argv[]) {
  GraphicsUI* ui;
#if CY_PLATFORM == CY_PLATFORM_WIN32
# ifdef DIRECTX11
  ui = new Directx11UI();
  ImGuiIO a;
  ui->init(a, nullptr);
# endif // DIRECTX11
#elif CY_PLATFORM == CY_PLATFORM_LINUX

#endif

  return 0;
  
}
