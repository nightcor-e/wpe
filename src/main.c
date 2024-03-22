#define SDL_MAIN_HANDLED

#include <engine.h>
#include <player.h>

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  Engine engine;
  Player player = {
    .formatContext = NULL,
    .videoCodec = NULL,
    .codecParameters = NULL,
  };

  SDL_Event event;

  const char *path = argv[1];
  uint32_t tick;

  if (!access(path, 0) == 0) {
    return 1;
  }

  Init(&engine);
  CreateTexture(&player, &engine, path);

  while (1) {
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
      Destroy(&engine);
      RemoveTexture(&player);

      return 0;
    }

    UpdateTexture(&player, &engine);

    tick = SDL_GetTicks();

    if (DELAY > SDL_GetTicks() - tick) {
      SDL_Delay(DELAY - (SDL_GetTicks() - tick));
    }
  }
}