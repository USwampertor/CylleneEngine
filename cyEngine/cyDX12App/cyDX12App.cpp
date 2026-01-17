// cyDX12App.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "cyDX12App.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <d3dx12.h>
#include <DirectXMath.h> 
#include <dxcapi.h>

#include <cyCorePrerequisites.h>

#define MAX_LOADSTRING 100

using namespace CYLLENE_SDK;

// Global Variables:
HWND m_hwnd;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Pipeline variables
IDXGIFactory4* m_dxgiFactory = nullptr;
ID3D12Device5* m_device = nullptr;
ID3D12CommandQueue* m_commandQueue = nullptr;
ID3D12CommandAllocator* m_commandAllocator = nullptr;
ID3D12DescriptorHeap* m_rtvHeap = nullptr;
ID3D12DescriptorHeap* m_srvHeap;
ID3D12DescriptorHeap* m_uavHeap;

// Asset variables
ID3D12GraphicsCommandList4* m_commandList;
ID3D12StateObject* m_pipelineState = nullptr;
ID3D12RootSignature* m_rootSignature = nullptr;
// ID3D12Resource* m_vertexBuffer;
// D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
CD3DX12_VIEWPORT* m_viewport;
// ID3D12Resource* m_texture;

ID3D12Resource* m_quadVB;
ID3D12Resource* m_cubeVB;
ID3D12Resource* m_cubeIB;

ID3D12Resource* m_quadBLAS;
ID3D12Resource* m_cubeBLAS;

ID3D12Resource* m_tlas;
ID3D12Resource* m_tlasUpdateScratch;

constexpr UINT64 NUM_SHADER_IDS = 3;
ID3D12Resource* m_shaderIDs;

ID3D12Resource* m_instances;
D3D12_RAYTRACING_INSTANCE_DESC* m_instanceData;


IDXGISwapChain3* m_swapChain = nullptr;
ID3D12Resource* m_renderTargets[2] = { nullptr, nullptr };
UINT m_rtvDescriptorSize = 0;

HANDLE m_fenceEvent;
ID3D12Fence* m_fence;
UINT64 m_fenceValue;

void render();
void loadAssets();
void update(float deltaTime);
void initPipeline();
void getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
void waitForPreviousFrame();
ID3D12Resource* makeAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs,
                                         UINT64* updateScratchSize = nullptr);
ID3D12Resource* makeBLAS(ID3D12Resource* vertexBuffer,
                         UINT vertexCount,
                         ID3D12Resource* indexBuffer,
                         UINT indexCount);

ID3D12Resource* makeTLAS(ID3D12Resource* INSTANCES, 
                         UINT NUMiNSTANCES, 
                         UINT64* updateScratchSize);

void initBLAS();

void initTLAS();

void updateTransforms();

IDxcBlob* compileShader(LPCWSTR filePath);

void flush();

void resize(HWND hwnd);

void initShaderTable();

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

  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

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
    // update(0.016f);
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
                         D3D_FEATURE_LEVEL_12_1,
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

//     D3D12_DESCRIPTOR_HEAP_DESC uavHeapDesc = {};
//     uavHeapDesc.NumDescriptors = 1;
//     uavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//     uavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//     hr = m_device->CreateDescriptorHeap(&uavHeapDesc, IID_PPV_ARGS(&m_uavHeap));
//     if (FAILED(hr)) {
//       return FALSE;
//     }

  }

  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT n = 0; n < 2; ++n) {
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
  case WM_SIZE: {
    resize(hWnd);
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

constexpr float quadVertex[] = {
      -1.0f,  0.0f, -1.0f, -1.0f,  0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
       1.0f,  0.0f, -1.0f,  1.0f,  0.0f, -1.0f, 1.0f,  0.0f, 1.0f
};

constexpr float cubeVertex[] = {
  -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  1.0f
};

constexpr short cubeIdx[] = {
  4,6,0,2,0,6,0,1,4,5,4,1,
  0,2,1,3,1,2,1,3,5,7,5,3,
  2,6,3,7,3,6,4,5,6,7,6,5
};

constexpr DXGI_SAMPLE_DESC no_aa = { 1,0 };
constexpr D3D12_HEAP_PROPERTIES uploadHeapProps = {
  .Type = D3D12_HEAP_TYPE_UPLOAD,
};
constexpr D3D12_HEAP_PROPERTIES defaultHeapProps = {
  .Type = D3D12_HEAP_TYPE_DEFAULT,
};
constexpr D3D12_RESOURCE_DESC basic_buffer_desc = {
  .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
  .Width = 0,
  .Height = 1,
  .DepthOrArraySize = 1,
  .MipLevels = 1,
  .SampleDesc = no_aa,
  .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
};

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
                       nullptr);

    D3DCompileFromFile(L"shader.hlsl",
                       nullptr,
                       D3D_COMPILE_STANDARD_FILE_INCLUDE,
                       "PSMain",
                       "ps_5_0",
                       compileFlags,
                       0,
                       &pixelShader,
                       nullptr);

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
    m_device->CreateCommandList1(0, 
                                D3D12_COMMAND_LIST_TYPE_DIRECT,
                                D3D12_COMMAND_LIST_FLAG_NONE,
                                // m_commandAllocator, 
                                // m_pipelineState, 
                                IID_PPV_ARGS(&m_commandList));
    m_commandList->Close();
  }
  {
    struct Vertex
    {
      DirectX::XMFLOAT3 position;
      DirectX::XMFLOAT4 color;
    };

    // Vertex triangleVertices[] =
    // {
    //   {{0.0f,0.25,0.0f},{1.0f,0.0f,0.0f,1.0f}},
    //   {{0.25, -0.25, 0.0f},{0.0f, 1.0f, 0.0f, 1.0f}},
    //   {{-0.25, -0.25, 0.0f},{0.0f, 0.0f, 1.0f, 1.0f}},
    // };

    

    

    auto makeAndCopy = [](auto& data) {
      auto desc = basic_buffer_desc;
      desc.Width = sizeof(data);
      ID3D12Resource* res;
      m_device->CreateCommittedResource(&uploadHeapProps,
                                        D3D12_HEAP_FLAG_NONE,
                                        &desc,
                                        D3D12_RESOURCE_STATE_GENERIC_READ,
                                        nullptr,
                                        IID_PPV_ARGS(&res));

      void* pData;
      res->Map(0, nullptr, &pData);
      memcpy(pData, data, sizeof(data));
      res->Unmap(0, nullptr);
      return res;

    };
    m_quadVB = makeAndCopy(quadVertex);
    m_cubeVB = makeAndCopy(cubeVertex);
    m_cubeIB = makeAndCopy(cubeIdx);

    // const UINT vertexBufferSize = sizeof(triangleVertices);
    // 
    // auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    // auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    // m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));
    // UINT8* pVertexDataBegin;
    // CD3DX12_RANGE readRange(0, 0);
    // m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
    // memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
    // m_vertexBuffer->Unmap(0, nullptr);
    // m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    // m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    // m_vertexBufferView.SizeInBytes = vertexBufferSize;

  }

  ID3D12Resource* textureUploadHeap;
  {
    // D3D12_RESOURCE_DESC textureDesc = {};
    // textureDesc.MipLevels = 1;
    // textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    // textureDesc.Width = 256;
    // textureDesc.Height = 256;
    // textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    // textureDesc.DepthOrArraySize = 1;
    // textureDesc.SampleDesc.Count = 1;
    // textureDesc.SampleDesc.Quality = 0;
    // textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    // 
    // auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    // m_device->CreateCommittedResource(&heapProperties, 
    //                                   D3D12_HEAP_FLAG_NONE, 
    //                                   &textureDesc, 
    //                                   D3D12_RESOURCE_STATE_COPY_DEST, 
    //                                   nullptr, 
    //                                   IID_PPV_ARGS(&m_texture));
    // 
    // const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, 1);
    // auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    // auto uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
    // 
    // m_device->CreateCommittedResource(&uploadHeapProperties,
    //                                   D3D12_HEAP_FLAG_NONE,
    //                                   &uploadResourceDesc,
    //                                   D3D12_RESOURCE_STATE_GENERIC_READ,
    //                                   nullptr,
    //                                   IID_PPV_ARGS(&textureUploadHeap));
    // Vector<UINT8> textureData;
    // textureData.resize(256 * 256, 4);
    // D3D12_SUBRESOURCE_DATA textureSubresource = {};
    // textureSubresource.pData = &textureData[0];
    // textureSubresource.RowPitch = 256 * 4;
    // textureSubresource.SlicePitch = textureSubresource.RowPitch * 256;
    // 
    // UpdateSubresources(m_commandList, m_texture, textureUploadHeap, 0, 0, 1, &textureSubresource);
    // auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    // m_commandList->ResourceBarrier(1, &barrier);
    // 
    // D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    // srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    // srvDesc.Format = textureDesc.Format;
    // srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    // srvDesc.Texture2D.MipLevels = 1;
    // m_device->CreateShaderResourceView(m_texture, &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
  }

  {
    m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
  //   m_fenceValue = 1;
  //   m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  //   if (m_fenceEvent == nullptr) {
  //     HRESULT_FROM_WIN32(GetLastError());
  //   }
  //   waitForPreviousFrame();
  }


  // Updated section 

  initBLAS();
  constexpr UINT numInstances = 3;

  {
    auto instanceDesc = basic_buffer_desc;
    instanceDesc.Width = sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * numInstances;
    m_device->CreateCommittedResource(&uploadHeapProps,
                                      D3D12_HEAP_FLAG_NONE,
                                      &instanceDesc,
                                      D3D12_RESOURCE_STATE_GENERIC_READ,
                                      nullptr,
                                      IID_PPV_ARGS(&m_instances));

    m_instances->Map(0, nullptr, reinterpret_cast<void**>(&m_instanceData));
    for (UINT i = 0; i < numInstances; ++i) {
      auto& desc = m_instanceData[i];
      desc = {
        .InstanceID = i,
        .InstanceMask = 1, 
        .AccelerationStructure = ( i ? m_quadBLAS : m_cubeBLAS)->GetGPUVirtualAddress(),
      };
    }

    updateTransforms();
  }

  initTLAS();

  {
    D3D12_DESCRIPTOR_RANGE uavRange = {
      .RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
      .NumDescriptors = 1,
    };

    D3D12_ROOT_PARAMETER rootParameters[] = {
      {
        .ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
        .DescriptorTable = {
          .NumDescriptorRanges = 1,
          .pDescriptorRanges = &uavRange,
        },
      },

      {
        .ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
        .Descriptor = {
          .ShaderRegister = 0,
          .RegisterSpace = 0,
        },
      }
    };

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {
      .NumParameters = _countof(rootParameters),
      .pParameters = rootParameters,
    };

    ID3DBlob* signature;
    ID3DBlob* error;
    
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc,
                                             D3D_ROOT_SIGNATURE_VERSION_1_0,
                                             &signature,
                                             &error);

    if (error != nullptr) {
      OutputDebugStringA( (char*)error->GetBufferPointer() );
      error->Release();
    }

    m_device->CreateRootSignature(0,
                                  signature->GetBufferPointer(),
                                  signature->GetBufferSize(),
                                  IID_PPV_ARGS(&m_rootSignature));

    signature->Release();

  }
  initPipeline();
  initShaderTable();

}

void initPipeline() {

  auto pShaderLib = compileShader(L"RayTracingShaders.hlsl");

  // Initialize the pipeline state
  D3D12_DXIL_LIBRARY_DESC dxilLibDesc = {
    .DXILLibrary = {
      .pShaderBytecode = pShaderLib->GetBufferPointer(),
      .BytecodeLength = pShaderLib->GetBufferSize(),
    },

  };

  D3D12_HIT_GROUP_DESC hitGroupDesc = {
    .HitGroupExport = L"HitGroup",
    .Type = D3D12_HIT_GROUP_TYPE_TRIANGLES,
    .ClosestHitShaderImport = L"ClosestHit"
  };

  D3D12_RAYTRACING_SHADER_CONFIG shaderConfig = {
    .MaxPayloadSizeInBytes = 5 * sizeof(float),
    .MaxAttributeSizeInBytes = 2 * sizeof(float),
  };

  D3D12_GLOBAL_ROOT_SIGNATURE globalRootSig = {
    .pGlobalRootSignature = m_rootSignature
  };

    D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfig = {
    .MaxTraceRecursionDepth = 3
  };

  D3D12_STATE_OBJECT_DESC stateObjectDesc = {
    .Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE,
    .NumSubobjects = 5,
    .pSubobjects = new D3D12_STATE_SUBOBJECT[5]{
      {.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY, .pDesc = &dxilLibDesc},
      {.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP, .pDesc = &hitGroupDesc},
      {.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG, .pDesc = &shaderConfig},
      {.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE, .pDesc = &globalRootSig},
      {.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG, .pDesc = &pipelineConfig},
    }
  };

  m_device->CreateStateObject(&stateObjectDesc, IID_PPV_ARGS(&m_pipelineState));

  // m_commandAllocator->Reset();
  // m_commandList->Reset(m_commandAllocator, m_pipelineState);
  // 
  // m_commandList->SetGraphicsRootSignature(m_rootSignature);
  // m_commandList->RSSetViewports(1, m_viewport);
  // D3D12_RECT scissorRect = { 0,0, static_cast<LONG>(m_viewport->Width), static_cast<LONG>(m_viewport->Height) };
  // m_commandList->RSSetScissorRects(1, &scissorRect);
  // 
  // 
  // auto pBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()],
  //                                                      D3D12_RESOURCE_STATE_PRESENT,
  //                                                      D3D12_RESOURCE_STATE_RENDER_TARGET);
  // m_commandList->ResourceBarrier(1,
  //                                &pBarrier);
  // 
  // CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
  //                                         m_swapChain->GetCurrentBackBufferIndex(),
  //                                         m_rtvDescriptorSize);
  // const float rgba[4] = { 0,0,1,1 };
  // 
  // m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
  // 
  // m_commandList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);
  // m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // m_commandList->DrawInstanced(3, 1, 0, 0);
  // 
  // 
  // auto pBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()],
  //                                                 D3D12_RESOURCE_STATE_RENDER_TARGET,
  //                                                 D3D12_RESOURCE_STATE_PRESENT);
  // m_commandList->ResourceBarrier(1, 
  //                                &pBarrier2);
  // m_commandList->Close();

}

void initShaderTable() {
  auto idDesc = basic_buffer_desc;
  idDesc.Width = NUM_SHADER_IDS * D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
  m_device->CreateCommittedResource(&uploadHeapProps, 
                                    D3D12_HEAP_FLAG_NONE, 
                                    &idDesc, 
                                    D3D12_RESOURCE_STATE_GENERIC_READ, 
                                    nullptr, 
                                    IID_PPV_ARGS(&m_shaderIDs));

  ID3D12StateObjectProperties* stateObjectProps;
  m_pipelineState->QueryInterface(IID_PPV_ARGS(&stateObjectProps));

  void* pData;

  auto writeID = [&](const wchar_t* name) {
    auto id = stateObjectProps->GetShaderIdentifier(name);
    memcpy(reinterpret_cast<uint8_t*>(pData), id, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
    pData = static_cast<char*>(pData) + D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

  };

  m_shaderIDs->Map(0, nullptr, &pData);
  writeID(L"RayGeneration");
  writeID(L"Miss");
  writeID(L"ClosestHit");
  m_shaderIDs->Unmap(0, nullptr);

  stateObjectProps->Release();
}

ID3D12Resource* makeAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& inputs, 
                                          UINT64* updateScratchSize) {
  auto makeBuffer = [](UINT64 size, auto initialState) {
    auto desc = basic_buffer_desc;
    desc.Width = size;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    ID3D12Resource* buffer;
    m_device->CreateCommittedResource(&defaultHeapProps,
                                      D3D12_HEAP_FLAG_NONE,
                                      &desc,
                                      initialState,
                                      nullptr,
                                      IID_PPV_ARGS(&buffer));
    return buffer;
  };

  D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo = {};
  m_device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuildInfo);

  if (updateScratchSize) {
    *updateScratchSize = prebuildInfo.UpdateScratchDataSizeInBytes;
  }

  auto* scratch = makeBuffer(prebuildInfo.ScratchDataSizeInBytes, 
                             D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
  auto* accelStruct = makeBuffer(prebuildInfo.ResultDataMaxSizeInBytes, 
                                 D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE);

  D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = {
    .DestAccelerationStructureData = accelStruct->GetGPUVirtualAddress(),
    .Inputs = inputs,
    .ScratchAccelerationStructureData = scratch->GetGPUVirtualAddress(),
  };

  m_commandAllocator->Reset();
  m_commandList->Reset(m_commandAllocator, nullptr);
  m_commandList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);
  m_commandList->Close();
  m_commandList->Close();
  m_commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&m_commandList));
  flush();
  scratch->Release();

  return accelStruct;


}

void updateTransforms() {
  using namespace DirectX;
  auto setMatrix = [](int idx, const XMMATRIX& mat) {
    auto ptr = reinterpret_cast<XMFLOAT3X4*>(
      m_instanceData[idx].Transform
      );
    XMStoreFloat3x4(ptr, mat);
  };

  auto time = static_cast<float>(GetTickCount64() / 1000.0f);
  auto cubeMat = XMMatrixRotationRollPitchYaw(time / 2.0f, time / 3.0f, time / 5.0f);
  cubeMat *= XMMatrixTranslation(-1.5f, 2.0f, 2.0f);

  setMatrix(0, cubeMat);

  auto floorMat = XMMatrixScaling(5.0f, 5.0f, 5.0f); // XMMatrixRotationX(-1.0f);
  floorMat *= XMMatrixRotationY(XMScalarASinEst(time) / 8.0f + 1.0f);
  floorMat *= XMMatrixTranslation(-1.5f, 2.0f, 2.0f);
  setMatrix(1, floorMat);


  auto mirrorMat = XMMatrixRotationX(-1.0f);
  mirrorMat *= XMMatrixRotationY(XMScalarASinEst(time) / 8.0f + 1.0f);
  mirrorMat *= XMMatrixTranslation(2.0f, 2.0f, 2.0f);
  setMatrix(1, mirrorMat);

}

IDxcBlob* compileShader(LPCWSTR filePath) {
  IDxcUtils* pUtils = nullptr;
  IDxcCompiler3* pCompiler = nullptr;

  DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
  DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

  uint32 codePage = DXC_CP_UTF8;
  IDxcBlobEncoding* pSource = nullptr;
  pUtils->LoadFile(filePath, &codePage, &pSource);

  DxcBuffer sourceBuffer;
  sourceBuffer.Ptr = pSource->GetBufferPointer();
  sourceBuffer.Size = pSource->GetBufferSize();
  sourceBuffer.Encoding = codePage;

  Vector<LPCWSTR> arguments = {
    L"-T", L"lib_6_3", // target profile
    L"-O3",         // optimization level 3
    L"-Zi",
    L"Qstrip_reflect"
  };

  IDxcResult* pResults = nullptr;

  pCompiler->Compile(&sourceBuffer,
                     arguments.data(),
                     static_cast<UINT32>(arguments.size()),
                     nullptr,
                     IID_PPV_ARGS(&pResults));

  IDxcBlob* pError = nullptr;
  pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pError), nullptr);
  if (pError != nullptr && pError->GetBufferSize() != 0) {
    OutputDebugStringA(static_cast<LPCSTR>(pError->GetBufferPointer()));
  }

  HRESULT hr;
  pResults->GetStatus(&hr);
  if (FAILED(hr)) {
    return nullptr;
  }

  IDxcBlob* pShaderBlob = nullptr;
  pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShaderBlob), nullptr);

  pUtils->Release();
  pCompiler->Release();
  pSource->Release();
  pResults->Release();

  return pShaderBlob;

}

void flush() {
  static UINT64 value = 1;
  m_commandQueue->Signal(m_fence, value);
  m_fence->SetEventOnCompletion(value++, nullptr);
}

void waitForPreviousFrame() {
  const UINT64 fence = m_fenceValue;
  m_commandQueue->Signal(m_fence, fence);
  m_fenceValue++;
  if (m_fence->GetCompletedValue() < fence) {
    m_fence->SetEventOnCompletion(fence, m_fenceEvent);
    WaitForSingleObject(m_fenceEvent, INFINITE);
  }
}

void resize(HWND hwnd) {
  if (!m_swapChain) {
    return;
  }

  RECT rect;
  GetClientRect(hwnd, &rect);
  auto width = std::max(rect.right - rect.left, static_cast<LONG>(8));
  auto height = std::max(rect.bottom - rect.top, static_cast<LONG>(8));
  flush();
  m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

  if (m_renderTargets) [[likely]]
    m_renderTargets->Release();

  D3D12_RESOURCE_DESC rtDesc = {
    .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
    .Width = static_cast<UINT64>(width),
    .Height = static_cast<UINT>(height),
    .DepthOrArraySize = 1
    .MipLevels = 1,
    .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
    .SampleDesc = no_aa,
    .Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS
  };

  m_device->CreateCommittedResource(&defaultHeapProps, 
    D3D12_HEAP_FLAG_NONE, 
    &rtDesc, 
    D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 
    nullptr, 
    IID_PPV_ARGS(&m_renderTargets));

  D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {
    .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
    .ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D
  };

  m_device->CreateUnorderedAccessView(m_renderTargets, 
                                      nullptr, 
                                      &uavDesc, 
                                      m_srvHeap->GetCPUDescriptorHandleForHeapStart());



}

void update(float deltaTime) {
  constexpr UINT numInstances = 3;
  updateTransforms();

  D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc = {
    .DestAccelerationStructureData = m_tlas->GetGPUVirtualAddress(),
    .Inputs = {
      .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
      .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE,
      .NumDescs = numInstances,
      .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
      .InstanceDescs = m_instances->GetGPUVirtualAddress()
    },
    .SourceAccelerationStructureData = m_tlas->GetGPUVirtualAddress(),
    .ScratchAccelerationStructureData = m_tlasUpdateScratch->GetGPUVirtualAddress(),
  };

  m_commandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
  D3D12_RESOURCE_BARRIER barrier = {
    .Type = D3D12_RESOURCE_BARRIER_TYPE_UAV,
    .UAV = {
      .pResource = m_tlas
    }
  };

  m_commandList->ResourceBarrier(1, &barrier);

}

void render() {

  m_commandAllocator->Reset();
  m_commandList->Reset(m_commandAllocator, nullptr);
  update(0.0f);

  m_commandList->SetPipelineState1(m_pipelineState);
  m_commandList->SetComputeRootSignature(m_rootSignature);
  m_commandList->SetDescriptorHeaps(1, &m_srvHeap);

  auto uavTable = m_srvHeap->GetGPUDescriptorHandleForHeapStart();
  m_commandList->SetComputeRootDescriptorTable(0, uavTable);
  m_commandList->SetComputeRootShaderResourceView(1, m_tlas->GetGPUVirtualAddress());

  auto renderTarget = m_renderTargets[0];
  auto rtDesc = m_renderTargets[0]->GetDesc();

  D3D12_DISPATCH_RAYS_DESC dispatchDesc = {
    .RayGenerationShaderRecord = {
      .StartAddress = m_shaderIDs->GetGPUVirtualAddress(),
      .SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
    },
    .MissShaderTable = {
          .StartAddress = m_shaderIDs->GetGPUVirtualAddress() + 
                          D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT,
          .SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
    },
    .HitGroupTable = {
          .StartAddress = m_shaderIDs->GetGPUVirtualAddress() + 
                          2 * D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT,
          .SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES,
    },
    .Width = static_cast<UINT>(rtDesc.Width),
    .Height = rtDesc.Height,
    .Depth = 1
  };

  m_commandList->DispatchRays(&dispatchDesc);
  ID3D12Resource* backBuffer;
  m_swapChain->GetBuffer(m_swapChain->GetCurrentBackBufferIndex(),
                         IID_PPV_ARGS(&backBuffer));

  auto barrier = [](auto* resource, auto before, auto after) {
    D3D12_RESOURCE_BARRIER rb = {
      .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
      .Transition = {
        .pResource = resource,
        .StateBefore = before,
        .StateAfter = after
      },
    };
    m_commandList->ResourceBarrier(1, &rb);
  };

  barrier(renderTarget, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
  barrier(backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_SOURCE);
  m_commandList->CopyResource(backBuffer, renderTarget);
  
  barrier(backBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
  barrier(renderTarget, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

  backBuffer->Release();
  m_commandList->Close();
  m_commandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList**>(&m_commandList));
  flush();
  m_swapChain->Present(1, 0);
  // initPipeline();
  // ID3D12CommandList* ppCommandLists[] = { m_commandList };
  // m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
  // 
  // m_swapChain->Present(1, 0);
  // waitForPreviousFrame();

}

ID3D12Resource* makeBLAS(ID3D12Resource* vertexBuffer,
                         UINT vertexCount,
                         ID3D12Resource* indexBuffer = nullptr,
                         UINT indexCount = 0) {
  D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {
    .Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES,
    .Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE,
    .Triangles = {
      .Transform3x4 = 0,
      .IndexFormat = indexBuffer ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_UNKNOWN,
      .VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT,
      .IndexCount = indexCount,
      .VertexCount = vertexCount,
      .IndexBuffer = indexBuffer ? indexBuffer->GetGPUVirtualAddress() : 0,
      .VertexBuffer = {
        .StartAddress = vertexBuffer->GetGPUVirtualAddress(),
        .StrideInBytes = sizeof(float) * 3
      }
    }
  };

  D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {
    .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL,
    .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
    .NumDescs = 1,
    .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
    .pGeometryDescs = &geometryDesc
  };

  return makeAccelerationStructure(inputs);

}

void initBLAS() {
  m_quadBLAS = makeBLAS(m_quadVB,
                        std::size(quadVertex));
  m_cubeBLAS = makeBLAS(m_cubeVB,
                        std::size(cubeVertex), 
                        m_cubeIB, 
                        std::size(cubeIdx));
}

ID3D12Resource* makeTLAS(ID3D12Resource* instances, 
                         UINT numInstances, 
                         UINT64* updateScratchSize = nullptr) {
  D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {
    .Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL,
    .Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE,
    .NumDescs = numInstances,
    .DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY,
    .InstanceDescs = instances->GetGPUVirtualAddress(),
  };

  return makeAccelerationStructure(inputs, updateScratchSize);
}

void initTLAS() {

  constexpr UINT numInstances = 3;
  UINT64 updateScratchSize = 0;

  m_tlas = makeTLAS(m_instances,
                    numInstances,
                    &updateScratchSize);

  auto desc = basic_buffer_desc;
  desc.Width = std::max(updateScratchSize, 8ULL);
  desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
  m_device->CreateCommittedResource(&defaultHeapProps,
                                    D3D12_HEAP_FLAG_NONE,
                                    &desc,
                                    D3D12_RESOURCE_STATE_COMMON,
                                    nullptr,
                                    IID_PPV_ARGS(&m_tlasUpdateScratch));



}