#pragma once

#include <OIS/OIS.h>

#include "cyPlatformTypes.h"
#include "cyStdHeaders.h"

namespace CYLLENE_SDK
{

/**
*
*/
class Input {
 public:
  /**
  *
  */
  Input();

  /**
  *
  */
  Input(SIZE_T _windowHandle);
  
  /**
  *
  */
  ~Input();

  /**
  *
  */
  void
  Initialize();
 private:
   SIZE_T m_windowHandle;
   OIS::InputManager* m_manager;
};

}