#include <chrono>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <iostream>

#include <GLEW/glew.h>
#include <gl/GLU.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_syswm.h>

#include <FreeImage/FreeImage.h>

void
printProgramLog(GLuint program);

void
printShaderLog(GLuint shader);

//Starts up SDL, creates window, and initializes OpenGL
bool
init(int clientWidth,
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
initGL(GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor);

void
render(GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
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

  if (!init(clientWidth,
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
  FIBITMAP* dib1 = NULL;
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

  HWND winHwnd = GetActiveWindow();

  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(window, &wmInfo);

  bool open = true;
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

    /*
    windowSurface = SDL_GetWindowSurface(window);

    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));

    SDL_UpdateWindowSurface(window);
    */

    render(gProgramID, gVBO, gIBO, gVertexPosition2D, gUV, gVertexColor, texture);

    SDL_GL_SwapWindow(window);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    Uint32 delay = (Uint32)(elapsedTime < 16 ? 16 - elapsedTime : 0);
    SDL_Delay(delay);
  }

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
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
init(int clientWidth,
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
  if (!initGL(gProgramID,
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
initGL(GLuint& gProgramID,
       GLuint* gVBO,
       GLuint& gIBO,
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor) {
  // Generate program
  gProgramID = glCreateProgram();
  
  GLenum error = GL_NO_ERROR;

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
    const GLchar* vertexShaderSource[] =
    {
"\
#version 330 core\n\
in vec3 LVertexPos; \n\
in vec2 LVertexUV; \n\
in vec3 LVertexColor; \n\
\n\
out vec2 vUVOut; \n\
out vec3 vColorOut; \n\
\n\
void \n\
main() { \n\
  vUVOut = LVertexUV; \n\
  vColorOut = LVertexColor; \n\
  gl_Position = vec4(LVertexPos.x, LVertexPos.y, 0, 1); \n\
}"
    };

    // Set vertex source
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

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

    // Get fragment source
    const GLchar* fragmentShaderSource[] =
    {
"\
#version 330 core\n\
in vec2 vUVOut; \n\
in vec3 vColorOut; \n\
 \n\
uniform sampler2D ourTexture; \n\
\n\
out vec4 LFragment; \n\
\n\
void \n\
main() { \n\
  //LFragment = vec4(vUVOut.xy, 0.0, 1.0); \n\
  //LFragment = vec4(vColorOut, 1.0); \n\
  //LFragment = vec4(vUVOut.xy, min(0.0, vColorOut.x), 1.0); \n\
  LFragment = vec4(texture(ourTexture, vUVOut).rgb, 1.0f) * vec4(vColorOut, 1.0f); \n\
  //LFragment = vec4(texture(ourTexture, vUVOut).rgb, 1.0f); \n\
}"
    };

    // Set fragment source
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

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
       GLint& gVertexPosition2D,
       GLint& gUV,
       GLint& gVertexColor,
       unsigned int texture) {
  // Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Bind program
  glUseProgram(gProgramID);

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