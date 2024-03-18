#include <engine.h>
#include <utils.h>

void Init(Engine *engine) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    engine->window = SDL_CreateWindowFrom((void *)GetDesktopW());
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);

    if (engine->renderer) {
      SDL_SetRenderDrawColor(engine->renderer, 255, 255, 255, 255);
    }
  }
}

void Clean(Engine *engine) {
  SDL_DestroyTexture(engine->texture);
  SDL_DestroyWindow(engine->window);
  SDL_DestroyRenderer(engine->renderer);
  SDL_Quit();
}