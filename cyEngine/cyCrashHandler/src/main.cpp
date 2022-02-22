/*********************************************/
/*
 * @file 	main
 * @author	Marco "Swampertor" Millan
 * @date	24/01/2022
 * @brief	
 *
 */
/******************************************** */
#include "cyCrashHandlerPrerequisites.h"
#include "cyCrashHandlerUI.h"

#include <cyFileSystem.h>

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include <cyWindows.h>
#endif

using namespace CYLLENE_SDK;

int 
main(int argc, char* argv[]) {

  try
  {
    // CrashHandlerUI* window = new CrashHandlerUI();
    // Placeholder

    String params(*argv);

    if (params.find("-p") != -1) {
      // FileSystem::open()
      printf("p");


    }

    const String errorMessage = "Cyllene Engine Crash Reporter";
    MessageBox(nullptr, errorMessage.c_str(), "Cyllene Engine Reporter", MB_ICONERROR | MB_YESNO);
    // const String errorMessage = msg +
    //   "\n\nFor more information check the crash report located at:\n " +
    //   folder.fullPath();
    // MessageBox(nullptr, errorMessage.c_str(), "Cyllene Engine Error!", MB_OK);

  }
  catch (StdException* e)
  {
  	
  }

  return 0;
}