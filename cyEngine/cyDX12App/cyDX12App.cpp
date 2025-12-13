// cyDX12App.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "cyDX12App.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <DirectXMath.h> 
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Pipeline variables
ID3D12Device* m_device = nullptr;
ID3D12CommandQueue* m_commandQueue = nullptr;
ID3D12CommandAllocator* m_commandAllocator = nullptr;
ID3D12DescriptorHeap* m_rtvHeap = nullptr;
ID3D12DescriptorHeap* m_srvHeap;

// Asset variables
ID3D12GraphicsCommandList* m_commandList;
ID3D12PipelineState* m_pipelineState = nullptr;
ID3D12RootSignature* m_rootSignature = nullptr;
ID3D12Resource* m_vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
CD3DX12_VIEWPORT* m_viewport;
ID3D12Resource* m_texture;

IDXGISwapChain3* m_swapChain = nullptr;
ID3D12Resource* m_renderTargets[2] = { nullptr, nullptr };
UINT m_rtvDescriptorSize = 0;

HANDLE m_fenceEvent;
ID3D12Fence* m_fence;
UINT64 m_fenceValue;

void render();
void loadAssets();
void update(float deltaTime);
void populateCommandList();
void getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
void waitForPreviousFrame();

void 
getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter) {
  *ppAdapter = nullptr;
  IDXGIAdapter1* pAdapter = nullptr;
  IDXGIFactory6* pFactory6 = nullptr;

  if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&pFactory6)))) {
    for(UINT adapterIndex = 0;
        SUCCEEDED(pFactory6->EnumAdapterByGpuPreference(adapterIndex,
                                                        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                                        IID_PPV_ARGS(&pAdapter)));
        ++adapterIndex) {

      DXGI_ADAPTER_DESC1 desc;
      pAdapter->GetDesc1(&desc);
      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        // Don't select the Basic Render Driver adapter.
        pAdapter->Release();
        continue;
      }
      // Check to see if the adapter supports Direct3D 12, but don't create the
      // actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(pAdapter,
                                      D3D_FEATURE_LEVEL_11_0,
                                      __uuidof(ID3D12Device),
                                      nullptr))) {
        *ppAdapter = pAdapter;
        pFactory6->Release();
        break;
      }
      pAdapter->Release();
    }
  }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_CYDX12APP, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance (hInstance, nCmdShow)) {
    return FALSE;
  }

  loadAssets();

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CYDX12APP));

  MSG msg{0};
  bool running = true;
  // Main message loop:
  while (running) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        running = false;
        break;
      }

      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    update(0.016f);
    render();
  }

  return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CYDX12APP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CYDX12APP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance; // Store instance handle in our global variable

  HWND hWnd = CreateWindowW(szWindowClass, 
                            szTitle, 
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 
                            0, 
                            CW_USEDEFAULT, 
                            0, 
                            nullptr, 
                            nullptr, 
                            hInstance, 
                            nullptr);

  if (!hWnd) {
    return FALSE;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
  {
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
      debugController->EnableDebugLayer();
      debugController->Release();
      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }

#endif

  IDXGIFactory4* pFactory;
  HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&pFactory));
  if (FAILED(hr)) {
    return FALSE;
  }

  IDXGIAdapter1* pHardwareAdapter = nullptr;
  getHardwareAdapter(pFactory, &pHardwareAdapter);

  hr = D3D12CreateDevice(pHardwareAdapter,
                         D3D_FEATURE_LEVEL_11_0,
                         IID_PPV_ARGS(&m_device));
  // Create command queue
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));

  if(FAILED(hr)) {
    return FALSE;
  }


  RECT rect;
  GetClientRect(hWnd, &rect);

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = 2;
  swapChainDesc.Width = rect.right - rect.left;
  swapChainDesc.Height = rect.bottom - rect.top;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;

  pFactory->CreateSwapChainForHwnd(m_commandQueue,
                                  hWnd,
                                  &swapChainDesc,
                                  nullptr,
                                  nullptr,
                                  reinterpret_cast<IDXGISwapChain1**>(&m_swapChain));

  pFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
  auto frameIndex = m_swapChain->GetCurrentBackBufferIndex();
  {
    // Create descriptor heaps.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr)) {
      return FALSE;
    }
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    hr = m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
    if (FAILED(hr)) {
      return FALSE;
    }

  }

  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT n = 0; n < 2; n++) {
      hr = m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
      if (FAILED(hr)) {
        return FALSE;
      }
      m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSize);
    }

  }

  hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                        IID_PPV_ARGS(&m_commandAllocator));
  if (FAILED(hr)) {
    return FALSE;
  }

  m_viewport = new CD3DX12_VIEWPORT(0.0f, 
                                    0.0f, 
                                    rect.right - rect.left, 
                                    rect.bottom - rect.top);

  // Cleanup
  pHardwareAdapter->Release();
  pFactory->Release();

  return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
    {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId)
      {
      case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
      case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
      default:
        return DefWindowProc(hWnd, message, wParam, lParam);
      }
    }
    break;
  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      // TODO: Add any drawing code that uses hdc here...
      EndPaint(hWnd, &ps);
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}


void loadAssets() {

  // {
  //   m_device->CreateCommandList(0, 
  //                               D3D12_COMMAND_LIST_TYPE_DIRECT, 
  //                               m_commandAllocator, 
  //                               nullptr, 
  //                               IID_PPV_ARGS(&m_commandList));
  //   m_commandList->Close();
  //   {
  //     m_device->CreateFence(0,
  //                           D3D12_FENCE_FLAG_NONE,
  //                           IID_PPV_ARGS(&m_fence));
  //     m_fenceValue = 1;
  //     m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  //     if (m_fenceEvent == nullptr) {
  //       HRESULT_FROM_WIN32(GetLastError());
  // 
  //     }
  //   }
  // }

  {
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(0,
                           nullptr,
                           0,
                           nullptr,
                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ID3DBlob* signature;
    ID3DBlob* error;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc,
                                             D3D_ROOT_SIGNATURE_VERSION_1,
                                             &signature,
                                             &error);

    m_device->CreateRootSignature(0, 
                                  signature->GetBufferPointer(),
                                  signature->GetBufferSize(),
                                  IID_PPV_ARGS(&m_rootSignature));
    ID3DBlob* vertexShader;
    ID3DBlob* pixelShader;

    UINT compileFlags = 0;

#if defined(_DEBUG)
    {
      compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    }
#endif
    D3DCompileFromFile(L"shader.hlsl",
                       nullptr,
                       D3D_COMPILE_STANDARD_FILE_INCLUDE,
                       "VSMain",
                       "vs_5_0",
                       compileFlags,
                       0,
                       &vertexShader,
                       &error);

    D3DCompileFromFile(L"shader.hlsl",
                       nullptr,
                       D3D_COMPILE_STANDARD_FILE_INCLUDE,
                       "PSMain",
                       "ps_5_0",
                       compileFlags,
                       0,
                       &pixelShader,
                       &error);

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
  
  }

  {
    m_device->CreateCommandList(0, 
                                D3D12_COMMAND_LIST_TYPE_DIRECT, 
                                m_commandAllocator, 
                                m_pipelineState, 
                                IID_PPV_ARGS(&m_commandList));
    m_commandList->Close();
  }
  {
    struct Vertex
    {
      DirectX::XMFLOAT3 position;
      DirectX::XMFLOAT4 color;
    };

    Vertex triangleVertices[] =
    {
      {{0.0f,0.25,0.0f},{1.0f,0.0f,0.0f,1.0f}},
      {{0.25, -0.25, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f}},
      {{-0.25, -0.25, 0.0f},{0.0f, 0.0f, 1.0f, 1.0f}},
    };

    const UINT vertexBufferSize = sizeof(triangleVertices);
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
    m_vertexBuffer->Unmap(0, nullptr);
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;

  }

  ID3D12Resource* textureUploadHeap;
  {
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = 256;
    textureDesc.Height = 256;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    m_device->CreateCommittedResource(&heapProperties, 
                                      D3D12_HEAP_FLAG_NONE, 
                                      &textureDesc, 
                                      D3D12_RESOURCE_STATE_COPY_DEST, 
                                      nullptr, 
                                      IID_PPV_ARGS(&m_texture));

    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, 1);
    auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
    
    m_device->CreateCommittedResource(&uploadHeapProperties,
                                      D3D12_HEAP_FLAG_NONE,
                                      &uploadResourceDesc,
                                      D3D12_RESOURCE_STATE_GENERIC_READ,
                                      nullptr,
                                      IID_PPV_ARGS(&textureUploadHeap));
    std::vector<UINT8> textureData;
    textureData.resize(256 * 256, 4);
    D3D12_SUBRESOURCE_DATA textureSubresource = {};
    textureSubresource.pData = &textureData[0];
    textureSubresource.RowPitch = 256 * 4;
    textureSubresource.SlicePitch = textureSubresource.RowPitch * 256;

    UpdateSubresources(m_commandList, m_texture, textureUploadHeap, 0, 0, 1, &textureSubresource);
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    m_commandList->ResourceBarrier(1, &barrier);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    m_device->CreateShaderResourceView(m_texture, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
  }

  {
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fenceValue = 1;
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr) {
      HRESULT_FROM_WIN32(GetLastError());
    }
    waitForPreviousFrame();
  }

}

void populateCommandList() {
  m_commandAllocator->Reset();
  m_commandList->Reset(m_commandAllocator, m_pipelineState);

  m_commandList->SetGraphicsRootSignature(m_rootSignature);
  m_commandList->RSSetViewports(1, m_viewport);
  D3D12_RECT scissorRect = { 0,0, static_cast<LONG>(m_viewport->Width), static_cast<LONG>(m_viewport->Height) };
  m_commandList->RSSetScissorRects(1, &scissorRect);


  auto pBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()],
                                                       D3D12_RESOURCE_STATE_PRESENT,
                                                       D3D12_RESOURCE_STATE_RENDER_TARGET);
  m_commandList->ResourceBarrier(1,
                                 &pBarrier);

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
                                          m_swapChain->GetCurrentBackBufferIndex(),
                                          m_rtvDescriptorSize);
  const float rgba[4] = { 0,0,1,1 };

  m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

  m_commandList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);
  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_commandList->DrawInstanced(3, 1, 0, 0);


  pBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()],
                                                  D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                  D3D12_RESOURCE_STATE_PRESENT);
  m_commandList->ResourceBarrier(1, 
                                 &pBarrier);

}

void waitForPreviousFrame() {
  const UINT64 fence = m_fenceValue;
  m_commandQueue->Signal(m_fence, fence);
  ++m_fenceValue;
  if (m_fence->GetCompletedValue() < fence) {
    m_fence->SetEventOnCompletion(fence, m_fenceEvent);
    WaitForSingleObject(m_fenceEvent, INFINITE);
  }
}

void update(float deltaTime) {

}

void render() {
  populateCommandList();
  ID3D12CommandList* ppCommandLists[] = { m_commandList };
  m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

  m_swapChain->Present(1, 0);
  waitForPreviousFrame();

}