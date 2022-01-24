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
using namespace CYLLENE_SDK;

int 
main(int argc, char* argv[]) {

  try
  {
    CrashHandlerUI* window = new CrashHandlerUI();
  }
  catch (Exception* e)
  {
  	
  }

  return 0;
}