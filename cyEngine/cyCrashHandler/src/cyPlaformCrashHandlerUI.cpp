#include "cyPlatformCrashHandlerUI.h"

namespace CYLLENE_SDK {

  const String m_icon = "ch.ico";

  const Vector2i PlatformCrashHandlerUI::m_buttonSize = Vector2i(450, 50);

  ImVec2
  toImVec2(Vector2i v) {
    return ImVec2(v.x, v.y);
  }

  bool
  PlatformCrashHandlerUI::init(void* data) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void)m_io;

    ImGui::StyleColorsDark();
    m_stackBuffer = "PLACEHOLDER \n PLACEHOLDER \n PLACEHOLDER";
    m_detailsBuffer = "";

    ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowTitleAlign = ImVec2(0.5, 0.5);

    ImVec4* colors = style->Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int32 is3D = 0;

    style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style->Colors[ImGuiCol_Separator] = style->Colors[ImGuiCol_Border];
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style->GrabRounding = style->FrameRounding = 2.3f;

    style->PopupRounding = 3;

    style->WindowPadding = ImVec2(4, 4);
    style->FramePadding = ImVec2(6, 4);
    style->ItemSpacing = ImVec2(6, 2);

    style->ScrollbarSize = 18;

    style->WindowBorderSize = 0;
    style->ChildBorderSize = 1;
    style->PopupBorderSize = 1;
    style->FrameBorderSize = 1;

    style->WindowRounding = 3;
    style->ChildRounding = 3;
    style->ScrollbarRounding = 2;

#ifdef IMGUI_HAS_DOCK 
    style->TabBorderSize = is3D;
    style->TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style->WindowRounding = 0.0f;
      style->Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif

    String fontPath = FileSystem::getWorkingDirectory().fullPath() + "/Buran USSR.ttf";
    ImFont* tinyUSSRfont = m_io->Fonts->AddFontFromFileTTF(fontPath.c_str(), 10.0f);
    ImFont* smallUSSRfont = m_io->Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f);
    ImFont* bigUSSRfont = m_io->Fonts->AddFontFromFileTTF(fontPath.c_str(), 30.0f);

    m_fontMap.insert(std::make_pair("tinyUSSR", tinyUSSRfont));
    m_fontMap.insert(std::make_pair("smallUSSR", smallUSSRfont));
    m_fontMap.insert(std::make_pair("bigUSSR", bigUSSRfont));

    return true;
  }

  void
  PlatformCrashHandlerUI::newFrame() {
    ImGui::NewFrame();
  }

  void
  PlatformCrashHandlerUI::showWindow() {

    ImGui::SetNextWindowSize(ImVec2(m_width - 10, m_height));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::PushFont(m_fontMap["bigUSSR"]);
    ImGui::Begin("CYLLENE CRASH HANDLER",
                 &m_done,
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);
      
    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["bigUSSR"]);
    ImGui::Text("Oh no! Cyllene Engine has crashed!");
    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["smallUSSR"]);
    ImGui::Text("\n");
    ImGui::Text("We are very sorry comrade, but it seems that Cyllene Engine got into an error" \
                " and crashed! But we can grow stronger and better if you help us sending the" \
                "\ninformation below so our experts can handle the situation! " \
                " Thanks for your help improving the Cyllene Engine");
    ImGui::Text("\n");
    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["tinyUSSR"]);
    if (ImGui::InputTextMultilineWithHint("##Description",
                                          "Type what you were doing here",
                                          strdup(m_detailsBuffer.c_str()) ,
                                          sizeof(m_detailsBuffer.c_str()),
                                          ImVec2(m_width, ImGui::GetTextLineHeight() * 10),
                                          ImGuiInputTextFlags_AllowTabInput)) {

    }
    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["smallUSSR"]);
    ImGui::Text("\n");
    ImGui::Text("Cyllene Engine retrieved the next callstack before shutting down:");
    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["tinyUSSR"]);
    if (ImGui::InputTextMultilineWithHint("##Stack",
                                          "",
                                          strdup(m_stackBuffer.c_str()),
                                          sizeof(m_stackBuffer.c_str()),
                                          ImVec2(m_width, ImGui::GetTextLineHeight() * 18),
                                          ImGuiInputTextFlags_ReadOnly)) {

    }

    ImGui::PopFont();
    ImGui::PushFont(m_fontMap["bigUSSR"]);
    ImGui::Text("\n");
    ImGui::Text("\n");
    ImGui::SameLine(50, 0);
    if (ImGui::Button("Send Report", toImVec2(m_buttonSize))) {
      m_done = true;
    }
    ImGui::SameLine(0, 10);
    if (ImGui::Button("Close", toImVec2(m_buttonSize))) {
      m_done = true;
    }

    ImGui::PopFont();
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