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

#include <imgui/imgui.h>

#include <cyFileSystem.h>
#include <cyModule.h>
#include <cyVector2i.h>

namespace CYLLENE_SDK {

class PlatformCrashHandlerUI
{
 public:
  PlatformCrashHandlerUI() = default;


  /**
    *
    */
  virtual ~PlatformCrashHandlerUI() = default;

  virtual void
  init(void* data);

  virtual void
  showWindow();

  virtual void
  newFrame();

  void
  sendError();

  virtual void
  shutdown();

  bool
  isDone() { return m_done; }

  int m_width = 0;

  int m_height = 0;

  bool m_done = false;

  File m_iconFile = {};

  File m_stackFile = {};

  static const String m_icon;

  static const Vector2i m_buttonSize;

  ImGuiIO* m_io;

  String m_stackBuffer;

  String m_detailsBuffer;
};

}


