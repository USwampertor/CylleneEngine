
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

// Using namespace for ease of use
using namespace CYLLENE_SDK;


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

  auto window = WindowManager::instance().createWindow("SDL3 Window", Vector2i(1280, 720));

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

  WindowManager::instance().m_windowEvent.addListener([&](WPtr<WindowEvent> event) {
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
    Time::instance().update();
    WindowManager::instance().update();
    timer += Time::instance().deltaTime();
  }
}


