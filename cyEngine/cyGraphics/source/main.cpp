#include <chrono>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FreeImage/FreeImage.h>
#include <GLEW/glew.h>
#include <OIS/OIS.h>
#include <gl/GLU.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#include "cyVector3f.h"
#include "cyFileSystem.h"
#include "cyMatrix4x4.h"

struct Mesh
{
  CYLLENE_SDK::Vector3f* vertices;
  CYLLENE_SDK::Vector3f* indices;
  CYLLENE_SDK::int32 nVertices;
  CYLLENE_SDK::int32 nIndices;
};

struct Model
{
  Mesh* meshes;
};

bool
DoTheImportThing(const std::string& pFile);

void
printProgramLog(GLuint program);

void
printShaderLog(GLuint shader);

//Starts up SDL, creates window, and initializes OpenGL
bool
init(CYLLENE_SDK::String basepath,
     int clientWidth,
     int clientHeight,
     SDL_Window** window,
     SDL_GLContext& glContext,
     GLuint& gProgramID,
     GLuint* gVBO,
     GLuint& gIBO,
     GLint& gVertexPosition2D,
     GLint& gUV,
     GLint& gVertexColor);

//Initializes matrices and clear color
bool
initGL(CYLLENE_SDK::String basepath,
       GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor);

void
render(GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       CYLLENE_SDK::Matrix4x4 View,
       CYLLENE_SDK::Matrix4x4 Projection,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor,
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
  GLuint gProgramID = 0;
  // Vertex Atributtes Position
  GLint gVertexPosition2D = -1;
  // Vertex Atributtes UV
  GLint gUV = -1;
  // Vertex Atributtes Color
  GLint gVertexColor = -1;
  // OpenGL Vertex Buffer Object
  GLuint gVBO[3] = { 0, 0, 0 };
  // OpenGL Index Buffer Object
  GLuint gIBO = 0;

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
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

  printf("\n");

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture object)

  /***************************************************************************/
  FreeImage_Initialise();

  printf("FreeImage version: %s\n", FreeImage_GetVersion());
  std::string textureName = currentDirectory + "../../Checker.webp";
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(textureName.c_str());

  FIBITMAP* pImage = FreeImage_Load(fif, textureName.c_str(), 0);
  if (pImage) {
    int nWidth = FreeImage_GetWidth(pImage);
    int nHeight = FreeImage_GetHeight(pImage);
    FREE_IMAGE_COLOR_TYPE fict = FreeImage_GetColorType(pImage);

    BITMAPINFO* imageInfo = FreeImage_GetInfo(pImage);
    printf("Format: %i\n", fif);
    printf("Width: %i\n", nWidth);
    printf("Height: %i\n", nHeight);
    printf("Bit count: %i\n", imageInfo->bmiHeader.biBitCount);
    printf("Size: %i\n", imageInfo->bmiHeader.biSize);
    printf("Image size: %i\n", imageInfo->bmiHeader.biSizeImage);
    printf("Color type: %i", fict);

    BYTE* imageBytes = FreeImage_GetBits(pImage);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 nWidth,
                 nHeight,
                 0,
                 GL_BGR,
                 GL_UNSIGNED_BYTE,
                 reinterpret_cast<void*>(imageBytes));

    FreeImage_Unload(pImage);
  }
  else {
    printf("Error while loading image\n");
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_POINT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_POINT);

  // Generamos los mapas MIP, claro está.
  glGenerateMipmap(GL_TEXTURE_2D);

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

  CYLLENE_SDK::Vector4f CamPosition(0, 0, -10, 1);

  bool open = true;
  double deltaTime = 0.0;
  while (open) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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

    if (keyboardW) CamPosition.z += 10.0f * deltaTime;
    if (keyboardS) CamPosition.z -= 10.0f * deltaTime;
    if (keyboardA) CamPosition.x -= 10.0f * deltaTime;
    if (keyboardD) CamPosition.x += 10.0f * deltaTime;
    if (keyboardQ) CamPosition.y += 10.0f * deltaTime;
    if (keyboardE) CamPosition.y -= 10.0f * deltaTime;

    CYLLENE_SDK::Matrix4x4 View;
    View.View(CamPosition,
              CYLLENE_SDK::Vector4f(0, 0, 0, 0),
              CYLLENE_SDK::Vector4f(0, 1, 0, 0));

    CYLLENE_SDK::Matrix4x4 Projection;
    Projection.Perspective(1920.0f, 1080.0f, 0.3f, 1000.0f, 60.0f * 0.0174533f);

    render(gProgramID, gVBO, gIBO, View, Projection, gVertexPosition2D, gUV, gVertexColor, texture);

    SDL_GL_SwapWindow(window);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    Uint32 delay = (Uint32)(elapsedTime < 16 ? 16 - elapsedTime : 0);
    deltaTime = elapsedTime / 1000.0;
    //SDL_Delay(delay);
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

void
printProgramLog(GLuint name) {
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
printShaderLog(GLuint name) {
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

bool
init(CYLLENE_SDK::String basepath,
     int clientWidth,
     int clientHeight,
     SDL_Window** window,
     SDL_GLContext& glContext,
     GLuint& gProgramID,
     GLuint* gVBO,
     GLuint& gIBO,
     GLint& gVertexPosition2D,
     GLint& gUV,
     GLint& gVertexColor) {
  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Use OpenGL 3.1
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  // Remove old functionality
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // Create window
  *window = SDL_CreateWindow("SDL CylleneEngine",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             clientWidth,
                             clientHeight,
                             SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Create context
  glContext = SDL_GL_CreateContext(*window);
  if (glContext == NULL) {
    printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    return false;
  }

  // Initialize GLEW
  //glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
  }

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0) {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }

  //Initialize OpenGL
  if (!initGL(basepath,
              gProgramID,
              gVBO,
              gIBO,
              gVertexPosition2D,
              gUV,
              gVertexColor)) {
    printf("Unable to initialize OpenGL!\n");
    return false;
  }

  return true;
}

bool
initGL(CYLLENE_SDK::String basepath,
       GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor) {
  // Generate program
  gProgramID = glCreateProgram();
  
  GLenum error = GL_NO_ERROR;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  // Initialize Projection Matrix
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! %s\n", gluErrorString(error));
      return false;
    }
  }

  // Initialize Modelview Matrix
  {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! %s\n", gluErrorString(error));
      return false;
    }
  }

  // Create vertex shader
  {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Get vertex source
    CYLLENE_SDK::File VSFile = CYLLENE_SDK::FileSystem::open(basepath + "Shaders/Vertex.glsl");
    CYLLENE_SDK::String VSString = VSFile.readFile();

    const GLchar* vertexShaderSource = VSString.c_str();

    // Set vertex source
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile vertex source
    glCompileShader(vertexShader);

    // Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE) {
      printf("Unable to compile vertex shader %d!\n", vertexShader);
      printShaderLog(vertexShader);

      return false;
    }

    // Attach vertex shader to program
    glAttachShader(gProgramID, vertexShader);
  }

  // Create fragment shader
  {
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    CYLLENE_SDK::File PSFile = CYLLENE_SDK::FileSystem::open(basepath + "Shaders/Fragment.glsl");
    CYLLENE_SDK::String PSString = PSFile.readFile();

    // Get fragment source
    const GLchar* fragmentShaderSource = PSString.c_str();

    // Set fragment source
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile fragment source
    glCompileShader(fragmentShader);

    // Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE) {
      printf("Unable to compile fragment shader %d!\n", fragmentShader);
      printShaderLog(fragmentShader);
      return false;
    }

    // Attach fragment shader to program
    glAttachShader(gProgramID, fragmentShader);
  }

  // Link program
  {
    glLinkProgram(gProgramID);

    // Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
      printf("Error linking program %d!\n", gProgramID);
      printProgramLog(gProgramID);
      return false;
    }
  }

  // Get vertex position attribute location
  {
    gVertexPosition2D = glGetAttribLocation(gProgramID, "LVertexPos");
    if (gVertexPosition2D == -1) {
      printf("LVertexPos is not a valid glsl program variable!\n");
      return false;
    }
  }

  // Get vertex texcoord attribute location
  {
    gUV = glGetAttribLocation(gProgramID, "LVertexUV");
    if (gUV == -1) {
      printf("LVertexUV is not a valid glsl program variable!\n");
      return false;
    }
  }

  // Get vertex color attribute location
  {
    gVertexColor = glGetAttribLocation(gProgramID, "LVertexColor");
    if (gVertexColor == -1) {
      printf("LVertexColor is not a valid glsl program variable!\n");
      return false;
    }
  }

  // Initialize clear color
  {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! %s\n", gluErrorString(error));
      return false;
    }
  }

  // VBO (position) data
  GLfloat positionData[] = {
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,

    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };

  // VBO (uv) data
  GLfloat uvData[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };

  // VBO (color) data
  GLfloat colorData[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f
  };

  // IBO data
  GLuint indexData[] = { 0, 1, 2, 3, 4, 5 };

  unsigned long long vertices = 6;

  // Create VBO (Position)
  glGenBuffers(1, &gVBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
  glBufferData(GL_ARRAY_BUFFER, 3 * vertices * sizeof(GLfloat), positionData, GL_STATIC_DRAW);

  // Create VBO (uv)
  glGenBuffers(1, &gVBO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
  glBufferData(GL_ARRAY_BUFFER, 2 * vertices * sizeof(GLfloat), uvData, GL_STATIC_DRAW);

  // Create VBO (Color)
  glGenBuffers(1, &gVBO[2]);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
  glBufferData(GL_ARRAY_BUFFER, 3 * vertices * sizeof(GLfloat), colorData, GL_STATIC_DRAW);

  // Create IBO
  glGenBuffers(1, &gIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices * sizeof(GLuint), indexData, GL_STATIC_DRAW);

  return true;
}

void
render(GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       CYLLENE_SDK::Matrix4x4 View,
       CYLLENE_SDK::Matrix4x4 Projection,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor,
       unsigned int texture) {
  // Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Bind program
  glUseProgram(gProgramID);

  // Bind View matrix to uniforms
  int viewMatLocation = glGetUniformLocation(gProgramID, "View");
  glUniformMatrix4fv(viewMatLocation, 1, FALSE, &View.m[0][0]);

  // Bind Projection matrix to uniforms
  int projMatLocation = glGetUniformLocation(gProgramID, "Projection");
  glUniformMatrix4fv(projMatLocation, 1, FALSE, &Projection.m[0][0]);

  // Enable vertex position
  glEnableVertexAttribArray(gVertexPosition2D);
  glEnableVertexAttribArray(gUV);
  glEnableVertexAttribArray(gVertexColor);

  // Set vertex data
  glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
  glVertexAttribPointer(gVertexPosition2D, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

  glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);
  glVertexAttribPointer(gUV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

  glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);
  glVertexAttribPointer(gVertexColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

  // Set index data and render
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

  //Disable vertex position
  glDisableVertexAttribArray(gVertexPosition2D);

  //Unbind program
  glUseProgram(NULL);
}