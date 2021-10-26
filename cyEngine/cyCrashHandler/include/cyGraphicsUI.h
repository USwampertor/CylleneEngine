/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyCrashHandlerUI.h
 * @author Marco "Swampy" Millan
 * @date 10/22/2021
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once

#include "cyCrashHandlerPrerequisites.h"
#include "imgui/imgui.h"
#include <cyModule.h>

namespace CYLLENE_SDK {

class GraphicsUI : public Module<GraphicsUI>
{
public :
  
  /**
    * Default constructor
    */
  GraphicsUI() = default;

  /**
    * Default destructor
    */
  virtual ~GraphicsUI() = default;

  virtual bool
  init(ImGuiIO& io, void* hwnd);

  virtual void
  prerender();

  virtual void
  render();

  virtual void
  postRender();

  virtual void
  newFrame();

  virtual void
  endFrame();

  virtual bool
  updateInput() { return true; }

  void
  renderBaseUI();

  virtual void
  finish();

  /**
    * This object should exist and be initialized only once. This is the constant
    * reference for ImGui to exist.
    */
  ImGuiIO* m_ui;

  void* m_device;

  void* m_deviceContext;

  void* m_swapChain;

  void* m_mainRenderTargetView;

};

}
