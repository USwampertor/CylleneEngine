#include "cyPlatformCrashHandlerUI.h"

namespace CYLLENE_SDK {

  const String m_icon = "ceicon.ico";

  const Vector2i m_buttonSize = Vector2i(200, 50);

  ImVec2
  toImVec2(Vector2i v) {
    return ImVec2(v.x, v.y);
  }

  void
  PlatformCrashHandlerUI::init() {
    ImGui::CreateContext();
    // m_io = ImGui::GetIO(); 
    // (void)m_io;

    ImGui::StyleColorsDark();
  }

  void
  PlatformCrashHandlerUI::showWindow() {
    static float f = 0.0f;
      static int counter = 0;
      bool alwaysOpen = true;
      ImGui::SetNextWindowSize(ImVec2(1010, 570));
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::Begin("CYLLENE CRASH HANDLER", 
                   &alwaysOpen, 
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

      // if (ImGui::Button("Send Report"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      //   counter++;
      // ImGui::SameLine();
      // ImGui::Text("counter = %d", counter);
      // 
      // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
  }

  void
  PlatformCrashHandlerUI::sendError() {

  }

  void
  PlatformCrashHandlerUI::shutdown() {

  }
}