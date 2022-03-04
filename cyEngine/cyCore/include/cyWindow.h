/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyWindow.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/03/04
 * @brief This is a cross platform integration of CrossWindow project
 *        for, as obvious as it could be, cross platform window handling
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCorePrerequisites.h"

#include <cyException.h>
#include <cyModule.h>
#include <cyVector2i.h>
#include <cyVector2f.h>
#include <cyVector3f.h>
#include <cyVector4f.h>

#include <SDL2/SDL.h>

namespace CYLLENE_SDK {

using Window      = SDL_Window;
using WindowEvent = SDL_WindowEvent;

struct CY_CORE_EXPORT WindowSettings
{
  String name;
  String title;
  Vector2i position;
  Vector2i size;
  uint32 flags;
};

class CY_CORE_EXPORT WindowManager : public Module<WindowManager>
{
public:

  WindowManager() = default;

  ~WindowManager() = default;


  bool 
  init();

  SharedPointer<Window> 
  createWindow(const String& title,
               const int32& posX, 
               const int32& posY, 
               const int32& width, 
               const int32& height, 
               const int32& flags);

  SharedPointer<Window> 
  createWindow(const String& title,
               const Vector2i& pos, 
               const Vector2i& size, 
               const int32& flags);

  SharedPointer<Window>
  createWindow(const WindowSettings& settings);

  SharedPointer<Window>
  getWindow(const int32& window);


  void
  finish();

  Vector<SharedPointer<Window>> m_windows;
};
}