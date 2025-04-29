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

namespace CYLLENE_SDK {

struct Ve2
{
public:
  Ve2() = default;
  Ve2(int32 nx, int32 ny)
    : x(nx),
      y(ny) {}

  int32 x, y;
};


class PlatformCrashHandlerUI
{
 public:
  PlatformCrashHandlerUI() = default;


  /**
    *
    */
  virtual ~PlatformCrashHandlerUI() = default;

  virtual bool
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

  int32 m_width = 0;

  int32 m_height = 0;

  bool m_done = false;

  File m_iconFile = {};

  File m_stackFile = {};

  static const String m_icon;

  static const Ve2 m_buttonSize;

  ImGuiIO* m_io;

  String m_stackBuffer;

  String m_detailsBuffer;

  Map<String, ImFont*> m_fontMap;
};

}


