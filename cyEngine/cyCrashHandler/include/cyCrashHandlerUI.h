/*********************************************/
/*
 * @file 	cyCrashHandler
 * @author	Marco "Swampertor" Millan
 * @date	24/01/2022
 * @brief	
 *
 */
/******************************************** */

#pragma once
#include <cyModule.h>

namespace CYLLENE_SDK {

  class CrashHandlerUI
  {
  public:

    /**
     *
     */
    CrashHandlerUI() = default;

    /**
     *
     */
    ~CrashHandlerUI() = default;

    void
    init();

    void
    showMessage();

    void
    sendError();

    void
    shutdown();
  };

}


