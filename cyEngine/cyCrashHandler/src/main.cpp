
#include "cyCrashHandlerPrerequisites.h"
#include <conio.h>
#include <SDL2/SDL.h>
#undef main;

using namespace CYLLENE_SDK;

int main(int argc, char* argv[]) {
  int renderFlags = 0;
  int windowFlags = 0;
  windowFlags = 0;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Crash Handler was not able to initialize SDL: \n" << SDL_GetError() << std::endl;
    exit(505);
  }
  SDL_Window* window = SDL_CreateWindow("Cyllene Engine Crash Handler",
                                       SDL_WINDOWPOS_CENTERED, 
                                       SDL_WINDOWPOS_CENTERED,
                                       1024,
                                       720, 
                                       windowFlags);
  if (!window)
  {
    std::cout << "Crash Handler was not able to initialize SDL window: \n" << SDL_GetError() << std::endl;
    exit(506);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, renderFlags);

  if (!renderer)
  {
    std::cout << "Crash Handler was not able to initialize SDL renderer: \n" << SDL_GetError() << std::endl;
    exit(507);
  }
}
