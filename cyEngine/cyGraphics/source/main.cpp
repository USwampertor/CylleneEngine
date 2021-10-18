#include <chrono>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FreeImage/FreeImage.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <OIS/OIS.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <tchar.h>

#include "cyTime.h"
#include "cyVector2f.h"
#include "cyVector3f.h"
#include "cyFileSystem.h"
#include "cyMatrix4x4.h"
#include "cyQuaternion.h"

#define durationCast std::chrono::duration_cast

using nanosecs = std::chrono::nanoseconds;
using millisecs = std::chrono::milliseconds;
using steady_clock = std::chrono::steady_clock;
using namespace CYLLENE_SDK;

ID3D11Device* device;
IDXGISwapChain1* swapchain;
ID3D11DeviceContext* context;
ID3D11Resource* backbuffer;
ID3D11RenderTargetView* renderTargetView;
D3D11_VIEWPORT viewport;
ID3D11RasterizerState* rasterizer;
ID3D11DepthStencilState* DSState;
ID3D11Texture2D* depthStencil;
ID3D11DepthStencilView* DSV;
ID3DBlob* cs_blob;
ID3D11ComputeShader* csShader;
ID3DBlob* vs_blob;
ID3D11VertexShader* vsShader;
ID3DBlob* fs_blob;
ID3D11PixelShader* psShader;
ID3D11InputLayout* inputLayout;
ID3D11SamplerState* samplerState;
ID3D11UnorderedAccessView* uavBuffer;
ID3D11ShaderResourceView* srvuav;
ID3D11Buffer* computebuffer;
ID3D11Buffer* constantbuffer;
ID3D11Buffer* vertexbuffer;
ID3D11Buffer* uvbuffer;
ID3D11Buffer* colorbuffer;
ID3D11Buffer* indexbuffer;
ID3D11Texture2D* tex;
ID3D11ShaderResourceView* checkerTexture;
float g_aspectRatio;

struct Vertex
{
  Vector3f position;
  Vector2f uv;
  Vector3f color;
};

struct Mesh
{
  Vertex* vertices;
  Vector3f* indices;
  int32 nVertices;
  int32 nIndices;
};

struct Model
{
  Mesh* meshes;
};

void
ErrorDescription(HRESULT hr) {
  if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
    hr = HRESULT_CODE(hr);
  TCHAR* szErrMsg;

  if (FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&szErrMsg, 0, NULL) != 0)
  {
    _tprintf(TEXT("%s"), szErrMsg);
    LocalFree(szErrMsg);
  }
  else
    _tprintf(TEXT("[Could not find a description for error # %#x.]\n"), hr);
}

bool
DoTheImportThing(const std::string& pFile);
/*
void
printProgramLog(uint32 program);

void
printShaderLog(uint32 shader);
*/

//Starts up SDL, creates window, and initializes OpenGL
bool
init(String basepath,
     int clientWidth,
     int clientHeight,
     SDL_Window** window,
     SDL_GLContext& glContext,
     uint32& gProgramID,
     uint32* gVBO,
     uint32& gIBO,
     int32& gVertexPosition2D,
     int32& gUV,
     int32& gVertexColor);

//Initializes matrices and clear color
bool
initDX11(String basepath,
         uint32& gProgramID,
         uint32* gVBO,
         uint32& gIBO,
         int32& gVertexPosition2D,
         int32& gUV,
         int32& gVertexColor);

void
CreateComputeShader(String path);

void
CreateComputeBuffer();

void
render(uint32& gProgramID,
       uint32* gVBO,
       uint32& gIBO,
       Matrix4x4 View,
       Matrix4x4 Projection,
       int32& gVertexPosition2D,
       int32& gUV,
       int32& gVertexColor,
       unsigned int texture);

// Define your user buttons somewhere global
enum Button
{
  ButtonConfirm
};

int
main(int argc, char** argv) {
  int clientWidth = 1280, clientHeight = 720;

  if (argc > 0) {
    printf("Application args:\n");

    for (int i = 0; i < argc; ++i) {
      printf("arg[%d]: %s\n", i, argv[i]);

      if (std::strcmp(argv[i], "-w") == 0 ||
          std::strcmp(argv[i], "--width") == 0) {
        clientWidth = std::stoi(argv[++i]);
      }
      if (std::strcmp(argv[i], "-h") == 0 ||
          std::strcmp(argv[i], "--height") == 0) {
        clientHeight = std::stoi(argv[++i]);
      }
    }
  }

  printf("\n");

  std::string currentDirectory(argv[0]);
  currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of('\\') + 1);
  printf("Current path: %s\n", currentDirectory.c_str());

  printf("\n");
  printf("Starting app with resolution %dx%d.\n", clientWidth, clientHeight);
  printf("\n");

  //The window we'll be rendering to
  SDL_Window* window = nullptr;
  SDL_GLContext glContext;
  //SDL_Surface* windowSurface = nullptr;

  /***************************************************************************/
  // OpenGL Shader ID
  uint32 gProgramID = 0;
  // Vertex Atributtes Position
  int32 gVertexPosition2D = -1;
  // Vertex Atributtes UV
  int32 gUV = -1;
  // Vertex Atributtes Color
  int32 gVertexColor = -1;
  // OpenGL Vertex Buffer Object
  uint32 gVBO[3] = { 0, 0, 0 };
  // OpenGL Index Buffer Object
  uint32 gIBO = 0;

  if (!init(currentDirectory,
            clientWidth,
            clientHeight,
            &window,
            glContext,
            gProgramID,
            gVBO,
            gIBO,
            gVertexPosition2D,
            gUV,
            gVertexColor)) {
    return -1;
  }

  // get version info
  //printf("Renderer: %s\n", glGetString(GL_RENDERER));
  //printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

  printf("\n");

  //unsigned int texture;
  //glGenTextures(1, &texture);
  //glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture object)

  /***************************************************************************/
  FreeImage_Initialise();

  printf("FreeImage version: %s\n", FreeImage_GetVersion());
  std::string textureName = currentDirectory + "../../Checker.webp";
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(textureName.c_str());

  FIBITMAP* pImage = FreeImage_Load(fif, textureName.c_str(), 0);
  if (pImage) {
    FIBITMAP* pImageRGBA = FreeImage_ConvertToRGBAF(pImage);
    int nWidth = FreeImage_GetWidth(pImageRGBA);
    int nHeight = FreeImage_GetHeight(pImageRGBA);

    FREE_IMAGE_COLOR_TYPE fict = FreeImage_GetColorType(pImageRGBA);
    uint32 pitch = FreeImage_GetPitch(pImageRGBA);
    BITMAPINFO* imageInfo = FreeImage_GetInfo(pImageRGBA);

    printf("Format: %i\n", fif);
    printf("Width: %i\n", nWidth);
    printf("Height: %i\n", nHeight);
    printf("Bit count: %i\n", imageInfo->bmiHeader.biBitCount);
    printf("Size: %i\n", imageInfo->bmiHeader.biSize);
    printf("Image size: %i\n", imageInfo->bmiHeader.biSizeImage);
    printf("Image byte pitch: %i\n", pitch);
    printf("Color type: %i\n", fict);

    BYTE* imageBytes = FreeImage_GetBits(pImageRGBA);
    
    D3D11_SUBRESOURCE_DATA initData = { imageBytes, pitch, pitch * nHeight };

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = nWidth;
    desc.Height = nHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    HRESULT HRTexture = device->CreateTexture2D(&desc, &initData, &tex);

    if (FAILED(HRTexture)) {
      printf("Texture couldn't be created: ");
      ErrorDescription(HRTexture);
      return -1;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    HRTexture = device->CreateShaderResourceView(tex,
                                                 &SRVDesc,
                                                 &checkerTexture);

    if (FAILED(HRTexture)) {
      printf("Shader resource view couldn't be created: ");
      ErrorDescription(HRTexture);
      return -1;
    }

    FreeImage_Unload(pImage);
    FreeImage_Unload(pImageRGBA);
  }
  else {
    printf("Error while loading image\n");
  }
  

  FreeImage_DeInitialise();
  /***************************************************************************/

  printf("\n");

  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(window, &wmInfo);

  // Inputs
  OIS::ParamList pl;
  size_t windowHnd = reinterpret_cast<size_t>(wmInfo.info.win.window);
  std::ostringstream windowHndStr;

  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
  OIS::InputManager* m_InputManager = OIS::InputManager::createInputSystem(pl);
  OIS::Mouse* m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject(OIS::OISMouse, false));
  OIS::Keyboard* m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, false));

  const OIS::MouseState& ms = m_Mouse->getMouseState();
  ms.width = clientWidth;
  ms.height = clientHeight;

  Vector4f CamPosition(0, 0, -10, 1);
  Vector3f CamRight(1, 0, 0);
  Vector3f CamForward(0, 0, 1);

  Matrix4x4 Projection;
  Projection.Perspective(1920.0f, 1080.0f, 0.01f, 1000.0f, 60.0f * 0.0174533f);
  //Projection.Orthogonal(19.2f, 10.8f, 0.01f, 1000.0);

  bool open = true;
  double deltaTime = 0.0;
  steady_clock::time_point lastFrame = steady_clock::now();

  while (open) {
    SDL_Event event;
    if (SDL_PollEvent(&event) > 0) {
      switch (event.type)
      {
        case SDL_QUIT:
          open = false;
          break;
        default:
          break;
      }
    }

    m_Keyboard->capture();
    m_Mouse->capture();

    if (m_Keyboard->isKeyDown(OIS::KC_ESCAPE)) { open = false; }

    bool keyboardW = m_Keyboard->isKeyDown(OIS::KC_W);
    bool keyboardA = m_Keyboard->isKeyDown(OIS::KC_A);
    bool keyboardS = m_Keyboard->isKeyDown(OIS::KC_S);
    bool keyboardD = m_Keyboard->isKeyDown(OIS::KC_D);
    bool keyboardQ = m_Keyboard->isKeyDown(OIS::KC_Q);
    bool keyboardE = m_Keyboard->isKeyDown(OIS::KC_E);

    float mouseDeltaX = m_Mouse->getMouseState().X.rel * deltaTime;
    float mouseDeltaY = m_Mouse->getMouseState().Y.rel * deltaTime;

    if (keyboardW) CamPosition += CamForward * 10.0f * deltaTime;
    if (keyboardS) CamPosition -= CamForward * 10.0f * deltaTime;
    if (keyboardA) CamPosition -= CamRight * 10.0f * deltaTime;
    if (keyboardD) CamPosition += CamRight * 10.0f * deltaTime;
    if (keyboardE) CamPosition.y += 10.0f * deltaTime;
    if (keyboardQ) CamPosition.y -= 10.0f * deltaTime;
    
    Quaternion rotY;
    rotY.fromEuler(Vector3f(0.0f, 0.1f * mouseDeltaX, 0.0f), 0);

    CamRight = rotY.rotate(CamRight);

    Quaternion rotX;
    rotX.fromEuler(CamRight * 0.1f * mouseDeltaY, 0);

    CamForward = rotX.rotate(rotY.rotate(CamForward));

    Matrix4x4 View;
    View.View(CamPosition,
              CamPosition + CamForward,
              Vector4f(0, 1, 0, 0));

    render(gProgramID, gVBO, gIBO, View, Projection, gVertexPosition2D, gUV, gVertexColor, 0);

    SDL_GL_SwapWindow(window);

    steady_clock::time_point currentFrame = steady_clock::now();
    nanosecs frameDiff = currentFrame - lastFrame;
    long long elapsedTime = durationCast<millisecs>(frameDiff).count();

    Uint32 delay = static_cast<Uint32>(elapsedTime < 16 ? 16 - elapsedTime : 0);
    SDL_Delay(delay);

    lastFrame = currentFrame;
    
    deltaTime = elapsedTime / 1000.0;
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}

bool
DoTheImportThing(const std::string& pFile) {
  // Create an instance of the Importer class
  Assimp::Importer importer;
  
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(pFile,
                                           aiProcess_CalcTangentSpace |
                                           aiProcess_Triangulate |
                                           aiProcess_JoinIdenticalVertices |
                                           aiProcess_SortByPType);

  // If the import failed, report it
  if (!scene) {
    printf("Assimp scene import error: %s", importer.GetErrorString());
    return false;
  }

  // Now we can access the file's contents.
  //DoTheSceneProcessing(scene);

  if (!scene->HasMeshes()) {
    printf("Assimp trying to import empty model: %s", importer.GetErrorString());
    return false;
  }

  int totalMeshes = scene->mNumMeshes;
  for (int meshIndex = 0; meshIndex < totalMeshes; meshIndex++) {
    aiMesh* pMesh = scene->mMeshes[meshIndex];
    aiFace* triangle = &pMesh->mFaces[0];
    triangle->mIndices[0];
  }

  // We're done. Everything will be cleaned up by the importer destructor
  return true;
}
/*
void
printProgramLog(uint32 name) {
  // Make sure name is shader
  if (glIsProgram(name)) {
    // Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetProgramiv(name, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetProgramInfoLog(name, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
  }
  else {
    printf("Name %d is not a program\n", name);
  }
}

void
printShaderLog(uint32 name) {
  // Make sure name is shader
  if (glIsShader(name)) {
    // Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;

    // Get info string length
    glGetShaderiv(name, GL_INFO_LOG_LENGTH, &maxLength);

    // Allocate string
    char* infoLog = new char[maxLength];

    // Get info log
    glGetShaderInfoLog(name, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      printf("%s\n", infoLog);
    }

    // Deallocate string
    delete[] infoLog;
  }
  else {
    printf("Name %d is not a shader\n", name);
  }
}
*/

bool
init(String basepath,
     int clientWidth,
     int clientHeight,
     SDL_Window** window,
     SDL_GLContext& glContext,
     uint32& gProgramID,
     uint32* gVBO,
     uint32& gIBO,
     int32& gVertexPosition2D,
     int32& gUV,
     int32& gVertexColor) {
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  *window = SDL_CreateWindow("SDL CylleneEngine",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             clientWidth,
                             clientHeight,
                             SDL_WINDOW_SHOWN);

  if (window == NULL) {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Create factory
  IDXGIFactory2* pFactory;
  HRESULT HRFactory = CreateDXGIFactory(__uuidof(IDXGIFactory2),
                                        (void**)(&pFactory));

  if (FAILED(HRFactory)) {
    printf("Factory couldn't be created: ");
    ErrorDescription(HRFactory);
    return false;
  }

  // Create device
  UINT createDeviceFlags = 0;
#if _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  Vector<D3D_FEATURE_LEVEL> featureLevels = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };

  D3D_FEATURE_LEVEL featureLevel;
  HRESULT HRDevice = D3D11CreateDevice(nullptr,
                                       D3D_DRIVER_TYPE_HARDWARE,
                                       nullptr,
                                       createDeviceFlags,
                                       featureLevels.data(),
                                       featureLevels.size(),
                                       D3D11_SDK_VERSION,
                                       &device,
                                       nullptr,
                                       &context);

  if (FAILED(HRDevice)) {
    printf("Device couldn't be created: ");
    ErrorDescription(HRDevice);
    return false;
  }

  uint32 FrameCount = 2;

  // Create SwapChain
  DXGI_SAMPLE_DESC SampleDesc;
  SampleDesc.Count = 0;
  SampleDesc.Quality = 0;

  DXGI_SWAP_CHAIN_DESC1 sd;
  sd.Width = clientWidth;
  sd.Height = clientHeight;
  sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.Stereo = false;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.BufferCount = FrameCount;
  sd.Scaling = DXGI_SCALING_NONE;
  sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  sd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Flags = 0;

  DXGI_SWAP_CHAIN_FULLSCREEN_DESC sdf;
  sdf.RefreshRate.Numerator = 60;
  sdf.RefreshRate.Denominator = 1;
  sdf.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  sdf.Windowed = true;

  SDL_SysWMinfo sysWMInfo;
  SDL_VERSION(&sysWMInfo.version);
  SDL_GetWindowWMInfo(*window, &sysWMInfo);

  HWND win = sysWMInfo.info.win.window;

  HRESULT HRSwapChain = pFactory->CreateSwapChainForHwnd(device,
                                                         win,
                                                         &sd,
                                                         &sdf,
                                                         nullptr,
                                                         &swapchain);
  if (FAILED(HRSwapChain)) {
    printf("SwapChain couldn't be created: ");
    ErrorDescription(HRSwapChain);
    return false;
  }

  pFactory->Release();

  // Back buffer
  swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);

  if (backbuffer == nullptr) {
    printf("Backbuffer address is zero.");
    return false;
  }

  HRESULT HRCreateRT = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);

  if (FAILED(HRCreateRT)) {
    printf("Backbuffer Rendertarget couldn't be created: ");
    ErrorDescription(HRCreateRT);
    return false;
  }

  // Create Depth Stencil texture
  {
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = clientWidth;
    descDepth.Height = clientHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    HRESULT HRDepthStencil = device->CreateTexture2D(&descDepth, NULL, &depthStencil);

    if (FAILED(HRDepthStencil)) {
      printf("Depth Stencil texture couldn't be created: ");
      ErrorDescription(HRDepthStencil);
      return false;
    }
  }

  // Create Depth Stencil State
  {
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    //dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;

    // Stencil test parameters
    dsDesc.StencilEnable = false;
    dsDesc.StencilReadMask = 0xFF;
    dsDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing
    dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

    // Stencil operations if pixel is back-facing
    dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

    // Create depth stencil state
    device->CreateDepthStencilState(&dsDesc, &DSState);
  }

  // Create Depth Stencil view
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Flags = 0;
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    HRESULT HRDepthStencilView = device->CreateDepthStencilView(depthStencil, // Depth stencil texture
                                                                &descDSV, // Depth stencil desc
                                                                &DSV);  // [out] Depth stencil view

    if (FAILED(HRDepthStencilView)) {
      printf("Depth Stencil View couldn't be created: ");
      ErrorDescription(HRDepthStencilView);
      return false;
    }
  }

  // Viewport
  viewport.Width = clientWidth;
  viewport.Height = clientHeight;
  viewport.TopLeftX = 0;
  viewport.TopLeftY = 0;
  viewport.MinDepth = 0;
  viewport.MaxDepth = 1;

  //Initialize DirectX 11
  if (!initDX11(basepath,
                gProgramID,
                gVBO,
                gIBO,
                gVertexPosition2D,
                gUV,
                gVertexColor)) {
    printf("Unable to initialize DirectX 11!\n");
    return false;
  }

  return true;
}

bool
initDX11(String basepath,
         uint32& gProgramID,
         uint32* gVBO,
         uint32& gIBO,
         int32& gVertexPosition2D,
         int32& gUV,
         int32& gVertexColor) {
  CreateComputeShader(basepath + "Shaders/hlsl/Compute.hlsl");

  // Create vertex shader
  {
    File VSFile = FileSystem::open(basepath + "Shaders/hlsl/Vertex.hlsl");
    String VSString = VSFile.readFile();

    ID3DBlob* errorBlob = nullptr;

    uint32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    HRESULT HRVertexShader = D3DCompile(VSString.c_str(),
                                        VSString.size(),
                                        0,
                                        0,
                                        0,
                                        "main",
                                        "vs_5_0",
                                        flags,
                                        0,
                                        &vs_blob,
                                        &errorBlob);

    if (FAILED(HRVertexShader)) {
      printf("Vertex shader couldn't be compiled: ");
      printf("%s\n", (char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
      return false;
    }

    HRESULT HRCreateVS = device->CreateVertexShader(vs_blob->GetBufferPointer(),
                                                    vs_blob->GetBufferSize(),
                                                    nullptr,
                                                    &vsShader);

    if (FAILED(HRCreateVS)) {
      printf("Vertex shader couldn't be created: ");
      ErrorDescription(HRCreateVS);
      return false;
    }
  }

  // Create fragment shader
  {
    File FSFile = FileSystem::open(basepath + "Shaders/hlsl/Fragment.hlsl");
    String FSString = FSFile.readFile();

    ID3DBlob* errorBlob = nullptr;

    uint32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
    flags |= D3DCOMPILE_DEBUG;
#endif

    HRESULT HRFragmentShader = D3DCompile(FSString.c_str(),
                                          FSString.size(),
                                          0,
                                          0,
                                          0,
                                          "main",
                                          "ps_5_0",
                                          flags,
                                          0,
                                          &fs_blob,
                                          &errorBlob);

    if (FAILED(HRFragmentShader)) {
      printf("Fragment shader couldn't be compiled: ");
      printf("%s\n", (char*)errorBlob->GetBufferPointer());
      errorBlob->Release();
      return false;
    }

    HRESULT HRCreatePS = device->CreatePixelShader(fs_blob->GetBufferPointer(),
                                                   fs_blob->GetBufferSize(),
                                                   nullptr,
                                                   &psShader);

    if (FAILED(HRCreatePS)) {
      printf("Pixel shader couldn't be created: ");
      ErrorDescription(HRCreatePS);
      return false;
    }
  }

  // create the input layout
  {
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    HRESULT HRInputLayout = device->CreateInputLayout(ied,
                                                      ARRAYSIZE(ied),
                                                      vs_blob->GetBufferPointer(),
                                                      vs_blob->GetBufferSize(),
                                                      &inputLayout);

    if (FAILED(HRInputLayout)) {
      printf("Pixel shader couldn't be created: ");
      ErrorDescription(HRInputLayout);
      return false;
    }
  }

  // create rasterizer
  {
    D3D11_RASTERIZER_DESC rasterDes;
    rasterDes.FillMode = D3D11_FILL_SOLID;
    rasterDes.CullMode = D3D11_CULL_BACK;
    rasterDes.FrontCounterClockwise = false;
    rasterDes.DepthBias = 0;
    rasterDes.DepthBiasClamp = 0.0f;
    rasterDes.SlopeScaledDepthBias = 0.0f;
    rasterDes.DepthClipEnable = true;
    rasterDes.ScissorEnable = false;
    rasterDes.MultisampleEnable = false;
    rasterDes.AntialiasedLineEnable = false;

    HRESULT HRRasterizer = device->CreateRasterizerState(&rasterDes,
                                                         &rasterizer);

    if (FAILED(HRRasterizer)) {
      printf("Rasterizer couldn't be created: ");
      ErrorDescription(HRRasterizer);
      return false;
    }
  }

  // create sampler
  {
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0;

    HRESULT HRSampler = device->CreateSamplerState(&samplerDesc, &samplerState);

    if (FAILED(HRSampler)) {
      printf("Sampler state couldn't be created: ");
      ErrorDescription(HRSampler);
      return false;
    }

  }

  CreateComputeBuffer();

  // Create constant buffer
  {
    Matrix4x4 contantBuffer[] =
    {
      Matrix4x4::IDENTITY,
      Matrix4x4::IDENTITY,
    };

    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = sizeof(Matrix4x4) * ARRAYSIZE(contantBuffer);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA srd = { contantBuffer, 0, 0 };
    HRESULT HRCB = device->CreateBuffer(&bd, &srd, &constantbuffer);

    if (FAILED(HRCB)) {
      printf("Constant buffer couldn't be created: ");
      ErrorDescription(HRCB);
      return false;
    }
  }

  // Create vertex buffer
  {
    Vertex OurVertices[] =
    {
      { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
      { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
      { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },

      { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
      { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
      { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
    };
  
    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = sizeof(Vertex) * ARRAYSIZE(OurVertices);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;
  
    D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };
    HRESULT HRVertex = device->CreateBuffer(&bd, &srd, &vertexbuffer);

    if (FAILED(HRVertex)) {
      printf("Vertex buffer couldn't be created: ");
      ErrorDescription(HRVertex);
      return false;
    }
  }

  // Create index buffer
  {
    uint32 indices[] =
    {
      0, 1, 2,
      3, 4, 5,
    };

    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(uint32) * ARRAYSIZE(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA srd = { indices, 0, 0 };
    HRESULT HRColor = device->CreateBuffer(&bd, &srd, &indexbuffer);

    if (FAILED(HRColor)) {
      printf("Color buffer couldn't be created: ");
      ErrorDescription(HRColor);
      return false;
    }
  }

  return true;
}

void
CreateComputeShader(String path) {
  File CSFile = FileSystem::open(path);
  String CSString = CSFile.readFile();

  ID3DBlob* errorBlob = nullptr;

  uint32 flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
  flags |= D3DCOMPILE_DEBUG;
#endif

  HRESULT HRCS = D3DCompile(CSString.c_str(),
                            CSString.size(),
                            0,
                            0,
                            0,
                            "main",
                            "cs_5_0",
                            flags,
                            0,
                            &cs_blob,
                            &errorBlob);

  if (FAILED(HRCS)) {
    printf("Compute shader couldn't be compiled: ");
    printf("%s\n", (char*)errorBlob->GetBufferPointer());
    errorBlob->Release();
    return;
  }

  HRESULT HRCreateCS = device->CreateComputeShader(cs_blob->GetBufferPointer(),
                                                   cs_blob->GetBufferSize(),
                                                   nullptr,
                                                   &csShader);

  if (FAILED(HRCreateCS)) {
    printf("Compute shader couldn't be created: ");
    ErrorDescription(HRCreateCS);
    return;
  }
}

void
CreateComputeBuffer() {
  D3D11_BUFFER_DESC bd;
  bd.ByteWidth = sizeof(float) * 1024;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  bd.StructureByteStride = sizeof(float);

  HRESULT HRCB = device->CreateBuffer(&bd, nullptr, &computebuffer);

  if (FAILED(HRCB)) {
    printf("Compute buffer couldn't be created: ");
    ErrorDescription(HRCB);
    return;
  }

  D3D11_BUFFER_UAV uavbDesc;
  uavbDesc.Flags = 0;
  uavbDesc.FirstElement = 0;
  uavbDesc.NumElements = 1024;

  D3D11_UNORDERED_ACCESS_VIEW_DESC UAV;
  UAV.Format = DXGI_FORMAT_R32_FLOAT;
  UAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  UAV.Buffer = uavbDesc;

  HRESULT HRUAV = device->CreateUnorderedAccessView(computebuffer, &UAV, &uavBuffer);

  if (FAILED(HRUAV)) {
    printf("UAV couldn't be created: ");
    ErrorDescription(HRUAV);
    return;
  }

  D3D11_BUFFER_SRV srvuavDesc;
  srvuavDesc.FirstElement = 0;
  srvuavDesc.ElementOffset = 0;
  srvuavDesc.NumElements = 1;
  srvuavDesc.ElementWidth = 1024;

  D3D11_SHADER_RESOURCE_VIEW_DESC SRUAV;
  SRUAV.Format = DXGI_FORMAT_R32_FLOAT;
  SRUAV.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
  SRUAV.Buffer = srvuavDesc;

  HRESULT HRUAVSV = device->CreateShaderResourceView(computebuffer,
                                                     &SRUAV,
                                                     &srvuav);
}

void
render(uint32& gProgramID,
       uint32* gVBO,
       uint32& gIBO,
       Matrix4x4 View,
       Matrix4x4 Projection,
       int32& gVertexPosition2D,
       int32& gUV,
       int32& gVertexColor,
       unsigned int texture) {

  context->CSSetUnorderedAccessViews(0, 1, &uavBuffer, nullptr);
  context->CSSetShader(csShader, nullptr, 0);
  context->Dispatch(4, 4, 1);

  ID3D11UnorderedAccessView* UAV_NULL = nullptr;
  context->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, 0);

  float color[4] = { 1.0f, 0.0f, 1.0f, 1.0f };

  context->RSSetViewports(1, &viewport);

  context->RSSetState(rasterizer);
  
  context->OMSetDepthStencilState(DSState, 1);

  context->OMSetRenderTargets(1, &renderTargetView, DSV);

  context->ClearRenderTargetView(renderTargetView, color);
  context->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

  context->VSSetShader(vsShader, nullptr, 0);
  context->PSSetShader(psShader, nullptr, 0);

  Matrix4x4 matrices[2] = {
    View,
    Projection
  };

  context->UpdateSubresource(constantbuffer, 0, nullptr, matrices, 0, 0);

  context->VSSetShaderResources(0, 1, &srvuav);
  context->VSSetConstantBuffers(0, 1, &constantbuffer);
  context->PSSetConstantBuffers(0, 1, &constantbuffer);

  context->PSSetSamplers(0, 1, &samplerState);

  context->PSSetShaderResources(0, 1, &checkerTexture);

  context->IASetInputLayout(inputLayout);

  context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  uint32 stride = sizeof(Vertex);
  uint32 offset = 0;
  context->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);

  context->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);

  //context->DrawIndexed(6, 0, 0);
  context->DrawIndexedInstanced(6, 1024, 0, 0, 0);

  swapchain->Present(1, 0);

  ID3D11ShaderResourceView* SRV_NULL = nullptr;
  context->VSSetShaderResources(0, 1, &SRV_NULL);
}