#include <cyUnitTesting.h>

#include <cyGraphicsDX11API.h>
#include <cyDLLLoader.h>
#include <cyGraphicsAPI.h>
#include <cyWindow.h> 
#include <cyLogger.h> 
#include <cyResourceManager.h>
#include <cyShader.h>
#include <cyFileSystem.h>
#include <cyGShader.h>
#include <cyTime.h>
#include <cyGInputLayout.h>

// Using namespace for ease of use
using namespace CYLLENE_SDK;


/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[])
{
  Logger::startUp();
  ResourceManager::startUp();
  Time::startUp();


  WindowManager::startUp();
  WindowManager::instance().init();
  WindowManager::instance().createWindow("Test", Vector2i(1280, 720), 0);

  void* hwnd = WindowManager::instance().getWindowHandle(0);

  GraphicsDX11API::startUp<GraphicsDX11API>();
  GraphicsDX11API::instance().initialize(hwnd);

  Path resourceDir = FileSystem::getWorkingDirectory().directoryPath() + "../resources";
  File shaderVSF = FileSystem::open(resourceDir.fullPath() + "/vertexShader.hlsl");
  SPtr<ShaderResource> vsShaderR = ResourceManager::instance().loadFromPath<ShaderResource>(shaderVSF.path());
  File shaderPSF = FileSystem::open(resourceDir.fullPath() + "/pixelShader.hlsl");
  SPtr<ShaderResource> psShaderR = ResourceManager::instance().loadFromPath<ShaderResource>(shaderPSF.path());

  SPtr<GVertexShader> vShader = GraphicsDX11API::instance().createVertexShader(vsShaderR, 
                                                                               "vertex_main");

  SPtr<GPixelShader> pShader = GraphicsDX11API::instance().createPixelShader(psShaderR, 
                                                                             "pixel_main");


  Vector<GInputLayoutElement> inputDescs = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 28,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  SPtr<GInputLayout> pInputLayout = GraphicsDX11API::instance().createInputLayout(inputDescs, vShader);

  if (!pInputLayout) {
    WindowManager::ShowErrorMessage("Error", "Error creating Input Layout");
    return -1;
  }

  SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  bool running = true;
  float timer = 0.0f;
  while (running) {
    eventQueue->update();
    Time::instance().update();
    if (!eventQueue->empty()) {
      WindowEvent event = eventQueue->front();
      eventQueue->pop();

      switch (event.type)
      {
      case xwin::EventType::MouseMove:
        //mouse.x, mouse.y
        break;
      case xwin::EventType::Close:
        WindowManager::instance().destroyWindow(0);
        break;
      default:
        // Do nothing
        break;
      }
    }

    GraphicsDX11API::instance().setViewport(0, 0, 1280, 720);
    GraphicsDX11API::instance().clear(Color::MISSING);
    GraphicsDX11API::instance().present();
  }
  

  WindowManager::shutDown();

  return 0;

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}


