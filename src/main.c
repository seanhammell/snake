#include <stdio.h>

#include "SDL2/SDL.h"

int init(SDL_Window **window, SDL_Renderer **renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return 1;
  }

  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return 1;
  }

  if (SDL_RenderSetLogicalSize(*renderer, 240, 240) != 0) {
    fprintf(stderr, "Error setting screen dimensions: %s\n", SDL_GetError());
    return 1;
  }

  return 0;
}

void destroy(SDL_Window **window, SDL_Renderer **renderer) {
  SDL_DestroyRenderer(*renderer);
  SDL_DestroyWindow(*window);
  SDL_Quit();
}

int main(void) {
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  if (init(&window, &renderer) != 0) {
    destroy(&window, &renderer);
    return 0;
  }

  SDL_Event e;
  for (;;) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        destroy(&window, &renderer);
        return 0;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0xB5, 0xA3, 0x7E, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  destroy(&window, &renderer);
  return 0;
}
