/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyWindowsUI.h
 * @author Marco "Swampy" Millan
 * @date 10/23/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyCrashHandlerPrerequisites.h"
#include "cyGraphicsUI.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace CYLLENE_SDK {

class Directx11UI : public GraphicsUI
{
public:
  /**
   * Default constructor
   */
  Directx11UI()   = default;
  
  /**
   * Default destructor
   */
  ~Directx11UI()  = default;

  virtual bool
  init(ImGuiIO& io, void* hwnd) override;

  virtual void
  prerender() override;

  virtual void
  render() override;

  virtual void
  postRender() override;

  virtual void
  newFrame() override;

  virtual void
  endFrame() override;

  virtual bool
  updateInput() override;
};

}
