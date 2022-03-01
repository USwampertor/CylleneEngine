/*********************************************/
/*
 * @file 	main
 * @author	Marco "Swampertor" Millan
 * @date	24/01/2022
 * @brief	
 *
 */
/******************************************** */
#include "cyCrashHandlerPrerequisites.h"

#include <imgui/imgui.h>
#include <tchar.h>

#include <cyFileSystem.h>
#include <cyUtilities.h>
#include <cyArgumentParser.h>
#include <cyVector2i.h>

#if CY_PLATFORM == CY_PLATFORM_WIN32
# include <cyWindows.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include <d3d11.h>
#endif

using namespace CYLLENE_SDK;

static ImVec4 base = ImVec4(0.502f, 0.075f, 0.256f, 1.0f);
static ImVec4 bg = ImVec4(0.200f, 0.220f, 0.270f, 1.0f);
static ImVec4 text = ImVec4(0.860f, 0.930f, 0.890f, 1.0f);
static float high_val = 0.8f;
static float mid_val = 0.5f;
static float low_val = 0.3f;
static float window_offset = -0.2f;

inline ImVec4 make_high(float alpha) {
  ImVec4 res(0, 0, 0, alpha);
  ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
  res.z = high_val;
  ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
  return res;
}

inline ImVec4 make_mid(float alpha) {
  ImVec4 res(0, 0, 0, alpha);
  ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
  res.z = mid_val;
  ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
  return res;
}

inline ImVec4 make_low(float alpha) {
  ImVec4 res(0, 0, 0, alpha);
  ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
  res.z = low_val;
  ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
  return res;
}

inline ImVec4 make_bg(float alpha, float offset = 0.f) {
  ImVec4 res(0, 0, 0, alpha);
  ImGui::ColorConvertRGBtoHSV(bg.x, bg.y, bg.z, res.x, res.y, res.z);
  res.z += offset;
  ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
  return res;
}

inline ImVec4 make_text(float alpha) {
  return ImVec4(text.x, text.y, text.z, alpha);
}

void theme_generator() {
  ImGui::Begin("Theme generator");
  ImGui::ColorEdit3("base", (float*)&base, ImGuiColorEditFlags_PickerHueWheel);
  ImGui::ColorEdit3("bg", (float*)&bg, ImGuiColorEditFlags_PickerHueWheel);
  ImGui::ColorEdit3("text", (float*)&text, ImGuiColorEditFlags_PickerHueWheel);
  ImGui::SliderFloat("high", &high_val, 0, 1);
  ImGui::SliderFloat("mid", &mid_val, 0, 1);
  ImGui::SliderFloat("low", &low_val, 0, 1);
  ImGui::SliderFloat("window", &window_offset, -0.4f, 0.4f);

  ImGuiStyle& style = ImGui::GetStyle();

  style.Colors[ImGuiCol_Text] = make_text(0.78f);
  style.Colors[ImGuiCol_TextDisabled] = make_text(0.28f);
  style.Colors[ImGuiCol_WindowBg] = make_bg(1.00f, window_offset);
  style.Colors[ImGuiCol_ChildBg] = make_bg(0.58f);
  style.Colors[ImGuiCol_PopupBg] = make_bg(0.9f);
  style.Colors[ImGuiCol_Border] = make_bg(0.6f, -0.05f);
  style.Colors[ImGuiCol_BorderShadow] = make_bg(0.0f, 0.0f);
  style.Colors[ImGuiCol_FrameBg] = make_bg(1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = make_mid(0.78f);
  style.Colors[ImGuiCol_FrameBgActive] = make_mid(1.00f);
  style.Colors[ImGuiCol_TitleBg] = make_low(1.00f);
  style.Colors[ImGuiCol_TitleBgActive] = make_high(1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = make_bg(0.75f);
  style.Colors[ImGuiCol_MenuBarBg] = make_bg(0.47f);
  style.Colors[ImGuiCol_ScrollbarBg] = make_bg(1.00f);
  style.Colors[ImGuiCol_ScrollbarGrab] = make_low(1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = make_mid(0.78f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = make_mid(1.00f);
  style.Colors[ImGuiCol_CheckMark] = make_high(1.00f);
  style.Colors[ImGuiCol_SliderGrab] = make_bg(1.0f, .1f);
  style.Colors[ImGuiCol_SliderGrabActive] = make_high(1.0f);
  style.Colors[ImGuiCol_Button] = make_bg(1.0f, .2f);
  style.Colors[ImGuiCol_ButtonHovered] = make_mid(1.00f);
  style.Colors[ImGuiCol_ButtonActive] = make_high(1.00f);
  style.Colors[ImGuiCol_Header] = make_mid(0.76f);
  style.Colors[ImGuiCol_HeaderHovered] = make_mid(0.86f);
  style.Colors[ImGuiCol_HeaderActive] = make_high(1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
  style.Colors[ImGuiCol_ResizeGripHovered] = make_mid(0.78f);
  style.Colors[ImGuiCol_ResizeGripActive] = make_mid(1.00f);
  style.Colors[ImGuiCol_PlotLines] = make_text(0.63f);
  style.Colors[ImGuiCol_PlotLinesHovered] = make_mid(1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = make_text(0.63f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = make_mid(1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = make_mid(0.43f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = make_bg(0.73f);
  style.Colors[ImGuiCol_Tab] = make_bg(0.40f);
  style.Colors[ImGuiCol_TabHovered] = make_high(1.00f);
  style.Colors[ImGuiCol_TabActive] = make_mid(1.00f);
  style.Colors[ImGuiCol_TabUnfocused] = make_bg(0.40f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = make_bg(0.70f);
  style.Colors[ImGuiCol_DockingPreview] = make_high(0.30f);

  if (ImGui::Button("Export")) {
    ImGui::LogToTTY();
    ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
      const ImVec4& col = style.Colors[i];
      const char* name = ImGui::GetStyleColorName(i);
      ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
        name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
    }
    ImGui::LogFinish();
  }
  ImGui::End();
}



// Data
static ID3D11Device*            g_pd3dDevice            = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext     = NULL;
static IDXGISwapChain*          g_pSwapChain            = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView  = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget(); 
void CleanupRenderTarget();
void sendReport(const String& path);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


ImVec2
toImvec2(Vector2i v) { return ImVec2(v.x, v.y); }

int 
main(int argc, char* argv[]) {
  #if _DEBUG
    String dbgString = "-p CylleneDump20220224_185815.ccr";
    ArgumentParser parser;
    parser.m_argumentMap.clear();
    parser.addArgument("-p");
    parser.parse(dbgString);
  #else
    ArgumentParser parser;
    parser.m_argumentMap.clear();
    parser.addArgument("-p");
    parser.parse(*argv);
  
  #endif
  // ArgumentParser parser;
  // parser.m_argumentMap.clear();
  // parser.addArgument("-p");
  // parser.parse(*argv);

  File crash;
  if (parser.getParameter("-p") != Utils::BLANK) {
    crash  = FileSystem::open(FileSystem::getWorkingDirectory().fullPath() + 
                              "/Crash Reports/" + 
                              parser.getParameter("-p"));

  }

  Vector2i windowSize(1024, 620);
  Vector2i windowPos(200, 100);
  Vector2i buttonSize(450, 50);
  String windowTitle = "Cyllene Engine Crash Handler";
  WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
                    CS_CLASSDC, 
                    WndProc, 
                    0L, 
                    0L, 
                    GetModuleHandle(nullptr),
                    nullptr,
                    nullptr, 
                    nullptr, 
                    nullptr, 
                    windowTitle.c_str(), 
                    nullptr };
  wc.hIcon = static_cast<HICON>(LoadImage(nullptr,
                                          (FileSystem::getWorkingDirectory().fullPath() + "/ch.ico").c_str(),
                                          IMAGE_ICON,
                                          0,
                                          0,
                                          LR_LOADFROMFILE |
                                          LR_DEFAULTSIZE  |
                                          LR_SHARED       ));
  ::RegisterClassEx(&wc);
  HWND hwnd = ::CreateWindow(wc.lpszClassName, 
                             windowTitle.c_str(),
                             WS_POPUP         | 
                             WS_SYSMENU       | 
                             WS_MAXIMIZEBOX   | 
                             WS_OVERLAPPED    |
                             WS_MINIMIZEBOX   ,
                             windowPos.x, 
                             windowPos.y, 
                             windowSize.x, 
                             windowSize.y, 
                             nullptr, 
                             nullptr, 
                             wc.hInstance, 
                             nullptr);
  
  // Initialize Direct3D
  if (!CreateDeviceD3D(hwnd)) {
    CleanupDeviceD3D();
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 1;
  }

  // Show the window
  ::ShowWindow(hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGuiIO& io = ImGui::GetIO(); 
  (void)io;

  // Setup Dear ImGui style
  // ImGui::StyleColorsDark();
  ImGuiStyle* style = &ImGui::GetStyle();
  style->WindowTitleAlign = ImVec2(0.5, 0.5);
  
  ImVec4* colors = style->Colors;

  /// 0 = FLAT APPEARENCE
  /// 1 = MORE "3D" LOOK
  int is3D = 0;

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

  auto originalFontSize = ImGui::GetFontSize();

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(hwnd);
  ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

  String fontPath       = FileSystem::getWorkingDirectory().fullPath() + "/Buran USSR.ttf";
  ImFont* tinyUSSRfont  = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 12.0f);
  ImFont* smallUSSRfont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f);
  ImFont* bigUSSRfont   = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 34.0f);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
  char detailsBuffer[1024] = "";
  int size = crash.readFile().length();
  char crashDetails[2048];
  strcpy(crashDetails, crash.readFile().c_str());
  // Main loop
  bool done = false;
  bool shouldDrag = false;
  bool alwaysOpen = true;
  while (!done)
  {
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        done = true;
    }
    if (done) {
      break;
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    {

      ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::PushFont(bigUSSRfont);
      if (ImGui::Begin("CYLLENE CRASH HANDLER",
                       &alwaysOpen,
                       ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove |
                       ImGuiWindowFlags_NoCollapse)) {

        if (ImGui::IsMouseDown(0) && ImGui::GetMousePos().y < ImGui::GetFontSize()) {
          shouldDrag = true;
        }
        else if ((!ImGui::IsMouseDown(0))) {
          shouldDrag = false;
        }

        if (shouldDrag) {
          RECT rect;
          if (GetWindowRect(hwnd, &rect)) {
            MoveWindow(hwnd, 
                       rect.left +  ImGui::GetMouseDragDelta().x, 
                       rect.top +   ImGui::GetMouseDragDelta().y,
                       windowSize.x, 
                       windowSize.y, 
                       true);

          }
        }
        ImGui::PopFont();
        ImGui::PushFont(bigUSSRfont);
        ImGui::Text("Oh no! Cyllene Engine has crashed!");
        ImGui::PopFont();
        ImGui::PushFont(smallUSSRfont);
        ImGui::Text("\n");
        ImGui::Text("We are very sorry comrade, but it seems that Cyllene Engine got into an error " \
                    "and crashed! But we can grow stronger and better if you help us sending the \n" \
                    "information below so our experts can handle the situation! " \
                    "Thanks for your help improving the Cyllene Engine");
        ImGui::Text("\n");
        ImGui::PopFont();
        ImGui::PushFont(tinyUSSRfont);
        if (ImGui::InputTextMultilineWithHint("##Description",
                                              "Type what you were doing here",
                                              detailsBuffer,
                                              sizeof(detailsBuffer),
                                              ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetTextLineHeight() * 6),
                                              ImGuiInputTextFlags_AllowTabInput)) {

        }
        ImGui::PopFont();
        ImGui::PushFont(smallUSSRfont);
        ImGui::Text("\n");
        ImGui::Text("Cyllene Engine retrieved the next callstack before shutting down:");
        ImGui::PopFont();
        ImGui::PushFont(tinyUSSRfont);
        if (ImGui::InputTextMultilineWithHint("##Stack",
                                              "",
                                              crashDetails,
                                              sizeof(crashDetails),
                                              ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetTextLineHeight() * 20),
                                              ImGuiInputTextFlags_ReadOnly)) {

        }

        ImGui::PopFont();
        ImGui::PushFont(bigUSSRfont);
        ImGui::Text("\n");
        ImGui::Text("");
        ImGui::SameLine(50, 0);
        if (ImGui::Button("Send Report", toImvec2(buttonSize) )) {
          sendReport(crash.path());
          done = true;
        }
        ImGui::SameLine(0, ImGui::GetWindowContentRegionWidth() - (2 * (50 + buttonSize.x)) );
        if (ImGui::Button("Close", toImvec2(buttonSize))) {
          done = true;
        }

        ImGui::PopFont();
        ImGui::End();
      }

    }
    theme_generator();

    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    g_pSwapChain->Present(1, 0); // Present with vsync
    if (alwaysOpen == false) { break; }
  }

  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(hwnd);
  ::UnregisterClass(wc.lpszClassName, wc.hInstance);

  return 0;
}

void
sendReport(const String& fileName) {
  Utils::runCommand(String("curl --ssl-reqd --url 'smtps://smtp.gmail.com:465' "
                           "--user 'mmillan@uartesdigitales.edu.mx:millan97' "
                           "--mail-from 'mmillan@uartesdigitales.edu.mx' " 
                           "--mail-rcpt 'jmmillan@uartesdigitales.edu.mx' " 
                           "--upload-file") + fileName);
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.BufferDesc.Width = 0;
  sd.BufferDesc.Height = 0;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
  if (D3D11CreateDeviceAndSwapChain(NULL, 
                                    D3D_DRIVER_TYPE_HARDWARE, 
                                    NULL, 
                                    createDeviceFlags, 
                                    featureLevelArray, 
                                    2, 
                                    D3D11_SDK_VERSION, 
                                    &sd, 
                                    &g_pSwapChain,
                                    &g_pd3dDevice, 
                                    &featureLevel, 
                                    &g_pd3dDeviceContext) != S_OK)
                                    return false;

  CreateRenderTarget();
  return true;
}

void CleanupDeviceD3D()
{
  CleanupRenderTarget();
  if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
  if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
  if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
  ID3D11Texture2D* pBackBuffer;
  g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
  pBackBuffer->Release();
}

void CleanupRenderTarget()
{
  if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

  switch (msg)
  {
  case WM_SIZE:
    if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
    {
      CleanupRenderTarget();
      g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
      CreateRenderTarget();
    }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_DESTROY:
    ::PostQuitMessage(0);
    return 0;
  }
  return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

