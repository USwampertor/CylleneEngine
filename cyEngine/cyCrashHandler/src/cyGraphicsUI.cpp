
#include "cyGraphicsUI.h"

namespace CYLLENE_SDK {
  bool
  GraphicsUI::init(ImGuiIO& io, void* hwnd) {
    m_ui = &io;
    return true;
  }

  void
  GraphicsUI::prerender() {

  }

  void
  GraphicsUI::render() {
    renderBaseUI();
  }

  void
  GraphicsUI::postRender() {
    ImGui::Render();
  }

  void
  GraphicsUI::newFrame() {
    ImGui::NewFrame();
  }

  void
  GraphicsUI::endFrame() {
    ImGui::EndFrame();
  }

  void
  GraphicsUI::finish() {
    ImGui::DestroyContext();
  }

  void
  GraphicsUI::renderBaseUI() {

  }

}
