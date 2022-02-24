#include "cyPlatformCrashHandlerUI.h"

namespace CYLLENE_SDK {

  const String m_icon = "ch.ico";

  const Vector2i m_buttonSize = Vector2i(200, 50);

  ImVec2
  toImVec2(Vector2i v) {
    return ImVec2(v.x, v.y);
  }

  void
  PlatformCrashHandlerUI::init(void* data) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void)m_io;

    ImGui::StyleColorsDark();
    m_stackBuffer = "PLACEHOLDER \n PLACEHOLDER \n PLACEHOLDER";
    m_detailsBuffer = "";
  }

  void
  PlatformCrashHandlerUI::newFrame() {
    ImGui::NewFrame();
  }

  void
  PlatformCrashHandlerUI::showWindow() {

    ImGui::SetNextWindowSize(ImVec2(1010, 570));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("CYLLENE CRASH HANDLER", 
                  &m_done, 
                  ImGuiWindowFlags_NoResize  |
                  ImGuiWindowFlags_NoMove    |
                  ImGuiWindowFlags_NoCollapse);
      
    ImGui::Text("Oh no! Cyllene Engine has crashed!");
    ImGui::Text("We are very sorry comrade");
    ImGui::Text("Thanks for your help improving the Cyllene Engine");
    // char* buffer = {};
    // ImGui::InputText("",buffer, sizeof(buffer));
      
    if (ImGui::Button("Send Report", ImVec2(200,50))) {
      m_done = true;
    }
    ImGui::SameLine(0,1020-430);
    if (ImGui::Button("Close", ImVec2(200, 50))) {
      m_done = true;
    }

    ImGui::Text("Oh no! Cyllene Engine has crashed!");
    ImGui::Text("We are very sorry comrade");
    ImGui::Text("Thanks for your help improving the Cyllene Engine");
    if (ImGui::InputTextMultilineWithHint("##Description", "Type what you were doing here", strdup(m_detailsBuffer.c_str()), sizeof(m_detailsBuffer), ImVec2(1020, 200))) {

    }
    ImGui::Text("Cyllene Engine retrieved the next callstack before shutting down");
    if (ImGui::InputTextMultilineWithHint("##Stack", "", strdup(m_stackBuffer.c_str()), sizeof(m_stackBuffer), ImVec2(1020, 200), ImGuiInputTextFlags_ReadOnly)) {
      ImGui::BeginChild("scrolling");
      ImGui::SetScrollY(ImGui::GetScrollY());
      ImGui::EndChild();
    }


    if (ImGui::Button("Send Report", ImVec2(500, 50))) {
      m_done = true;
    }
    ImGui::SameLine(0, 1020 - 1015);
    if (ImGui::Button("Close", ImVec2(500, 50))) {
      m_done = true;
    }

    ImGui::End();
  }

  void
  PlatformCrashHandlerUI::sendError() {

  }

  void
  PlatformCrashHandlerUI::shutdown() {
    ImGui::DestroyContext();
  }
}