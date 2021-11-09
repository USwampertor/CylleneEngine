#define BISTRO 0
#define COW 1
#define QUAD 2
#define PLANT 3

#define SCENE BISTRO

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
#include <unordered_map>

#include "cyUtilitiesPrerequisites.h"

#include "cyCountAllocator.h"
#include "cyCountAllocatorPointer.h"

#include "cyFileSystem.h"
#include "cyMatrix3x3.h"
#include "cyMatrix4x4.h"
//#include "cyPlatformMath.h"
#include "cyQuaternion.h"
#include "cyTime.h"
#include "cyVector2f.h"
#include "cyVector3f.h"

#define durationCast std::chrono::duration_cast

int clientWidth = 1280;
int clientHeight = 720;

std::unordered_map<size_t, std::pair<ID3D11Texture2D*, ID3D11ShaderResourceView*>> textures;

using nanosecs = std::chrono::nanoseconds;
using millisecs = std::chrono::milliseconds;
using secs = std::chrono::seconds;
using steady_clock = std::chrono::steady_clock;
using namespace CYLLENE_SDK;

ID3D11Device* device;
ID3D11DeviceContext* context;

IDXGISwapChain1* swapchain;
ID3D11Resource* backbuffer;
ID3D11RenderTargetView* renderTargetView;
ID3D11UnorderedAccessView* unorderedAccessView;

Uint32 shadowSize = 4098;

D3D11_VIEWPORT cameraViewport;
D3D11_VIEWPORT shadowViewport;

ID3D11RasterizerState* rasterizer;

ID3D11Texture2D* depthStencil;
ID3D11DepthStencilState* DSState;
ID3D11DepthStencilView* DSV;

ID3D11Texture2D* shadowDepthStencil;
ID3D11DepthStencilView* shadowDSV;
ID3D11ShaderResourceView* shadowSRV;

ID3DBlob* cs_blob;
ID3D11ComputeShader* csShader;

ID3DBlob* gbuffer_vs_blob;
ID3D11VertexShader* GBufferVSShader;

ID3DBlob* gbuffer_fs_blob;
ID3D11PixelShader* GBufferPSShader;

ID3DBlob* ao_cs_blob;
ID3D11ComputeShader* AOCSShader;

ID3DBlob* image_cs_blob;
ID3D11ComputeShader* ImageCSShader;

ID3D11InputLayout* inputLayout;
ID3D11SamplerState* AnisotropicSamplerState;
ID3D11SamplerState* LinearSamplerState;
ID3D11SamplerState* PointSamplerState;
ID3D11UnorderedAccessView* uavBuffer;
ID3D11ShaderResourceView* srvuav;
ID3D11Buffer* computebuffer;
ID3D11Buffer* computeBufferConstantbuffer;
ID3D11Buffer* graphicsConstantbuffer;
ID3D11Buffer* aoConstantbuffer;
ID3D11Buffer* computeTextureConstantbuffer;
ID3D11Buffer* uvbuffer;
ID3D11Buffer* colorbuffer;

ID3D11Texture2D* checkterTexture;
ID3D11ShaderResourceView* checkerTextureSRV;

ID3D11Texture2D* GBuffer1Texture;
ID3D11RenderTargetView* GBuffer1RTV;
ID3D11ShaderResourceView* GBuffer1SRV;
ID3D11UnorderedAccessView* GBuffer1UAV;

ID3D11Texture2D* GBuffer2Texture;
ID3D11RenderTargetView* GBuffer2RTV;
ID3D11ShaderResourceView* GBuffer2SRV;
ID3D11UnorderedAccessView* GBuffer2UAV;

ID3D11Texture2D* AOTexture;
ID3D11RenderTargetView* AORTV;
ID3D11ShaderResourceView* AOSRV;
ID3D11UnorderedAccessView* AOUAV;

float g_aspectRatio;

struct Vertex
{
  Vector3f position;
  Vector2f texcoord;
  Vector3f normal;
};

struct Mesh
{
  uint32 nVertices;
  Vertex* vertices;

  uint32 nIndices;
  uint32* indices;

  ID3D11Buffer* indexbuffer;
  ID3D11Buffer* vertexbuffer;

  bool hasTexture;
  size_t TextureID;
};

struct Model
{
  Mesh meshes;
};

struct ComputeCB
{
  Matrix4x4 ShadowV;
  Matrix4x4 ShadowP;
  Matrix4x4 CameraV;
  Matrix4x4 CameraP;
  Vector2f  ScreenDimensions;
};

struct ComputeAOCB
{
  Matrix4x4 CameraView;
  Matrix4x4 CameraInvView;
  Matrix4x4 CameraInvProjection;
  float     Intensity;
  float     Scale;
  float     Bias;
  float     Sample;
  Vector2f  ScreenDimensions;
};

Vector<Mesh> Meshes;

void
ErrorDescription(HRESULT hr) {
  if (FACILITY_WINDOWS == HRESULT_FACILITY(hr)) {
    hr = HRESULT_CODE(hr);
  }

  TCHAR* szErrMsg;

  if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    hr,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    (LPTSTR)&szErrMsg,
                    0,
                    NULL) != 0) {
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

bool
CreateTexture(String texturePath, ID3D11Texture2D** texture2D, ID3D11ShaderResourceView** SRV);

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

// Initializes matrices and clear color
bool
initDX11(String basepath,
         uint32& gProgramID,
         uint32* gVBO,
         uint32& gIBO,
         int32& gVertexPosition2D,
         int32& gUV,
         int32& gVertexColor);

void
CreateComputeShader(String path,
                    ID3DBlob** csBlob,
                    ID3D11ComputeShader** cs,
                    String entryPoint = "main");

void
CreateVertexShader(String path,
                   ID3DBlob** vsBlob,
                   ID3D11VertexShader** vs,
                   String entryPoint = "main");

void
CreateFragmentShader(String path,
                     ID3DBlob** fsBlob,
                     ID3D11PixelShader** ps,
                     String entryPoint = "main");

void
CreateConstantBuffer(int32 sizeOfBuffer,
                     ID3D11Buffer** buffer);

void
CreateComputeBuffer(int32 typeSize,
                    int32 numberOfElements,
                    ID3D11Buffer** buffer,
                    ID3D11UnorderedAccessView** uav,
                    ID3D11ShaderResourceView** srv);

void
CreateRenderTarget(uint32 width,
                   uint32 height,
                   ID3D11Texture2D** buffer,
                   ID3D11RenderTargetView** rtv,
                   ID3D11ShaderResourceView** srv,
                   ID3D11UnorderedAccessView** uav);

void
render(float time,
       float deltaTime,
       Matrix4x4 ShadowView,
       Matrix4x4 ShadowProjection,
       Matrix4x4 CameraView,
       Matrix4x4 CameraProjection);

int
main(int argc, char** argv) {
  //int clientWidth = 1280, clientHeight = 720;
  //int clientWidth = 1920, clientHeight = 1080;

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

  std::cout << std::endl;

  /*
  {
    CAPointer<Vector4f> test1 = CAlloc.GetPointer<Vector4f>();
    *test1.ptr = Vector4f(0.0f, 1.0f, 2.0f, 3.0f);
    CAPointer<char> test2 = CAlloc.GetPointer<char>();
    *test2.ptr = 'C';
    CAPointer<float> test3 = CAlloc.GetPointer<float>();
    *test3.ptr = 12.742f;
    CAPointer<uint16> test4 = CAlloc.GetPointer<uint16>();
    *test4.ptr = 7;

    test1.~CAPointer();
    test3.~CAPointer();
    test4.~CAPointer();
    test2.~CAPointer();
  }
  */

  printf("\n");

  std::string currentDirectory(argv[0]);
  currentDirectory = currentDirectory.substr(0, currentDirectory.find_last_of('\\') + 1);
  printf("Current path: %s\n", currentDirectory.c_str());

  printf("\n");
  printf("Starting app with resolution %dx%d.\n", clientWidth, clientHeight);
  printf("\n");

  FreeImage_Initialise();

  printf("FreeImage version: %s\n", FreeImage_GetVersion());

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
#if defined OIS_WIN32_PLATFORM 
  pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
  pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
  pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM 
  pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
  pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
  pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
  pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif 

  OIS::InputManager* m_InputManager = OIS::InputManager::createInputSystem(pl);
  OIS::Mouse* m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject(OIS::OISMouse, true));
  OIS::Keyboard* m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject(OIS::OISKeyboard, true));

  const OIS::MouseState& ms = m_Mouse->getMouseState();
  ms.width = clientWidth;
  ms.height = clientHeight;

#if SCENE == QUAD
  Vector4f CamPosition(-5, 5, -5, 1);
#else
  Vector4f CamPosition(0, 0, -10, 1);
#endif
  Vector3f CamRight(1, 0, 0);
  Vector3f CamForward(0, 0, 1);

#if SCENE == BISTRO
  Vector4f ShadowCamPosition(-1320.0f, 4200.0f, 230.0f, 1.0f);
  Vector4f ShadowCamLookAt = Vector4f(0.0f, 300.0f, 0.0f, 1.0f) - ShadowCamPosition;
#elif SCENE == COW
  Vector4f ShadowCamPosition(-96.0f, 150.0f, -96.0f, 1.0f);
  Vector4f ShadowCamLookAt = Vector4f(96.0f, 102.4f, 96.0f, 1.0f);
#else
  Vector4f ShadowCamPosition(-96.0f, 150.0f, -96.0f, 1.0f);
  Vector4f ShadowCamLookAt = Vector4f(96.0f, 102.4f, 96.0f, 1.0f);
#endif

  Matrix4x4 ShadowView;
  ShadowView.View(ShadowCamPosition,
                  ShadowCamLookAt,
                  Vector4f(0, 1, 0, 0));

  Matrix4x4 ShadowProjection;
#if SCENE == BISTRO
  ShadowProjection.Orthogonal(5760.0f, 3240.0f, 0.01f, 10000.0f);
#elif SCENE == COW
  ShadowProjection.Orthogonal(270.0f, 270.0f, 0.01f, 10000.0f);
#else
  ShadowProjection.Orthogonal(270.0f, 270.0f, 0.01f, 10000.0f);
#endif

  Matrix4x4 Projection;
  Projection.Perspective(1920.0f, 1080.0f, 1.0f, 10000.0f, 60.0f * 0.0174533f);
  //Projection.Orthogonal(1920.0f, 1080.0f, 0.01f, 5000.0);

  bool open = true;
  double deltaTime = 0.0;
  steady_clock::time_point appStartedTime = steady_clock::now();
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

    float mouseDeltaX = 0.0f;
    float mouseDeltaY = 0.0f;

    if (m_Mouse->getMouseState().buttonDown(OIS::MB_Right)) {
      mouseDeltaX = m_Mouse->getMouseState().X.rel * deltaTime;
      mouseDeltaY = m_Mouse->getMouseState().Y.rel * deltaTime;
    }

#if SCENE == BISTRO
    float camSpeed = 500.0f;
#else
    float camSpeed = 15.0f;
#endif
    if (keyboardW) CamPosition += CamForward * camSpeed * deltaTime;
    if (keyboardS) CamPosition -= CamForward * camSpeed * deltaTime;
    if (keyboardA) CamPosition -= CamRight * camSpeed * deltaTime;
    if (keyboardD) CamPosition += CamRight * camSpeed * deltaTime;
    if (keyboardE) CamPosition.y += camSpeed * deltaTime;
    if (keyboardQ) CamPosition.y -= camSpeed * deltaTime;

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

    steady_clock::time_point now = steady_clock::now();
    nanosecs startedDiff = now - appStartedTime;
    double appTime = durationCast<millisecs>(startedDiff).count() / 1000.0f;

    render(appTime,
           deltaTime,
           ShadowView,
           ShadowProjection,
           View,
           Projection);

    //SDL_GL_SwapWindow(window);

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
  
  FreeImage_DeInitialise();

  return 0;
}

bool
DoTheImportThing(const std::string& pFile) {
  Path filePath(pFile);
  String directoryPath = filePath.directoryPath();

  // Create an instance of the Importer class
  Assimp::Importer importer;
  
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.

  Uint32 flags = 0;
  flags |= aiProcess_MakeLeftHanded;
  flags |= aiProcess_FlipWindingOrder;
  //flags |= aiProcess_FlipUVs;
  flags |= aiProcess_CalcTangentSpace;
  flags |= aiProcess_RemoveRedundantMaterials;
  flags |= aiProcess_LimitBoneWeights;
  flags |= aiProcess_Triangulate;
  //flags |= aiProcess_OptimizeGraph;
  flags |= aiProcess_OptimizeMeshes;
  flags |= aiProcess_JoinIdenticalVertices;
  flags |= aiProcess_FindInvalidData;
  flags |= aiProcess_GenUVCoords;

  const aiScene* scene = importer.ReadFile(pFile, flags);

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

  Meshes.resize(totalMeshes);

  for (int meshIndex = 0; meshIndex < totalMeshes; meshIndex++) {
    Mesh& mesh = Meshes[meshIndex];

    aiMesh* pMesh = scene->mMeshes[meshIndex];

    Uint32 materialIndex = pMesh->mMaterialIndex;
    aiMaterial* material = scene->mMaterials[materialIndex];
    aiString aiTexturePath;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath);
    mesh.hasTexture = false;
    if (aiTexturePath.length > 0) {
      std::hash<String> hasher;

      Path texturePath(directoryPath + aiTexturePath.C_Str());
      String textureName = texturePath.baseName();
      size_t textureID = hasher(textureName);
      if (textures.find(textureID) == textures.end()) {
        std::pair<ID3D11Texture2D*, ID3D11ShaderResourceView*> pair;
        if (CreateTexture(texturePath.fullPath(),
                          &pair.first,
                          &pair.second)) {
          mesh.hasTexture = true;
          mesh.TextureID = textureID;
          textures[textureID] = pair;
        }
        else {
          printf("Oh no.\n");
        }
      }
      else {
        mesh.hasTexture = true;
        mesh.TextureID = textureID;
      }
    }

    Uint32 totalTriangles = pMesh->mNumFaces;

    mesh.nIndices = totalTriangles * 3;
    mesh.indices = new uint32[mesh.nIndices];

    for (int32 t = 0, i = 0;
         t < totalTriangles;
         t++, i += 3) {
      aiFace face = pMesh->mFaces[t];

      if (face.mNumIndices != 3) {
        printf("Non triangle face, import aborted");
        return false;
      }

      mesh.indices[i + 0] = face.mIndices[0];
      mesh.indices[i + 1] = face.mIndices[1];
      mesh.indices[i + 2] = face.mIndices[2];
    }

    /*
    for (int i = 0; i < 8; i++) {
      printf("Mesh index %i uvs: %s\n", i, pMesh->HasTextureCoords(i) ? "Yes" : "No");
      pMesh->HasTextureCoords(i);
    }
    */

    bool hasUVs = pMesh->HasTextureCoords(0);
    aiVector3D* texCoords = nullptr;
    if (hasUVs)
      texCoords = pMesh->mTextureCoords[0];

    mesh.nVertices = pMesh->mNumVertices;
    mesh.vertices = new Vertex[mesh.nVertices];
    for (int32 i = 0; i < mesh.nVertices; i++) {
      mesh.vertices[i].position = Vector3f(pMesh->mVertices[i].x,
                                           pMesh->mVertices[i].y,
                                           pMesh->mVertices[i].z);
      mesh.vertices[i].normal = Vector3f(pMesh->mNormals[i].x,
                                         pMesh->mNormals[i].y,
                                         pMesh->mNormals[i].z);
      if (hasUVs) {
        mesh.vertices[i].texcoord = Vector2f(texCoords[i].x,
                                             texCoords[i].y);
      }
      else {
        mesh.vertices[i].texcoord = Vector2f(0.0f, 0.0f);
      }
    }
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
CreateTexture(String texturePath, ID3D11Texture2D** texture2D, ID3D11ShaderResourceView** SRV) {
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(texturePath.c_str());

  uint32 flags = 0;
  FIBITMAP* fileImage = FreeImage_Load(fif, texturePath.c_str(), flags);
  //FreeImage_FlipVertical(pImage);
  //FreeImage_FlipHorizontal(pImage);

  if (fileImage == nullptr) {
    printf("Error while loading image\n");
    return false;
  }

  //FIBITMAP* standardImage = FreeImage_ConvertToStandardType(fileImage);
  //FIBITMAP* image = FreeImage_ConvertToType(fileImage, FIT_BITMAP);
  //FIBITMAP* imageF = FreeImage_ConvertToType(fileImage, FIT_BITMAP);
  //FIBITMAP* image = FreeImage_ConvertToStandardType(fileImage);
  //FIBITMAP* imageF = FreeImage_ConvertToRGBAF(fileImage);
  //FIBITMAP* image = FreeImage_ConvertToRGBA16(fileImage);
  //FIBITMAP* image = FreeImage_ConvertTo32Bits(imageF);
  FIBITMAP* image = FreeImage_ConvertTo32Bits(fileImage);
  FreeImage_Unload(fileImage);
  //FreeImage_Unload(imageF);
  //FreeImage_Unload(image);

  if (image == nullptr) {
    printf("Error while converting image\n");
    return false;
  }

  //FIBITMAP* pImageRGB = FreeImage_ConvertToType(pImage, FIT_BITMAP);
  //FIBITMAP* pImageRGB = FreeImage_ConvertTo32Bits(pImage);
  //FIBITMAP* pImageRGB = FreeImage_ConvertToRGBAF(pImage);
  //FreeImage_SetTransparent(pImageRGB, true);

  int nWidth = FreeImage_GetWidth(image);
  int nHeight = FreeImage_GetHeight(image);
  FREE_IMAGE_COLOR_TYPE fict = FreeImage_GetColorType(image);

  uint32 pitch = FreeImage_GetPitch(image);

  /*
  BITMAPINFO* imageInfo = FreeImage_GetInfo(image);
  printf("Format: %i\n", fif);
  printf("Width: %i\n", nWidth);
  printf("Height: %i\n", nHeight);
  printf("Bit count: %i\n", imageInfo->bmiHeader.biBitCount);
  printf("Size: %i\n", imageInfo->bmiHeader.biSize);
  printf("Image size: %i\n", imageInfo->bmiHeader.biSizeImage);
  printf("Image byte pitch: %i\n", pitch);
  printf("Color type: %i\n", fict);
  */

  BYTE* imageBytes = FreeImage_GetBits(image);

  if (imageBytes == nullptr) {
    printf("Error while getting image bytes\n");
    return false;
  }

  D3D11_SUBRESOURCE_DATA initData = { imageBytes, pitch, pitch * nHeight };

  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = nWidth;
  desc.Height = nHeight;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  //desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
  //desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = 0;
  desc.MiscFlags = 0;

  HRESULT HRTexture = device->CreateTexture2D(&desc, &initData, texture2D);

  if (FAILED(HRTexture)) {
    printf("Texture couldn't be created: ");
    ErrorDescription(HRTexture);
    return false;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
  SRVDesc.Format = desc.Format;
  SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  SRVDesc.Texture2D.MipLevels = 1;

  HRTexture = device->CreateShaderResourceView(*texture2D,
                                               &SRVDesc,
                                               SRV);

  if (FAILED(HRTexture)) {
    printf("Texture SRV couldn't be created: ");
    ErrorDescription(HRTexture);
    return false;
  }

  FreeImage_Unload(image);
  return true;
}

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
  sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.Stereo = false;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_SHADER_INPUT;
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

  // Create swapchain back buffer RTV
  {
    HRESULT HRCreateRTV = device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);

    if (FAILED(HRCreateRTV)) {
      printf("Backbuffer RTV couldn't be created: ");
      ErrorDescription(HRCreateRTV);
      return false;
    }
  }

  // Create swapchain back buffer UAV
  {
    HRESULT HRCreateUAV = device->CreateUnorderedAccessView(backbuffer, nullptr, &unorderedAccessView);

    if (FAILED(HRCreateUAV)) {
      printf("Backbuffer UAV couldn't be created: ");
      ErrorDescription(HRCreateUAV);
      return false;
    }
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

  // Create Depth Stencil view
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Flags = 0;
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    HRESULT HRDepthStencilView = device->CreateDepthStencilView(depthStencil, // Depth stencil texture
                                                                nullptr, // Depth stencil desc
                                                                &DSV);  // [out] Depth stencil view

    if (FAILED(HRDepthStencilView)) {
      printf("Depth Stencil View couldn't be created: ");
      ErrorDescription(HRDepthStencilView);
      return false;
    }
  }

  // Create Depth Stencil State
  {
    D3D11_DEPTH_STENCIL_DESC dsDesc;

    // Depth test parameters
    dsDesc.DepthEnable = true;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

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

  // Create Shadow Depth Stencil texture
  {
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = shadowSize;
    descDepth.Height = shadowSize;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    HRESULT HRDepthStencil = device->CreateTexture2D(&descDepth, NULL, &shadowDepthStencil);

    if (FAILED(HRDepthStencil)) {
      printf("Shadow Depth Stencil texture couldn't be created: ");
      ErrorDescription(HRDepthStencil);
      return false;
    }
  }

  // Create Shadow Depth Stencil View
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Flags = 0;
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    // Create the depth stencil view
    HRESULT HRDepthStencilView = device->CreateDepthStencilView(shadowDepthStencil, // Depth stencil texture
                                                                &descDSV, // Depth stencil desc
                                                                &shadowDSV);  // [out] Depth stencil view

    if (FAILED(HRDepthStencilView)) {
      printf("Shadow Depth Stencil View couldn't be created: ");
      ErrorDescription(HRDepthStencilView);
      return false;
    }
  }
  
  // Create Shadow Shader Resource View
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView* rt = nullptr;
    HRESULT HRCreateSR = device->CreateShaderResourceView(shadowDepthStencil,
                                                          &SRVDesc,
                                                          &shadowSRV);

    if (FAILED(HRCreateSR)) {
      printf("Shadow Shader Resource View couldn't be created: ");
      ErrorDescription(HRCreateSR);
      return false;
    }
  }

  // Viewport
  cameraViewport.Width = clientWidth;
  cameraViewport.Height = clientHeight;
  cameraViewport.TopLeftX = 0.0f;
  cameraViewport.TopLeftY = 0.0f;
  cameraViewport.MinDepth = 0.0f;
  cameraViewport.MaxDepth = 1.0f;

  shadowViewport.Width = shadowSize;
  shadowViewport.Height = shadowSize;
  shadowViewport.TopLeftX = 0.0f;
  shadowViewport.TopLeftY = 0.0f;
  shadowViewport.MinDepth = 0.0f;
  shadowViewport.MaxDepth = 1.0f;

#if SCENE == PLANT
  CreateTexture(basepath + "Models/plant.png",
                &checkterTexture,
                &checkerTextureSRV);
#else
  CreateTexture(basepath + "../../Checker.webp",
                &checkterTexture,
                &checkerTextureSRV);
#endif

#if SCENE == BISTRO
  DoTheImportThing(basepath + "Models/BistroExterior.fbx");
#elif SCENE == COW
  DoTheImportThing(basepath + "Models/Cow.fbx");
#elif SCENE == PLANT
  DoTheImportThing(basepath + "Models/Plant.obj");
#elif SCENE == ARROW
  DoTheImportThing(basepath + "Models/ArrowRight.fbx");
#else
  DoTheImportThing(basepath + "Models/ScreenAlignedQuad.3ds");
#endif

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
  CreateComputeShader(basepath + "Shaders/hlsl/ComputeBuffer.hlsl",
                      &cs_blob,
                      &csShader);

  CreateVertexShader(basepath + "Shaders/hlsl/GBufferVertex.hlsl",
                     &gbuffer_vs_blob,
                     &GBufferVSShader);

  
  CreateFragmentShader(basepath + "Shaders/hlsl/GBufferFragment.hlsl",
                       &gbuffer_fs_blob,
                       &GBufferPSShader);

  CreateComputeShader(basepath + "Shaders/hlsl/ComputeSSAO.hlsl",
                      &ao_cs_blob,
                      &AOCSShader);

  CreateComputeShader(basepath + "Shaders/hlsl/ComputeTexture.hlsl",
                      &image_cs_blob,
                      &ImageCSShader);

  // create the input layout
  {
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    HRESULT HRInputLayout = device->CreateInputLayout(ied,
                                                      ARRAYSIZE(ied),
                                                      gbuffer_vs_blob->GetBufferPointer(),
                                                      gbuffer_vs_blob->GetBufferSize(),
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
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 8;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0;

    HRESULT HRSampler = device->CreateSamplerState(&samplerDesc, &AnisotropicSamplerState);

    if (FAILED(HRSampler)) {
      printf("Anisotropic Sampler state couldn't be created: ");
      ErrorDescription(HRSampler);
      return false;
    }
  }

  // create sampler
  {
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0;

    HRESULT HRSampler = device->CreateSamplerState(&samplerDesc, &LinearSamplerState);

    if (FAILED(HRSampler)) {
      printf("Linear Sampler state couldn't be created: ");
      ErrorDescription(HRSampler);
      return false;
    }
  }

  // create sampler
  {
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MipLODBias = 0;

    HRESULT HRSampler = device->CreateSamplerState(&samplerDesc, &PointSamplerState);

    if (FAILED(HRSampler)) {
      printf("Point Sampler state couldn't be created: ");
      ErrorDescription(HRSampler);
      return false;
    }
  }

#if SCENE == QUAD
  CreateComputeBuffer(sizeof(float) * 2, 1024 * 8 * 8, &computebuffer, &uavBuffer, &srvuav);
#else
  CreateComputeBuffer(sizeof(float) * 2, 1024, &computebuffer, &uavBuffer, &srvuav);
#endif

  CreateRenderTarget(clientWidth, clientHeight, &GBuffer1Texture, &GBuffer1RTV, &GBuffer1SRV, &GBuffer1UAV);
  
  CreateRenderTarget(clientWidth, clientHeight, &GBuffer2Texture, &GBuffer2RTV, &GBuffer2SRV, &GBuffer2UAV);

  CreateRenderTarget(clientWidth, clientHeight, &AOTexture, &AORTV, &AOSRV, &AOUAV);

  CreateConstantBuffer(sizeof(Vector2f),
                       &computeBufferConstantbuffer);

  CreateConstantBuffer(sizeof(Matrix4x4) * 3,
                       &graphicsConstantbuffer);

  CreateConstantBuffer((sizeof(Matrix4x4) * 3) +
                       (sizeof(float) * 6),
                       &aoConstantbuffer);

  CreateConstantBuffer((sizeof(Matrix4x4) * 4) + sizeof(Vector2f),
                       &computeTextureConstantbuffer);

  for (Mesh& mesh : Meshes) {
    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = sizeof(Vertex) * mesh.nVertices;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA srd = { mesh.vertices, 0, 0 };
    HRESULT HRIndex = device->CreateBuffer(&bd, &srd, &mesh.vertexbuffer);

    if (FAILED(HRIndex)) {
      printf("Mesh Vertex buffer couldn't be created: ");
      ErrorDescription(HRIndex);
      return false;
    }
  }

  for (Mesh& mesh : Meshes) {
    D3D11_BUFFER_DESC bd = { 0 };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(uint32) * mesh.nIndices;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA srd = { mesh.indices, 0, 0 };
    HRESULT HRIndex = device->CreateBuffer(&bd, &srd, &mesh.indexbuffer);

    if (FAILED(HRIndex)) {
      printf("Mesh index buffer couldn't be created: ");
      ErrorDescription(HRIndex);
      return false;
    }
  }

  return true;
}

void
CreateComputeShader(String path,
                    ID3DBlob** csBlob,
                    ID3D11ComputeShader** cs,
                    String entryPoint) {
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
                            entryPoint.c_str(),
                            "cs_5_0",
                            flags,
                            0,
                            csBlob,
                            &errorBlob);

  if (FAILED(HRCS)) {
    printf("Compute shader couldn't be compiled: ");
    printf("%s\n", (char*)errorBlob->GetBufferPointer());
    errorBlob->Release();
    return;
  }

  HRESULT HRCreateCS = device->CreateComputeShader((*csBlob)->GetBufferPointer(),
                                                   (*csBlob)->GetBufferSize(),
                                                   nullptr,
                                                   cs);

  if (FAILED(HRCreateCS)) {
    printf("Compute shader couldn't be created: ");
    ErrorDescription(HRCreateCS);
    return;
  }
}

void
CreateVertexShader(String path,
                   ID3DBlob** vsBlob,
                   ID3D11VertexShader** vs,
                   String entryPoint) {
  File VSFile = FileSystem::open(path);

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
                                      entryPoint.c_str(),
                                      "vs_5_0",
                                      flags,
                                      0,
                                      vsBlob,
                                      &errorBlob);

  if (FAILED(HRVertexShader)) {
    printf("Vertex shader couldn't be compiled: ");
    printf("%s\n", (char*)errorBlob->GetBufferPointer());

    errorBlob->Release();
  }

  HRESULT HRCreateVS = device->CreateVertexShader((*vsBlob)->GetBufferPointer(),
                                                  (*vsBlob)->GetBufferSize(),
                                                  nullptr,
                                                  vs);

  if (FAILED(HRCreateVS)) {
    printf("Vertex shader couldn't be created: ");
    ErrorDescription(HRCreateVS);
    return;
  }
}

void
CreateFragmentShader(String path,
                     ID3DBlob** fsBlob,
                     ID3D11PixelShader** ps,
                     String entryPoint) {
  File FSFile = FileSystem::open(path);
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
                                        entryPoint.c_str(),
                                        "ps_5_0",
                                        flags,
                                        0,
                                        fsBlob,
                                        &errorBlob);

  if (FAILED(HRFragmentShader)) {
    printf("Fragment shader couldn't be compiled: ");
    printf("%s\n", (char*)errorBlob->GetBufferPointer());

    errorBlob->Release();

    return;
  }

  HRESULT HRCreatePS = device->CreatePixelShader((*fsBlob)->GetBufferPointer(),
                                                 (*fsBlob)->GetBufferSize(),
                                                 nullptr,
                                                 ps);

  if (FAILED(HRCreatePS)) {
    printf("Pixel shader couldn't be created: ");
    ErrorDescription(HRCreatePS);

    return;
  }
}

void
CreateConstantBuffer(int32 sizeOfBuffer,
                     ID3D11Buffer** buffer) {
  D3D11_BUFFER_DESC bd;
  bd.ByteWidth = std::ceil(sizeOfBuffer / 16.0f) * 16;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = 0;
  bd.StructureByteStride = 0;

  //D3D11_SUBRESOURCE_DATA srd = { &initData, 0, 0 };
  HRESULT HRCB = device->CreateBuffer(&bd, nullptr, buffer);

  if (FAILED(HRCB)) {
    printf("Constant buffer couldn't be created: ");
    ErrorDescription(HRCB);
    return;
  }
}

void
CreateComputeBuffer(int32 typeSize,
                    int32 numberOfElements,
                    ID3D11Buffer** buffer,
                    ID3D11UnorderedAccessView** uav,
                    ID3D11ShaderResourceView** srv) {
  D3D11_BUFFER_DESC bd;
  bd.ByteWidth = typeSize * numberOfElements;
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
  bd.CPUAccessFlags = 0;
  bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
  bd.StructureByteStride = typeSize;

  HRESULT HRCB = device->CreateBuffer(&bd, nullptr, buffer);

  if (FAILED(HRCB)) {
    printf("Compute buffer couldn't be created: ");
    ErrorDescription(HRCB);
    return;
  }

  D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDescriptor;
  UAVDescriptor.Format = DXGI_FORMAT_UNKNOWN;
  UAVDescriptor.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  UAVDescriptor.Buffer.FirstElement = 0;
  UAVDescriptor.Buffer.NumElements = numberOfElements;
  UAVDescriptor.Buffer.Flags = 0;

  HRESULT HRUAV = device->CreateUnorderedAccessView(*buffer, &UAVDescriptor, uav);

  if (FAILED(HRUAV)) {
    printf("UAV couldn't be created: ");
    ErrorDescription(HRUAV);
    return;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC SRVDescriptor;
  SRVDescriptor.Format = DXGI_FORMAT_UNKNOWN;
  SRVDescriptor.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
  SRVDescriptor.Buffer.FirstElement = 0;
  SRVDescriptor.Buffer.NumElements = numberOfElements;

  HRESULT HRSRV = device->CreateShaderResourceView(*buffer,
                                                   &SRVDescriptor,
                                                   srv);

  if (FAILED(HRSRV)) {
    printf("SRV couldn't be created: ");
    ErrorDescription(HRSRV);
    return;
  }
}

void
CreateRenderTarget(uint32 width,
                   uint32 height,
                   ID3D11Texture2D** buffer,
                   ID3D11RenderTargetView** rtv,
                   ID3D11ShaderResourceView** srv,
                   ID3D11UnorderedAccessView** uav) {
  // Initialize the render target texture description.
  D3D11_TEXTURE2D_DESC textureDesc;
  ZeroMemory(&textureDesc, sizeof(textureDesc));

  //DXGI_FORMAT textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
  DXGI_FORMAT textureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

  // Setup the render target texture description.
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = textureFormat;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
                          D3D11_BIND_SHADER_RESOURCE |
                          D3D11_BIND_UNORDERED_ACCESS;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  HRESULT HRCB = device->CreateTexture2D(&textureDesc, nullptr, buffer);

  if (FAILED(HRCB)) {
    printf("RT buffer couldn't be created: ");
    ErrorDescription(HRCB);
    return;
  }

  // Create RTV
  {
    D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
    RTVDesc.Format = textureFormat;
    RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;

    HRESULT HRCreateRT = device->CreateRenderTargetView(*buffer, &RTVDesc, rtv);

    if (FAILED(HRCreateRT)) {
      printf("RT RTV couldn't be created: ");
      ErrorDescription(HRCreateRT);
      return;
    }
  }

  // Create SRV
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    SRVDesc.Format = textureFormat;
    SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView* rt = nullptr;
    HRESULT HRCreateSR = device->CreateShaderResourceView(*buffer, &SRVDesc, srv);

    if (FAILED(HRCreateSR)) {
      printf("RT SRV couldn't be created: ");
      ErrorDescription(HRCreateSR);
      return;
    }
  }

  // Create UAV
  {
    D3D11_UNORDERED_ACCESS_VIEW_DESC UAV;
    UAV.Format = textureFormat;
    UAV.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    UAV.Buffer.Flags = 0;
    UAV.Buffer.FirstElement = 0;
    UAV.Buffer.NumElements = 1;

    HRESULT HRCreateUAV = device->CreateUnorderedAccessView(*buffer, &UAV, uav);

    if (FAILED(HRCreateUAV)) {
      printf("RT UAV couldn't be created: ");
      ErrorDescription(HRCreateUAV);
      return;
    }
  }
}

void
render(float time,
       float deltaTime,
       Matrix4x4 ShadowView,
       Matrix4x4 ShadowProjection,
       Matrix4x4 CameraView,
       Matrix4x4 CameraProjection) {
  float color[4] = { 0.3f, 0.3f, 0.3f, 0.0f };
  float normal[4] = { 0.5f, 0.5f, 1.0f, 1.0f };
  Quaternion worldRotation;
  worldRotation.fromEuler(Euler(3.14159265f * 0.5f, 0.0f, 0.0f), 0);

  Matrix3x3 world3x3 = worldRotation.getRotationMatrix();
  Matrix4x4 world(world3x3.m[0][0], world3x3.m[0][1], world3x3.m[0][2], 0.0f,
                  world3x3.m[1][0], world3x3.m[1][1], world3x3.m[1][2], 0.0f,
                  world3x3.m[2][0], world3x3.m[2][1], world3x3.m[2][2], 0.0f,
                  0.0f,             0.0f,             0.0f,             1.0f);

  float timeVariables[2] = { time, deltaTime };

  Matrix4x4 shadowMatrices[3] = { world, ShadowView, ShadowProjection };

  Matrix4x4 cameraMatrices[3] = { world, CameraView, CameraProjection };

  ComputeCB computeData;
  computeData.ShadowV = ShadowView;
  computeData.ShadowP = ShadowProjection;
  computeData.CameraV = CameraView.inversed();
  computeData.CameraP = CameraProjection.inversed();
  computeData.ScreenDimensions = Vector2f(clientWidth, clientHeight);

  ComputeAOCB computeAOData;
  computeAOData.CameraView = CameraView;
  computeAOData.CameraInvView = CameraView.inversed();
  computeAOData.CameraInvProjection = CameraProjection.inversed();
  computeAOData.Intensity = 15.0f;
  computeAOData.Scale = 0.4f;
  computeAOData.Bias = 0.3f;
  computeAOData.Sample = 7.0f;
  computeAOData.ScreenDimensions = Vector2f(clientWidth, clientHeight);

  // Update compute buffers
  {
    context->UpdateSubresource(computeBufferConstantbuffer, 0, nullptr, &timeVariables[0], 0, 0);
  }

  // Compute shader buffer
  {
    context->CSSetShader(csShader, nullptr, 0);

    context->CSSetConstantBuffers(0, 1, &computeBufferConstantbuffer);
    context->CSSetUnorderedAccessViews(0, 1, &uavBuffer, nullptr);

#if SCENE == QUAD
    context->Dispatch(8, 8, 1);
#else
    context->Dispatch(1, 1, 1);
#endif

    ID3D11Buffer* CB_NULL = nullptr;
    ID3D11UnorderedAccessView* UAV_NULL = nullptr;
    context->CSSetConstantBuffers(0, 1, &CB_NULL);
    context->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, 0);

    context->CSSetShader(nullptr, nullptr, 0);
  }

  // Clear
  {
    context->ClearRenderTargetView(renderTargetView, color);
    context->ClearRenderTargetView(GBuffer1RTV, color);
    context->ClearRenderTargetView(GBuffer2RTV, normal);

    context->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

    context->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
  }
  
  // Input layout
  uint32 stride = sizeof(Vertex);
  uint32 offset = 0;
  {
    context->IASetInputLayout(inputLayout);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //context->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
    //
    //context->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);
  }

  // Camera Rasterizer
  {
    context->RSSetViewports(1, &shadowViewport);

    context->RSSetState(rasterizer);
  }
  
  // Update shadow buffers
  {
    context->UpdateSubresource(graphicsConstantbuffer, 0, nullptr, &shadowMatrices[0], 0, 0);
  }

  // Shadow Graphics shaders
  {
    // Vertex shader
    context->VSSetShader(GBufferVSShader, nullptr, 0);

    context->VSSetConstantBuffers(0, 1, &graphicsConstantbuffer);

    context->VSSetShaderResources(0, 1, &srvuav);
  }

  // Output Merge
  {
    context->OMSetDepthStencilState(DSState, 0);

    ID3D11RenderTargetView* RTV_NULL = nullptr;
    context->OMSetRenderTargets(1, &RTV_NULL, shadowDSV);
  }

  for (const Mesh& mesh : Meshes) {
    context->IASetVertexBuffers(0, 1, &mesh.vertexbuffer, &stride, &offset);

    context->IASetIndexBuffer(mesh.indexbuffer, DXGI_FORMAT_R32_UINT, 0);

#if SCENE == COW
    //context->DrawIndexedInstanced(mesh.nIndices, 1024, 0, 0, 0);
    context->DrawIndexedInstanced(mesh.nIndices, 1, 0, 0, 0);
#elif SCENE == QUAD
    //context->DrawIndexedInstanced(mesh.nIndices, 1024 * 4 * 4, 0, 0, 0);
#else
    context->DrawIndexed(mesh.nIndices, 0, 0);
#endif
  }

  // Unbind
  {
    ID3D11Buffer* BUFFER_NULL = nullptr;
    context->VSSetConstantBuffers(0, 1, &BUFFER_NULL);

    ID3D11ShaderResourceView* SRV_NULL = nullptr;
    context->VSSetShaderResources(0, 1, &SRV_NULL);

    ID3D11RenderTargetView* RTV_NULL = nullptr;
    context->OMSetRenderTargets(1, &RTV_NULL, nullptr);

    context->VSSetShader(nullptr, nullptr, 0);

    context->PSSetShader(nullptr, nullptr, 0);
  }

  // Camera Rasterizer
  {
    context->RSSetViewports(1, &cameraViewport);

    context->RSSetState(rasterizer);
  }
  
  // Update camera buffers
  {
    context->UpdateSubresource(graphicsConstantbuffer, 0, nullptr, &cameraMatrices[0], 0, 0);
  }

  // Camera Graphics shaders
  {
    // Vertex shader
    context->VSSetShader(GBufferVSShader, nullptr, 0);

    context->VSSetConstantBuffers(0, 1, &graphicsConstantbuffer);

    context->VSSetShaderResources(0, 1, &srvuav);

    // Pixel shader
    context->PSSetShader(GBufferPSShader, nullptr, 0);

    context->PSSetConstantBuffers(0, 1, &graphicsConstantbuffer);
    
    context->PSSetShaderResources(0, 1, &checkerTextureSRV);

    context->PSSetSamplers(0, 1, &AnisotropicSamplerState);
  }

  // Output Merge
  {
    context->OMSetDepthStencilState(DSState, 0);

    //context->OMSetRenderTargets(1, &renderTargetView, DSV);
    ID3D11RenderTargetView* RTVs[2] = {
      GBuffer1RTV,
      GBuffer2RTV
    };
    context->OMSetRenderTargets(2, RTVs, DSV);
  }

  bool checker = false;
  for (const Mesh& mesh : Meshes) {
    context->IASetVertexBuffers(0, 1, &mesh.vertexbuffer, &stride, &offset);
    
    context->IASetIndexBuffer(mesh.indexbuffer, DXGI_FORMAT_R32_UINT, 0);

    if (mesh.hasTexture) {
      context->PSSetShaderResources(0, 1, &textures[mesh.TextureID].second);
      checker = false;
    }
    else {
      if (!checker) {
        context->PSSetShaderResources(0, 1, &checkerTextureSRV);
        checker = true;
      }
    }

#if SCENE == COW
    //context->DrawIndexedInstanced(mesh.nIndices, 1024, 0, 0, 0);
    context->DrawIndexedInstanced(mesh.nIndices, 1, 0, 0, 0);
#elif SCENE == QUAD
    context->DrawIndexedInstanced(mesh.nIndices, 1024 * 8 * 8, 0, 0, 0);
#else
    context->DrawIndexed(mesh.nIndices, 0, 0);
#endif
  }
  
  // Unbind
  {
    ID3D11Buffer* BUFFER_NULL = nullptr;
    context->VSSetConstantBuffers(0, 1, &BUFFER_NULL);

    ID3D11ShaderResourceView* SRV_NULL = nullptr;
    context->VSSetShaderResources(0, 1, &SRV_NULL);

    ID3D11RenderTargetView* RTV_NULL[2] = {
      nullptr,
      nullptr
    };
    context->OMSetRenderTargets(2, RTV_NULL, nullptr);

    context->VSSetShader(nullptr, nullptr, 0);

    context->PSSetShader(nullptr, nullptr, 0);
  }

  // Update AO compute buffer
  {
    context->UpdateSubresource(aoConstantbuffer, 0, nullptr, &computeAOData, 0, 0);
  }

  // Compute AO shader
  {
    context->CSSetShader(AOCSShader, nullptr, 0);

    context->CSSetUnorderedAccessViews(0, 1, &AOUAV, nullptr);

    context->CSSetShaderResources(0, 1, &GBuffer1SRV);
    context->CSSetShaderResources(1, 1, &GBuffer2SRV);

    context->CSSetSamplers(0, 1, &AnisotropicSamplerState);
    context->CSSetSamplers(1, 1, &LinearSamplerState);
    context->CSSetSamplers(2, 1, &PointSamplerState);

    context->CSSetConstantBuffers(0, 1, &aoConstantbuffer);

    context->Dispatch(std::ceil(clientWidth / 32.0f),
                      std::ceil(clientHeight / 32.0f),
                      1);

    ID3D11Buffer* CB_NULL = nullptr;
    ID3D11ShaderResourceView* SRV_NULL = nullptr;
    ID3D11UnorderedAccessView* UAV_NULL = nullptr;
    ID3D11SamplerState* SS_NULL = nullptr;

    context->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, nullptr);

    context->CSSetShaderResources(0, 1, &SRV_NULL);
    context->CSSetShaderResources(1, 1, &SRV_NULL);

    context->CSSetSamplers(0, 1, &SS_NULL);
    context->CSSetSamplers(1, 1, &SS_NULL);

    context->CSSetConstantBuffers(0, 1, &CB_NULL);

    context->CSSetShader(nullptr, nullptr, 0);
  }

  // Update compute buffers
  {
    context->UpdateSubresource(computeTextureConstantbuffer, 0, nullptr, &computeData, 0, 0);
  }

  // Compute shader image
  {
    context->CSSetShader(ImageCSShader, nullptr, 0);

    context->CSSetUnorderedAccessViews(0, 1, &unorderedAccessView, nullptr);

    context->CSSetConstantBuffers(0, 1, &computeTextureConstantbuffer);
    context->CSSetShaderResources(0, 1, &GBuffer1SRV);
    context->CSSetShaderResources(1, 1, &GBuffer2SRV);
    context->CSSetShaderResources(2, 1, &AOSRV);
    context->CSSetShaderResources(3, 1, &shadowSRV);
    context->CSSetSamplers(0, 1, &AnisotropicSamplerState);
    context->CSSetSamplers(1, 1, &PointSamplerState);

    context->Dispatch(std::ceil(clientWidth / 32.0f),
                      std::ceil(clientHeight / 32.0f), 1);

    ID3D11Buffer* CB_NULL = nullptr;
    ID3D11ShaderResourceView* SRV_NULL = nullptr;
    ID3D11UnorderedAccessView* UAV_NULL = nullptr;
    context->CSSetUnorderedAccessViews(0, 1, &UAV_NULL, 0);
    context->CSSetConstantBuffers(0, 1, &CB_NULL);
    context->CSSetShaderResources(0, 1, &SRV_NULL);
    context->CSSetShaderResources(1, 1, &SRV_NULL);
    context->CSSetShaderResources(2, 1, &SRV_NULL);
    context->CSSetShaderResources(3, 1, &SRV_NULL);

    context->CSSetShader(nullptr, nullptr, 0);
  }

  swapchain->Present(1, 0);
}