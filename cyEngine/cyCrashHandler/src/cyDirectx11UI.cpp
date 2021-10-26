
#include "cyDirectx11UI.h"

namespace CYLLENE_SDK {
  
  bool
  Directx11UI::init(ImGuiIO& io, void* hwnd) {
    if (!GraphicsUI::init(io, hwnd)) { return false; }



    if (ImGui_ImplWin32_Init(hwnd)) { return false; }
    return true;
  }

  void
  Directx11UI::prerender() {

  }

  void
  Directx11UI::render() {

  }

  void
  Directx11UI::postRender() {

  }
  void
  Directx11UI::newFrame() {

  }

  void
  Directx11UI::endFrame() {

  }

  bool
  Directx11UI::updateInput() {
    return true;
  }
}

