
#pragma once

#include "cyPlatformCrashHandlerUI.h"
#include <imgui/imgui.h>

#if CY_PLATFORM == CY_PLATFORM_WIN32
#include <imgui/imgui_impl_win32.h>
# include <cyWindows.h>
# include <d3d11.h>
# ifndef CY_GRAPHICS_D3D11
#  define CY_GRAPHICS_D3D11
# endif

# ifdef CY_GRAPHICS_D3D11
#  include <imgui/imgui_impl_dx11.h>
# endif // DEBUG

#endif 

namespace CYLLENE_SDK {
class WindowsCrashHandlerUI : public PlatformCrashHandlerUI 
{
public:

  WindowsCrashHandlerUI() = default;

  WindowsCrashHandlerUI(int newWidth = 1024, int newHeight = 576) {
    m_width = newWidth;
    m_height = newHeight;
    m_hwnd = {};
    m_wc = {};
    m_windowIcon = {};
    m_io = nullptr;
  }

  ~WindowsCrashHandlerUI() = default;

  virtual bool
  init(void* data) override;

  virtual void
  newFrame() override;

  virtual void
  showWindow() override;

  virtual void
  shutdown() override;


private:

  HICON m_windowIcon;
  WNDCLASSEX m_wc;
  HWND m_hwnd;

};

  static ID3D11Device*           g_pd3dDevice            = nullptr;
  static ID3D11DeviceContext*    g_pd3dDeviceContext     = nullptr;
  static IDXGISwapChain*         g_pSwapChain            = nullptr;
  static ID3D11RenderTargetView* g_mainRenderTargetView  = nullptr;
  bool CreateDeviceD3D(HWND hWnd);
  void CleanupDeviceD3D();
  void CreateRenderTarget();
  void CleanupRenderTarget();
  // LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

}
