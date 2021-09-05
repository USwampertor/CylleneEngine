#include "cyDLLLoader.h"

#include "cyFileSystem.h"

namespace CYLLENE_SDK {

  typedef void* (*PLUGIN_FUNCTION)();
  void*
  DLLLoader::Load(String path, String functionName) {
    PLUGIN_FUNCTION t_api;

#if CY_PLATFORM == CY_PLATFORM_WIN32 
    char dirPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, dirPath);
    String folderPath(dirPath);
    folderPath = folderPath.append("\\").append(path);
    Path p(folderPath);
    HINSTANCE myDll = LoadLibrary(p.fullPath().c_str());
    if (!myDll) {
      DWORD err = GetLastError();
      std::cout << "Could not find dll at given path: " << path << std::endl;
      std::cout << err << std::endl;
      std::cout << "Press any key to continue...";
      return nullptr;
    }
    std::cout << "Loading " << path << "..." << std::endl;

    t_api = (PLUGIN_FUNCTION)GetProcAddress(myDll, functionName.c_str());
    if (!t_api) {

      CY_ASSERT(t_api == nullptr && "NO PLUGIN WITH GIVEN NAME");

      FreeLibrary(myDll);
      return nullptr;
    }
#elif

#endif
    return t_api();
  }
}