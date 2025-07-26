
#include <cyUnitTesting.h>
#include <cyCrashHandler.h>
#include <cySmartPointers.h>
#include <cyLogger.h>
#include <cyTime.h>
#include <cyWindow.h>
#include <cyEvent.h>
#include <cyWindowEvent.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <SDL3/SDL_opengl.h>

#include "cyCharGenUI.h";

// Using namespace for ease of use
using namespace CYLLENE_SDK;
using namespace ADND;

Vector2i windowSize(1280, 720);
CharGenUI charGenUI;
/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[]) {

  CrashHandler::startUp();
  SmartPointers::startUp();
  Logger::startUp();
  Time::startUp();

  WindowManager::startUp();

  WindowManager::instance().init();

  auto window = WindowManager::instance().createWindow("SDL3 Window", 
                                                       windowSize, 
                                                       WINDOW_FLAGS::E::eDEFAULT_WIN | 
                                                       WINDOW_FLAGS::E::eOPENGL);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window.lock().get());
  if (!gl_context) {
    SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
    return 1;
  }
  int quit = 0;
  Color clearColor = Color::MAGENTA;
  // Main loop
  bool running = true;
  float timer = 0.0f;

  // Setup Dear ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui_ImplSDL3_InitForOpenGL(window.lock().get(), gl_context);
  ImGui_ImplOpenGL3_Init("#version 150");

  WindowManager::instance().m_windowEvent.addListener([&](WPtr<WindowEvent> event) {
    ImGui_ImplSDL3_ProcessEvent(&event.lock()->m_sdlEvent);
    if (+EVENTTYPE::E::eQUIT == event.lock()->type) {
      std::cout << "Window closed" << std::endl;
      running = false;
    }
  });
  // SPtr<WEventQueue> eventQueue = WindowManager::instance().getWEventQueue(0);
  Time::instance().init();
  Time::instance().update();
  Time::instance().deltaTime();
  while (running) {
    // eventQueue->update();
    SDL_GetWindowSize(window.lock().get(), &windowSize.x, &windowSize.y);
    Time::instance().update();
    WindowManager::instance().update();
    timer += Time::instance().deltaTime();

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    
    charGenUI.render();

    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(Color::RED.r, Color::RED.g, Color::RED.b, Color::RED.a);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window.lock().get());

    // WindowManager::instance().setRenderColor(0, Color::RED);
    // WindowManager::instance().clear();
    // WindowManager::instance().present();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DestroyContext(gl_context);
  WindowManager::instance().destroyWindow(0);
  SDL_Quit();
  return 0;
}


