/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyGLUI.h
 * @author Marco "Swampy" Millan
 * @date 10/26/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCrashHandlerPrerequisites.h"
#include "cyGraphicsUI.h"

#include <cyModule.h>

namespace CYLLENE_SDK {

class GLUI : public GraphicsUI
{
public:
  
  /**
   * Default constructor
   */
  GLUI()  = default;
  
  /**
   * Default destructor
   */
  ~GLUI() = default;

};

}
