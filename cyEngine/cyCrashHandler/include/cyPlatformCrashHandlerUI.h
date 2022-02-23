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
#include "cyCrashHandlerPrerequisites.h"
#include <cyFileSystem.h>
#include <imgui/imgui.h>
#include <cyVector2i.h>

namespace CYLLENE_SDK {

class PlatformCrashHandlerUI
{
  public:

  /**
    *
    */
  PlatformCrashHandlerUI() = default;

  PlatformCrashHandlerUI(int newWidth = 1024, int newHeight = 576)
    : m_width(newWidth),
      m_height(newHeight),
      m_done(false) {}

  /**
    *
    */
  ~PlatformCrashHandlerUI() = default;

  void
  init();

  void
  showWindow();

  void
  sendError();

  void
  shutdown();

  int m_width;

  int m_height;

  bool m_done;

  File m_iconFile;

  static const String m_icon;

  static const Vector2i m_buttonSize;

  // static ImGuiIO& m_io;
};

}


