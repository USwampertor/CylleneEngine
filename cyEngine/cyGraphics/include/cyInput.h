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
  Input(SizeT _windowHandle);
  
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
   SizeT m_windowHandle;
   OIS::InputManager* m_manager;
};

}