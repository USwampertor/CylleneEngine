#include "cyWindowsCrashHandlerUI.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace CYLLENE_SDK {
  
  
  LRESULT WINAPI 
  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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


  void
  WindowsCrashHandlerUI::init(void* data) {

    HICON hicon = LoadIcon(NULL, IDI_QUESTION);
    if (FileSystem::exists(FileSystem::getWorkingDirectory().fullPath() + "/ch.ico")) {
      File iconFile = FileSystem::open(FileSystem::getWorkingDirectory().fullPath() + "/ch.ico");
      String iconBuffer = Utils::BLANK;
      iconBuffer = iconFile.readFile().c_str();
      std::cout << iconBuffer << std::endl;
      static const int icon_size = 32;
      int offset = LookupIconIdFromDirectoryEx((PBYTE)iconBuffer.c_str(), TRUE, icon_size, icon_size, LR_DEFAULTCOLOR);
      if (offset != 0) {
        hicon = CreateIconFromResourceEx((PBYTE)iconBuffer.c_str() + offset, 0, TRUE, 0x30000, icon_size, icon_size, LR_DEFAULTCOLOR);
        if (hicon == nullptr) {
          hicon = LoadIcon(NULL, IDI_QUESTION);
        }
      }
    }



    String crashTitle("Cyllene Engine Crash Handler");
    WNDCLASSEX m_wc = { sizeof(WNDCLASSEX),
                    CS_CLASSDC,
                    WndProc,
                    0L,
                    0L,
                    GetModuleHandle(nullptr),
                    m_windowIcon,
                    nullptr,
                    nullptr,
                    nullptr,
                    crashTitle.c_str(),
                    m_windowIcon };
    
    ::RegisterClassEx(&m_wc);
    m_hwnd = ::CreateWindow(m_wc.lpszClassName, 
                             crashTitle.c_str(), 
                             WS_POPUP         | 
                             WS_THICKFRAME    | 
                             WS_SYSMENU       | 
                             WS_MAXIMIZEBOX   | 
                             WS_MINIMIZEBOX   ,
                             100, 
                             100, 
                             1024, 
                             576, 
                             nullptr, 
                             nullptr, 
                             m_wc.hInstance, 
                             nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(m_hwnd))
    {
      CleanupDeviceD3D();
      ::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
      return;
    }
    // Show the window
    ::ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(m_hwnd);

    PlatformCrashHandlerUI::init(data);

    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
  }

  void
  WindowsCrashHandlerUI::newFrame() {
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        m_done = true;
    }
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    PlatformCrashHandlerUI::newFrame();
  }

  void
  WindowsCrashHandlerUI::showWindow() {

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    PlatformCrashHandlerUI::showWindow();
    if (ImGui::IsMouseDragging(0)) {

      MoveWindow(m_hwnd, 
                 ImGui::GetMousePos().x, 
                 ImGui::GetMousePos().y, 
                 m_width, 
                 m_height, 
                 true);
    }
    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
  }

  void
  WindowsCrashHandlerUI::shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    PlatformCrashHandlerUI::shutdown();

    CleanupDeviceD3D();
    ::DestroyWindow(m_hwnd);
    ::UnregisterClass(m_wc.lpszClassName, m_wc.hInstance);
  }

  bool 
  CreateDeviceD3D(HWND hWnd) {
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

  void 
  CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { 
      g_pSwapChain->Release(); 
      g_pSwapChain = nullptr; 
    }
    if (g_pd3dDeviceContext) { 
      g_pd3dDeviceContext->Release(); 
      g_pd3dDeviceContext = nullptr; 
    }
    if (g_pd3dDevice) {
      g_pd3dDevice->Release(); 
      g_pd3dDevice = nullptr;
    }
  }


  void 
  CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
  }
  

  void 
  CleanupRenderTarget() {
    if (g_mainRenderTargetView) { 
      g_mainRenderTargetView->Release(); 
      g_mainRenderTargetView = nullptr; }
  }

  
}