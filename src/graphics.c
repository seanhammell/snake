#include "src/graphics.h"

#include <stdio.h>

#include "SDL2/SDL.h"

#include "src/constants.h"

int graphics_init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  graphics.window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
  if (graphics.window == NULL) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return 1;
  }

  graphics.renderer = SDL_CreateRenderer(graphics.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (graphics.renderer == NULL) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return 1;
  }

  return 0;
}

void graphics_quit(void)
{
  SDL_DestroyRenderer(graphics.renderer);
  SDL_DestroyWindow(graphics.window);
  SDL_Quit();
}
