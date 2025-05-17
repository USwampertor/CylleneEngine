
#include <cyUnitTesting.h>

#include <cyGraphicsAPI.h>
#include <cyDLLLoader.h>
// Using namespace for ease of use
using namespace CYLLENE_SDK;


/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[])
{
  void* createFunc = CYLLENE_SDK::DLLLoader::load("cyDirectX11d.dll", "createPluginAPI", false);
  if (!createFunc) {
    // Handle error: DLL not found or function not exported
    return -1;
  }

  // Cast the function pointer to the correct type
  auto factory = reinterpret_cast<CYLLENE_SDK::GraphicsAPI * (*)()>(createFunc);

  // 2. Create the DX11 API instance
  CYLLENE_SDK::GraphicsAPI* dx11API = factory();
  if (!dx11API) {
    // Handle error: API creation failed
    return -1;
  }

  // 3. Set the module instance
  CYLLENE_SDK::GraphicsAPI::setModule(dx11API);

  // 4. Initialize with a window handle (HWND on Windows)
  // void* windowHandle = /* Get your platform-specific window handle */;
  // GraphicsAPI::instance().initialize(windowHandle);

  // Now use the API
  GraphicsAPI::instance().clear(CYLLENE_SDK::Color::GREEN);
  GraphicsAPI::instance().present();

  // ... rest of your rendering loop ...

  // Cleanup
  GraphicsAPI::shutDown();
  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}


