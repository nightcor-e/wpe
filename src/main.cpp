#define SDL_MAIN_HANDLED

#include <unistd.h>
#include <player.hpp>

int main(int argc, char **argv) {
  Player player = Player();
  std::string path = argv[1];
  SDL_Event event;
  uint32_t tick;

  if (!access(path.c_str(), 0) == 0) {
    return 1;
  }

  player.init(path);

  bool isRunning = true;

  while (isRunning) {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
      isRunning = false;
    }

    player.update();

    tick = SDL_GetTicks();

    if (DELAY > SDL_GetTicks() - tick) {
      SDL_Delay(DELAY - (SDL_GetTicks() - tick));
    }
  }

  player.clean();

  return 0;
}