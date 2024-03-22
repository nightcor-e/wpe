#pragma once

#include <SDL2/SDL.h>

typedef struct Engine {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
} Engine;

void Init(Engine *engine);
void Destroy(Engine *engine);